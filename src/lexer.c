#include "lexer.h"
#include "keywords.c"

Lexer *init_lexer(Arena *a, const unsigned char *src){
    Lexer *l = (Lexer*)arena_malloc(a, sizeof(Lexer));
    l->src = src;
    l->start = 0;
    l->forward = 0;
    l->line = 1;
    l->col = 1;
    l->arena = a;
    init_intern(a, &l->intern);
    return l;
}

static unsigned char peek(Lexer *l){
    return l->src[l->forward];
}

static unsigned char peek_next(Lexer *l){
    if(peek(l) == '\0'){
        return '\0';
    }
    return l->src[l->forward + 1];
}

static void advance(Lexer *l){
    if(peek(l) == '\0'){
        return;
    }

    unsigned char c = peek(l);
    l->forward++;

    if(c == '\n'){
        l->line++;
        l->col = 1;
    }
    else{
        l->col++;
    }
}

static bool match(Lexer *l, unsigned char to_match){
    if(peek(l) != to_match){
        return false;
    }
    advance(l);
    return true;
}

static Token* set_token(Lexer *l, TokenType type, size_t line, size_t col){
    uint32_t len = l->forward - l->start;
    const char *str = (const char*)&l->src[l->start];
    String *lex_str = intern_string(l->arena, &l->intern, str, len);

    Token *token = (Token*)arena_malloc(l->arena, sizeof(Token));
    token->lex = lex_str;
    token->type = type;
    token->line = line;
    token->col = col;
    
    return token;
}

static bool skip_comment(Lexer *l){
    if(peek_next(l) == '/'){
        while(peek(l) != '\n' && peek(l) != '\0'){
            advance(l);
        }
    }
    else if(peek_next(l) == '*'){
        advance(l);
        advance(l);
        while(!(peek(l) == '*' && peek_next(l) == '/') && peek(l) != '\0'){
            advance(l);
        }
        if(peek(l) == '\0'){
            return true;
        }
        advance(l);
        advance(l);
    }
    return false;
}

static bool skip_whitespace(Lexer *l){
    while(1){
        unsigned char c = peek(l);
        if(is_whitespace(c)){
            advance(l);
        }
        else if(c == '/'){
            if(skip_comment(l)){
                return true;
            }
        }
        else{
            return false;
        }
    }
}

static bool lex_hex_escape(Lexer *l){
    advance(l);
    if(!is_hex(peek(l))){
        return false;
    }
    while(is_hex(peek(l))){
        advance(l);
    }
    return true;
}

static bool lex_unicode_escape(Lexer *l){
    uint8_t req_digits = (peek(l) == 'u') ? 4 : 8;
    advance(l);
    for(uint8_t i=0; i<req_digits; i++){
        if(!is_hex(peek(l))){
            return false;
        }
        advance(l);
    }
    return true;
}

static bool lex_octal_escape(Lexer *l){
    advance(l);
    uint8_t count = 0;
    while(is_octal(peek(l)) && count < 3){
        advance(l);
        count++;
    }
    return true;
}

static Token* lex_literals(Lexer *l, unsigned char quote, TokenType type, size_t line, size_t col){
    uint8_t error = 0;
    uint32_t count = 0;
    while(peek(l) != quote && peek(l) != '\0'){
        if(peek(l) == '\\'){
            advance(l);
            if(peek(l) == 'x'){
                error = !lex_hex_escape(l);
            }
            else if(to_upper(peek(l)) == 'U'){
                error = !lex_unicode_escape(l);
            }
            else if(is_octal(peek(l))){
                lex_octal_escape(l);
            }
            else{
                error = !is_in_escape_list(peek(l));
                advance(l);
            }
            continue;
        }
        if(is_alnum(peek(l))){
            count++;
        }
        advance(l);
    }
    error |= (quote == '\'' && count > 1);
    if(peek(l) == quote){
        advance(l);
        return set_token(l, (error ? TOK_ERR : type), line, col);
    }
    return set_token(l, TOK_ERR, line, col);
}

static Token* lex_identifiers(Lexer *l, size_t tok_line, size_t tok_col){
    while(1){
        if(is_alnum(peek(l))){
            advance(l);
        }
        else if(peek(l) == '\\'){
            advance(l);
            if(to_upper(peek_next(l)) == 'U'){
                if(!lex_unicode_escape(l)){
                    return set_token(l, TOK_ERR, tok_line, tok_col);
                }
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
    uint32_t len = l->forward - l->start;
    const char *str = (const char*)&l->src[l->start];

    Token *token = (Token*)arena_malloc(l->arena, sizeof(Token));
    token->line = tok_line;
    token->col = tok_col;

    String *lex_str = intern_string(l->arena, &l->intern, str, len);
    const KeywordMap *kw = lookup_keyword(lex_str->data, len);
    token->lex = lex_str;

    if(kw != NULL){
        token->type = kw->type;
    }
    else{
        token->type = TOK_ID;
    }
    return token;
}

static bool lex_octal_numeral(Lexer *l){
    while(is_octal(peek(l))){
        advance(l);
    }
    if(is_digit(peek(l))){
        while(is_alnum(peek(l))){
            advance(l);
        }
        return false;
    }
    return true;
}

static bool lex_exp_numeral(Lexer *l){
    advance(l);
    if(peek(l) == '-' || peek(l) == '+'){
        advance(l);
    }
    if(!is_digit(peek(l))){
        return false;
    }
    while(is_digit(peek(l))){
        advance(l);
    }
    return true;
}

static bool lex_hex_numeral(Lexer *l, uint8_t *is_float, uint8_t *is_hex_float){
    uint8_t has_hex_digits = 0;
    uint8_t has_fraction_digits = 0;
    uint8_t has_exp_digits = 0;
    uint8_t has_exp = 0;

    while(is_hex(peek(l))){
        advance(l);
        has_hex_digits = 1;
    }

    if(peek(l) == '.'){
        *is_float = 1;
        advance(l);
        while(is_hex(peek(l))){
            advance(l);
            has_fraction_digits = 1;
        }
    }

    if(peek(l) == 'p' || peek(l) == 'P'){
        *is_float = 1;
        has_exp = 1;

        advance(l);

        if(peek(l) == '+' || peek(l) == '-'){
            advance(l);
        }
        while(is_digit(peek(l))){
            advance(l);
            has_exp_digits = 1;
        }
        if(!has_exp_digits){
            return false;
        }
    }
    if(*is_float){
        if(!has_exp){
            return false;
        }
        if(!has_hex_digits && !has_fraction_digits){
            return false;
        }
        if(has_fraction_digits){
            *is_hex_float = 1;
        }
    }
    if(!*is_float && !has_hex_digits){
        return false;
    }
    return true;
}

static bool lex_decimal_numeral(Lexer *l, uint8_t *is_float, uint8_t *is_exp){
    while(is_digit(peek(l))){
        advance(l);
    }
    if(peek(l) == '.'){
        *is_float = 1;
        advance(l);
        while(is_digit(peek(l))){
            advance(l);
        }
    }
    if(to_upper(peek(l)) == 'E'){
        *is_float = 1;
        *is_exp = 1;
        return lex_exp_numeral(l);
    }
    return true;
}

Token* next_token(Lexer *l){
    // whitespace
    if(skip_whitespace(l)){
        l->start = l->forward;
        return set_token(l, TOK_ERR, l->line, l->col);
    }

    l->start = l->forward;
    unsigned char c = peek(l);

    size_t tok_line = l->line;
    size_t tok_col = l->col;

    // eof
    if(c == '\0'){
        return set_token(l, TOK_EOF, tok_line, tok_col);
    }
    
    advance(l);

    // string lit
    if(c == '"' || ((c == 'L') && peek(l) == '"')){
        if(c == 'L'){
            advance(l);
        }
        return lex_literals(l, '"', TOK_LIT_STRING, tok_line, tok_col);
    }

    // char lit
    if(c == '\'' || ((c == 'L') && peek(l) == '\'')){
        if(c == 'L'){
            advance(l);
        }
        if(peek(l) == '\''){
            advance(l);
            return set_token(l, TOK_ERR, tok_line, tok_col);
        }
        return lex_literals(l, '\'', TOK_LIT_CHAR, tok_line, tok_col);
    }

    // ids and kw
    if(is_alpha(c)){
        return lex_identifiers(l, tok_line, tok_col);
    }

    // numerals
    if(is_digit(c) || (c == '.' && is_digit(peek(l)))){
        uint8_t is_float = 0;
        uint8_t is_exp = 0;
        uint8_t is_hex_float = 0;
        uint8_t error = 0;

        if(c == '.'){
            is_float = 1;
            while(is_digit(peek(l))){
                advance(l);
            }
            if(to_upper(peek(l)) == 'E'){
                error = !lex_exp_numeral(l);
            }
        }
        else if(c == '0' && (peek(l) == 'X' || peek(l) == 'x')){
            advance(l);
            error = !lex_hex_numeral(l, &is_float, &is_hex_float);
        }
        else if(c == '0' && is_octal(peek(l))){
            error = !lex_octal_numeral(l);
        }
        else{
            error = !lex_decimal_numeral(l, &is_float, &is_exp);
        }

        // float suffix
        if(peek(l) == 'f' || peek(l) == 'F'){
            is_float = 1;
            advance(l);
        }
        else if(peek(l) == 'l' || peek(l) == 'L'){
            // annoying float suffix
            if(is_float && (is_hex_float || is_exp)){
                advance(l);
            }
            // double float suffix
            else if(peek_next(l) == 'f' || peek_next(l) == 'F'){
                is_float = 1;
                advance(l);
                advance(l);
            }
        }
        // int suffix
        else if(!is_float){
            if(peek(l) == 'u' || peek(l) == 'U'){
                advance(l);
                if(peek(l) == 'l' || peek(l) == 'L'){
                    advance(l);
                    if(peek(l) == 'l' || peek(l) == 'L'){
                        advance(l);
                    }
                }
            }
            else if(peek(l) == 'l' || peek(l) == 'L'){
                advance(l);
                if(peek(l) == 'l' || peek(l) == 'L'){
                    advance(l);
                }
                if(peek(l) == 'u' || peek(l) == 'U'){
                    advance(l);
                }
            }
        }
        // suffix error
        if(is_alnum(peek(l))){
            while(is_alnum(peek(l))){
                advance(l);
            }
            return set_token(l, TOK_ERR, tok_line, tok_col);
        }
        // error check
        if(error){
            return set_token(l, TOK_ERR, tok_line, tok_col);
        }
        // float or int
        if(is_float){
            return set_token(l, TOK_NUM_FLOAT, tok_line, tok_col);
        }
        return set_token(l, TOK_NUM_INT, tok_line, tok_col);
    }
    
    // punctuators, separators, operators and other stuff
    switch(c){
        case '#':
            if(match(l, '#')){
                return set_token(l, TOK_HASH_HASH, tok_line, tok_col);
            } 
            return set_token(l, TOK_HASH, tok_line, tok_col);
        case '(':
            return set_token(l, TOK_LPAREN, tok_line, tok_col);
        case ')':
            return set_token(l, TOK_RPAREN, tok_line, tok_col);
        case '{':
            return set_token(l, TOK_LBRACE, tok_line, tok_col);
        case '}':
            return set_token(l, TOK_RBRACE, tok_line, tok_col);
        case '[':
            return set_token(l, TOK_LBRACKET, tok_line, tok_col);
        case ']':
            return set_token(l, TOK_RBRACKET, tok_line, tok_col);
        case ';':
            return set_token(l, TOK_SEMICOLON, tok_line, tok_col);
        case ',':
            return set_token(l, TOK_COMMA, tok_line, tok_col);
        case '?':
            return set_token(l, TOK_QUESTION, tok_line, tok_col);
        case ':':
            return set_token(l, TOK_COLON, tok_line, tok_col);
        case '~':
            return set_token(l, TOK_BIT_NOT, tok_line, tok_col);
        case '.':
            if(match(l, '.')){
                if(match(l, '.')){
                    return set_token(l, TOK_ELLIPSIS, tok_line, tok_col);
                }
                return set_token(l, TOK_ERR, tok_line, tok_col);
            }
            return set_token(l, TOK_DOT, tok_line, tok_col);
        case '+':
            if(match(l, '+')){
                return set_token(l, TOK_INC, tok_line, tok_col);
            }
            if(match(l, '=')){
                return set_token(l, TOK_PLUS_ASSIGN, tok_line, tok_col);
            }
            return set_token(l, TOK_PLUS, tok_line, tok_col);
        case '-':
            if(match(l, '-')){
                return set_token(l, TOK_DEC, tok_line, tok_col);
            }
            if(match(l, '=')){
                return set_token(l, TOK_MINUS_ASSIGN, tok_line, tok_col);
            }
            if(match(l, '>')){
                return set_token(l, TOK_ARROW, tok_line, tok_col);
            }
            return set_token(l, TOK_MINUS, tok_line, tok_col);
        case '*':
            if(match(l, '=')){
                return set_token(l, TOK_STAR_ASSIGN, tok_line, tok_col);
            }
            return set_token(l, TOK_STAR, tok_line, tok_col);
        case '/':
            if(match(l, '=')){
                return set_token(l, TOK_SLASH_ASSIGN, tok_line, tok_col);
            }
            return set_token(l, TOK_SLASH, tok_line, tok_col);
        case '%':
            if(match(l, '=')){
                return set_token(l, TOK_MOD_ASSIGN, tok_line, tok_col);
            }
            return set_token(l, TOK_MOD, tok_line, tok_col);
        case '!':
            if(match(l, '=')){
                return set_token(l, TOK_NOT_EQUAL, tok_line, tok_col);
            }
            return set_token(l, TOK_LOGICAL_NOT, tok_line, tok_col);
        case '&':
            if(match(l, '&')){
                return set_token(l, TOK_LOGICAL_AND, tok_line, tok_col);
            }
            if(match(l, '=')){
                return set_token(l, TOK_AND_ASSIGN, tok_line, tok_col);
            }
            return set_token(l, TOK_BIT_AND, tok_line, tok_col);
        case '|':
            if(match(l, '|')){
                return set_token(l, TOK_LOGICAL_OR, tok_line, tok_col);
            }
            if(match(l, '=')){
                return set_token(l, TOK_OR_ASSIGN, tok_line, tok_col);
            }
            return set_token(l, TOK_BIT_OR, tok_line, tok_col);
        case '^':
            if(match(l, '=')){
                return set_token(l, TOK_XOR_ASSIGN, tok_line, tok_col);
            }
            return set_token(l, TOK_BIT_XOR, tok_line, tok_col);
        case '=':
            if(match(l, '=')){
                return set_token(l, TOK_EQUAL_EQUAL, tok_line, tok_col);
            }
            return set_token(l, TOK_ASSIGN, tok_line, tok_col);
        case '<':
            if(match(l, '<')){
                if(match(l, '=')){
                    return set_token(l, TOK_SHL_ASSIGN, tok_line, tok_col);
                }
                return set_token(l, TOK_SHIFT_LEFT, tok_line, tok_col);
            }
            if(match(l, '=')){
                return set_token(l, TOK_LESS_EQUAL, tok_line, tok_col);
            }
            return set_token(l, TOK_LESS, tok_line, tok_col);
        case '>':
            if(match(l, '>')){
                if(match(l, '=')){
                    return set_token(l, TOK_SHR_ASSIGN, tok_line, tok_col);
                }
                return set_token(l, TOK_SHIFT_RIGHT, tok_line, tok_col);
            }
            if(match(l, '=')){
                return set_token(l, TOK_GREATER_EQUAL, tok_line, tok_col);
            }
            return set_token(l, TOK_GREATER, tok_line, tok_col);
    }
    return set_token(l, TOK_ERR, tok_line, tok_col);
}