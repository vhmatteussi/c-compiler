#include "lexer.h"

Lexer *init_lexer(Arena *a, const unsigned char *src){
    Lexer *l = (Lexer*)arena_malloc(a, sizeof(Lexer));
    l->src = src;
    l->start = 0;
    l->forward = 0;
    l->line = 1;
    l->col = 1;
    l->arena = a;
    init_interner(a, &l->interner);
    return l;
}

unsigned char peek(Lexer *l){
    return l->src[l->forward];
}

unsigned char peek_next(Lexer *l){
    if(peek(l) == '\0'){
        return '\0';
    }
    return l->src[l->forward + 1];
}

uint32_t match(Lexer *l, unsigned char to_match){
    if(peek(l) != to_match){
        return 0;
    }
    advance(l);
    return 1;
}

void advance(Lexer *l){
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

Token* set_token(Lexer *l, TokenType type, size_t line, size_t col){
    uint32_t len = l->forward - l->start;
    const char *str = (const char*)&l->src[l->start];
    String *lex_str = intern_string(l->arena, &l->interner, str, len);

    Token *token = (Token*)arena_malloc(l->arena, sizeof(Token));
    token->lex = lex_str;
    token->type = type;
    token->line = line;
    token->col = col;
    
    return token;
}

uint32_t skip_whitespace(Lexer *l){
    while(1){
        unsigned char c = peek(l);
        if(is_whitespace(c)){
            advance(l);
        }
        else if(c == '/'){
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
                    return 1;
                }
                advance(l);
                advance(l);
            }
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
    }
}

static inline Token* lex_literals(Lexer *l, unsigned char quote, TokenType type, size_t line, size_t col){
    while(peek(l) != quote && peek(l) != '\0'){
        if(peek(l) == '\\'){
            advance(l);

            // Hex escapes
            if(peek(l) == 'x'){
                advance(l);

                if(!is_hex(peek(l))){
                    return set_token(l, TOK_ERR, line, col);
                }

                while(is_hex(peek(l))){
                    advance(l);
                }
            }
            // Unicode escapes
            else if(peek(l) == 'u' || peek(l) == 'U'){
                uint32_t req_digits = (peek(l) == 'u') ? 4 : 8;
                advance(l);

                for(uint32_t i=0; i<req_digits; i++){
                    if(!is_hex(peek(l))){
                        return set_token(l, TOK_ERR, line, col);
                    }
                    advance(l);
                }
            }
            // Octal escapes
            else if(is_octal(peek(l))){
                uint32_t count = 0;
                while(is_octal(peek(l)) && count < 3){
                    advance(l);
                    count++;
                }
            }
            // Common escapes
            else{
                if(!is_in_escape_list(peek(l))){
                    advance(l);
                    return set_token(l, TOK_ERR, line, col);
                }
                advance(l);
            }
            // No escape
            continue;
        }
        advance(l);
    }

    if(peek(l) == quote){
        advance(l);
    }
    // Mais entendível do que retornar o token com um if
    return set_token(l, type, line, col);
}

Token* next_token(Lexer *l){
    if(skip_whitespace(l)){
        l->start = l->forward;
        return set_token(l, TOK_ERR, l->line, l->col);
    }

    l->start = l->forward;
    unsigned char c = peek(l);

    size_t tok_line = l->line;
    size_t tok_col = l->col;

    // EOF
    if(c == '\0'){
        return set_token(l, TOK_EOF, tok_line, tok_col);
    }

    advance(l);

    // Literals
    if(c == '"' || ((c == 'L') && peek(l) == '"')){
        // String literal
        if(c == 'L'){
            advance(l);
        }
        return lex_literals(l, '"', TOK_LIT_STRING, tok_line, tok_col);
    }
    
    if(c == '\'' || ((c == 'L') && peek(l) == '\'')){
        // Char literal
        if(c == 'L'){
            advance(l);
        }
        if(peek(l) == '\''){
            return set_token(l, TOK_ERR, tok_line, tok_col);
        }
        return lex_literals(l, '\'', TOK_LIT_CHAR, tok_line, tok_col);
    }

    // Identifiers e Keywords
    if(is_alpha(c)){
        while(1){
            if(is_alnum(peek(l))){
                advance(l);
            }
            // Unicode escape sequences
            else if(peek(l) == '\\'){
                if(peek_next(l) == 'u' || peek_next(l) == 'U'){
                    uint8_t req_digits = (peek_next(l) == 'u') ? 4 : 8;
                    advance(l);
                    advance(l);

                    for(uint8_t i=0; i<req_digits; i++){
                        if(!is_hex(peek(l))){
                            return set_token(l, TOK_ERR, tok_line, tok_col);
                        }
                        advance(l);
                    }
                }
                else{
                    break; // Deixa o erro para o próximo token
                }
            }
            else{
                break;
            }
        }

        uint32_t len = l->forward - l->start;
        const char *str = (const char*)&l->src[l->start];
        String *lex_str = intern_string(l->arena, &l->interner, str, len);
        
        Token *token = (Token*)arena_malloc(l->arena, sizeof(Token));
        token->lex = lex_str;
        token->type = lex_str->kw_type; 
        token->line = tok_line;
        token->col = tok_col;
        
        return token;
    }

    // Numerals
    if(is_digit(c) || (c == '.' && is_digit(peek(l)))){
        uint32_t is_float = 0;
        uint32_t hex_float_flag = 0;
        uint32_t exp_flag = 0;

        // Float sem parte inteira, e.g. .5f, .1e-2
        if(c == '.'){
            is_float = 1;
            
            while(is_digit(peek(l))){
                advance(l);
            }
            
            // Exponentials
            if(peek(l) == 'e' || peek(l) == 'E'){
                advance(l);

                if(peek(l) == '-' || peek(l) == '+'){
                    advance(l);
                }

                if(!is_digit(peek(l))){
                    return set_token(l, TOK_ERR, tok_line, tok_col);
                }

                while(is_digit(peek(l))){
                    advance(l);
                }
            }
        }
        // Hexadecimal
        else if(c == '0' && (peek(l) == 'x' || peek(l) == 'X')){
            advance(l);

            uint32_t has_hex_digits = 0;
            uint32_t has_fraction_digits = 0;
            uint32_t has_exp_digits = 0;
            uint32_t has_exp = 0;

            while(is_hex(peek(l))){
                advance(l);
                has_hex_digits = 1;
            }

            // Float hexs
            if(peek(l) == '.'){
                is_float = 1;
                advance(l);
                while(is_hex(peek(l))){
                    advance(l);
                    has_fraction_digits = 1;
                }
            }
            
            if(peek(l) == 'p' || peek(l) == 'P'){
                is_float = 1;
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
                    return set_token(l, TOK_ERR, tok_line, tok_col);
                }
            }

            if(is_float){
                if(!has_exp){
                    return set_token(l, TOK_ERR, tok_line, tok_col);
                }

                if(!has_hex_digits && !has_fraction_digits){
                    return set_token(l, TOK_ERR, tok_line, tok_col);
                }

                if(has_fraction_digits){
                    hex_float_flag = 1;
                }
            }

            if(!is_float && !has_hex_digits){
                return set_token(l, TOK_ERR, tok_line, tok_col);
            }
        }
        // Octals
        else if(c == '0' && is_octal(peek(l))){
            while(is_octal(peek(l))){
                advance(l);
            }

            if(is_digit(peek(l))){
                while(is_alnum(peek(l))){
                    advance(l);
                }
                return set_token(l, TOK_ERR, tok_line, tok_col);
            }
        }
        // Decimals
        else{
            while(is_digit(peek(l))){
                advance(l);
            }

            // Decimal floats, e.g. 1.0, 23.9, 0.3
            if(peek(l) == '.'){
                is_float = 1;
                advance(l);

                while(is_digit(peek(l))){
                    advance(l);
                }
            }

            // Exponential "integer" (float), e.g. 2e8, 10e-5, 3e+2
            if(peek(l) == 'e' || peek(l) == 'E'){
                is_float = 1;
                exp_flag = 1;
                advance(l);

                if(peek(l) == '-' || peek(l) == '+'){
                    advance(l);
                }

                if(!is_digit(peek(l))){
                    return set_token(l, TOK_ERR, tok_line, tok_col);
                }

                while(is_digit(peek(l))){
                    advance(l);
                }
            }
        }

        // Sufixos de float
        if(peek(l) == 'f' || peek(l) == 'F'){
            is_float = 1;
            advance(l);
        }
        else if(peek(l) == 'l' || peek(l) == 'L'){
            if(is_float && (hex_float_flag || exp_flag)){
                advance(l);
            }
            else if(peek_next(l) == 'f' || peek_next(l) == 'F'){
                is_float = 1;
                advance(l);
                advance(l);
            }
        }
        // Sufixos de int
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
        // Handle de erros de sufixo
        if(is_alnum(peek(l))){
            while(is_alnum(peek(l))){
                advance(l);
            }
            return set_token(l, TOK_ERR, tok_line, tok_col);
        }

        if(is_float){
            return set_token(l, TOK_NUM_FLOAT, tok_line, tok_col);
        }
        return set_token(l, TOK_NUM_INT, tok_line, tok_col);
    }
    
    // Operators e Separators
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