#include "lexer.h"

Lexer *init_lexer(Arena *a, const unsigned char *src){
    Lexer *l = (Lexer*)arena_malloc(a, sizeof(Lexer));
    l->src = src;
    l->start = 0;
    l->forward = 0;
    l->line = 0;
    l->col = 0;
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
        l->col = 0;
    }
    else{
        l->col++;
    }
}

void skip_whitespace(Lexer *l){
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
                if(peek(l) != '\0'){
                    advance(l);
                    advance(l);
                }
            }
            else{
                return;
            }
        }
        else{
            return;
        }
    }
}

Token* set_token(Lexer *l, TokenType type){
    uint32_t len = l->forward - l->start;
    const char *str = (const char*)&l->src[l->start];
    String *lex_str = intern_string(l->arena, &l->interner, str, len);

    Token *token = (Token*)arena_malloc(l->arena, sizeof(Token));
    token->lex = lex_str;
    token->type = type;
    token->line = l->line;
    token->col = l->col;
    
    return token;
}

Token* lex_literals(Lexer *l, unsigned char quote, TokenType type){
    while(peek(l) != quote && peek(l) != '\0'){
        if(peek(l) == '\\'){
            advance(l);

            // Hex escapes
            if(peek(l) == 'x'){
                advance(l);

                if(!is_hex(peek(l))){
                    return set_token(l, TOK_ERR);
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
                        return set_token(l, TOK_ERR);
                    }
                    advance(l);
                }
            }
            // Octal escapes
            else if(is_octal(peek(l))){
                uint32_t count = 0;
                while(is_octal(peek(l)) && count < 3){
                    advance(l);
                }
            }
            // Common escapes
            else{
                if(!is_in_escape_list(peek(l))){
                    return set_token(l, TOK_ERR);
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
    return set_token(l, type);
}

Token* next_token(Lexer *l){
    skip_whitespace(l);

    l->start = l->forward;
    unsigned char c = peek(l);

    // EOF
    if(c == '\0'){
        return set_token(l, TOK_EOF);
    }

    advance(l);

    // Literals
    if(c == '"' || ((c == 'L') && peek(l) == '"')){
        // String literal
        if(c == 'L'){
            advance(l);
        }
        return lex_literals(l, '"', TOK_LIT_STRING);
    }
    
    if(c == '\'' || ((c == 'L') && peek(l) == '\'')){
        // Char literal
        if(c == 'L'){
            advance(l);
        }
        return lex_literals(l, '\'', TOK_LIT_CHAR);
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
                            return set_token(l, TOK_ERR);
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
        token->line = l->line;
        token->col = l->col;
        
        return token;
    }

    // Numerals
    if(is_digit(c) || (c == '.' && is_digit(peek(l)))){
        uint32_t is_float = 0;

        // Float sem parte inteira, eg: .5f, .1e-2
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

                while(is_digit(peek(l))){
                    advance(l);
                }
            }
        }
        // Hexadecimal
        else if(c == '0' && (peek(l) == 'x' || peek(l) == 'X')){
            advance(l);

            while(is_hex(peek(l))){
                advance(l);
            }

            // Float hexs
            if(peek(l) == '.'){
                is_float = 1;
                advance(l);
                while(is_hex(peek(l))){
                    advance(l);
                }
            }
            
            if(peek(l) == 'p' || peek(l) == 'P'){
                is_float = 1;
                advance(l);

                if(peek(l) == '+' || peek(l) == '-'){
                    advance(l);
                }

                while(is_digit(peek(l))){
                    advance(l);
                }
            }
        }
        else{
            while(is_digit(peek(l))){
                advance(l);
            }

            // Decimal floats, eg: 1.0, 23.9, 0.3
            if(peek(l) == '.'){
                is_float = 1;
                advance(l);

                while(is_digit(peek(l))){
                    advance(l);
                }
            }

            // Exponential "integer" (float), eg: 2e8, 10e-5, 3e+2
            if(peek(l) == 'e' || peek(l) == 'E'){
                is_float = 1;
                advance(l);

                if(peek(l) == '-' || peek(l) == '+'){
                    advance(l);
                }

                while(is_digit(peek(l))){
                    advance(l);
                }
            }
        }

        // While é melhor, outra parte do compilador tem que lidar com 23ffffff ou 23.233ULL
        while(peek(l) == 'f' || peek(l) == 'F' || peek(l) == 'l' || peek(l) == 'L' || peek(l) == 'u' || peek(l) == 'U'){
            advance(l);
        }

        if(is_float){
            return set_token(l, TOK_NUM_FLOAT);
        }
        return set_token(l, TOK_NUM_INT);
    }
    
    // Operators, Separators e Bigraphs
    switch(c){
        case '#':
            if(match(l, '#')){
                return set_token(l, TOK_HASH_HASH);
            } 
            return set_token(l, TOK_HASH);
        case '(':
            return set_token(l, TOK_LPAREN);
        case ')':
            return set_token(l, TOK_RPAREN);
        case '{':
            return set_token(l, TOK_LBRACE);
        case '}':
            return set_token(l, TOK_RBRACE);
        case '[':
            return set_token(l, TOK_LBRACKET);
        case ']':
            return set_token(l, TOK_RBRACKET);
        case ';':
            return set_token(l, TOK_SEMICOLON);
        case ',':
            return set_token(l, TOK_COMMA);
        case '?':
            return set_token(l, TOK_QUESTION);
        case ':':
            return set_token(l, TOK_COLON);
        case '~':
            return set_token(l, TOK_BIT_NOT);
        case '.':
            if(match(l, '.')){
                if(match(l, '.')){
                    return set_token(l, TOK_ELLIPSIS);
                }
                return set_token(l, TOK_ERR); // Talvez seja melhor deixar o parser lidar com isso
            }
            return set_token(l, TOK_DOT);

        case '+':
            if(match(l, '+')){
                return set_token(l, TOK_INC);
            }
            if(match(l, '=')){
                return set_token(l, TOK_PLUS_ASSIGN);
            }
            return set_token(l, TOK_PLUS);

        case '-':
            if(match(l, '-')){
                return set_token(l, TOK_DEC);
            }
            if(match(l, '=')){
                return set_token(l, TOK_MINUS_ASSIGN);
            }
            if(match(l, '>')){
                return set_token(l, TOK_ARROW);
            }
            return set_token(l, TOK_MINUS);

        case '*':
            if(match(l, '=')){
                return set_token(l, TOK_STAR_ASSIGN);
            }
            return set_token(l, TOK_STAR);

        case '/':
            if(match(l, '=')){
                return set_token(l, TOK_SLASH_ASSIGN);
            }
            return set_token(l, TOK_SLASH);
        
        case '%':
            if(match(l, '=')){
                return set_token(l, TOK_MOD_ASSIGN);
            }
            if(match(l, ':')){
                if(peek(l) == '%'){
                    if(peek_next(l) == ':'){
                        advance(l);
                        advance(l);
                        return set_token(l, TOK_HASH_HASH);
                    }
                    // Vou deixar isso para o parser
                }
                return set_token(l, TOK_HASH);
            }
            return set_token(l, TOK_MOD);

        case '!':
            if(match(l, '=')){
                return set_token(l, TOK_NOT_EQUAL);
            }
            return set_token(l, TOK_LOGICAL_NOT);

        case '&':
            if(match(l, '&')){
                return set_token(l, TOK_LOGICAL_AND);
            }
            if(match(l, '=')){
                return set_token(l, TOK_AND_ASSIGN);
            }
            return set_token(l, TOK_BIT_AND);

        case '|':
            if(match(l, '|')){
                return set_token(l, TOK_LOGICAL_OR);
            }
            if(match(l, '=')){
                return set_token(l, TOK_OR_ASSIGN);
            }
            return set_token(l, TOK_BIT_OR);

        case '^':
            if(match(l, '=')){
                return set_token(l, TOK_XOR_ASSIGN);
            }
            return set_token(l, TOK_BIT_XOR);

        case '=':
            if(match(l, '=')){
                return set_token(l, TOK_EQUAL_EQUAL);
            }
            return set_token(l, TOK_ASSIGN);

        case '<':
            if(match(l, '<')){
                if(match(l, '=')){
                    return set_token(l, TOK_SHL_ASSIGN);
                }
                return set_token(l, TOK_SHIFT_LEFT);
            }
            if(match(l, '=')){
                return set_token(l, TOK_LESS_EQUAL);
            }
            if(match(l, ':')){
                return set_token(l, TOK_LBRACKET);
            }
            if(match(l, '%')){
                return set_token(l, TOK_LBRACE);
            }
            return set_token(l, TOK_LESS);
        
        case '>':
            if(match(l, '>')){
                if(match(l, '=')){
                    return set_token(l, TOK_SHR_ASSIGN);
                }
                return set_token(l, TOK_SHIFT_RIGHT);
            }
            if(match(l, '=')){
                return set_token(l, TOK_GREATER_EQUAL);
            }
            if(match(l, ':')){
                return set_token(l, TOK_RBRACKET);
            }
            if(match(l, '%')){
                return set_token(l, TOK_RBRACE);
            }
            return set_token(l, TOK_GREATER);
    }

    return set_token(l, TOK_ERR);
}