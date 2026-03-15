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
    return l->src[l->forward + sizeof(char)];
}

int match(Lexer *l, unsigned char to_match){
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
    String *lex_str = intern_string(l->arena, &l->interner, (const char*)&l->src[l->start], len);

    Token *token = (Token*)arena_malloc(l->arena, sizeof(Token));
    token->lex = lex_str;
    token->type = type;
    token->line = l->line;
    token->col = l->col;
    
    return token;
}

Token* next_token(Lexer *l){
    skip_whitespace(l);

    l->start = l->forward;
    unsigned char c = peek(l);

    if(c == '\0'){
        return set_token(l, TOK_EOF);
    }

    advance(l);

    if(is_alpha(c)){
        while(is_alpha(peek(l)) || is_digit(peek(l))){
            advance(l);
        }
        uint32_t len = l->forward - l->start;
        String *lex_str = intern_string(l->arena, &l->interner, (const char*)&l->src[l->start], len);
        
        Token *token = (Token*)arena_malloc(l->arena, sizeof(Token));
        token->lex = lex_str;
        token->type = lex_str->kw_type; 
        token->line = l->line;
        token->col = l->col;
        
        return token;
    }

    if(is_digit(c) || (c == '.' && is_digit(peek(l)))){
        int is_float = 0;

        if(c == '.'){
            is_float = 1;
            
            while(is_digit(peek(l))){
                advance(l);
            }

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
        else if(c == '0' && (peek(l) == 'x' || peek(l) == 'X')){
            advance(l);

            while(is_hex(peek(l))){
                advance(l);
            }

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

            if(peek(l) == '.'){
                is_float = 1;
                advance(l);

                while(is_digit(peek(l))){
                    advance(l);
                }
            }

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

        while(peek(l) == 'f' || peek(l) == 'F' || peek(l) == 'l' || peek(l) == 'L' || peek(l) == 'u' || peek(l) == 'U'){
            advance(l);
        }

        if(is_float){
            return set_token(l, TOK_NUM_FLOAT);
        }
        return set_token(l, TOK_NUM_INT);
    }

    if(c == '"'){
        while(peek(l) != '"' && peek(l) != '\0'){
            if(peek(l) == '\\'){
                advance(l);
            }
            advance(l);
        }
        if(peek(l) == '"'){
            advance(l);
        }
        return set_token(l, TOK_LIT_STRING);
    }

    if(c == '\''){
        while(peek(l) != '\'' && peek(l) != '\0'){
            if(peek(l) == '\\'){
                advance(l);
            }
            advance(l);
        }
        if(peek(l) == '\''){
            advance(l);
        }
        return set_token(l, TOK_LIT_CHAR);
    }

    // esse switch funciona para todos os operadores e separadores
    switch(c){
        case '(': return set_token(l, TOK_LPAREN);
        case ')': return set_token(l, TOK_RPAREN);
        case '{': return set_token(l, TOK_LBRACE);
        case '}': return set_token(l, TOK_RBRACE);
        case '[': return set_token(l, TOK_LBRACKET);
        case ']': return set_token(l, TOK_RBRACKET);
        case ';': return set_token(l, TOK_SEMICOLON);
        case ',': return set_token(l, TOK_COMMA);
        case '?': return set_token(l, TOK_QUESTION);
        case ':': return set_token(l, TOK_COLON);
        case '~': return set_token(l, TOK_BIT_NOT);
        case '.': 
            if(peek(l) == '.' && peek_next(l) == '.'){
                advance(l);
                advance(l);
                return set_token(l, TOK_ELLIPSIS);
            }
            return set_token(l, TOK_DOT);

        case '+':
            if(match(l, '+')) return set_token(l, TOK_INC);
            if(match(l, '=')) return set_token(l, TOK_PLUS_ASSIGN);
            return set_token(l, TOK_PLUS);

        case '-':
            if(match(l, '-')) return set_token(l, TOK_DEC);
            if(match(l, '=')) return set_token(l, TOK_MINUS_ASSIGN);
            if(match(l, '>')) return set_token(l, TOK_ARROW);
            return set_token(l, TOK_MINUS);

        case '*':
            if(match(l, '=')) return set_token(l, TOK_STAR_ASSIGN);
            return set_token(l, TOK_STAR);

        case '/':
            if(match(l, '=')) return set_token(l, TOK_SLASH_ASSIGN);
            return set_token(l, TOK_SLASH);
        
        case '%':
            if(match(l, '=')) return set_token(l, TOK_MOD_ASSIGN);
            return set_token(l, TOK_MOD);

        case '!':
            if(match(l, '=')) return set_token(l, TOK_NOT_EQUAL);
            return set_token(l, TOK_LOGICAL_NOT);

        case '&':
            if(match(l, '&')) return set_token(l, TOK_LOGICAL_AND);
            if(match(l, '=')) return set_token(l, TOK_AND_ASSIGN);
            return set_token(l, TOK_BIT_AND);

        case '|':
            if(match(l, '|')) return set_token(l, TOK_LOGICAL_OR);
            if(match(l, '=')) return set_token(l, TOK_OR_ASSIGN);
            return set_token(l, TOK_BIT_OR);

        case '^':
            if(match(l, '=')) return set_token(l, TOK_XOR_ASSIGN);
            return set_token(l, TOK_BIT_XOR);

        case '=':
            if(match(l, '=')) return set_token(l, TOK_EQUAL_EQUAL);
            return set_token(l, TOK_ASSIGN);

        case '<':
            if(match(l, '<')){
                if(match(l, '=')) return set_token(l, TOK_SHL_ASSIGN);
                return set_token(l, TOK_SHIFT_LEFT);
            }
            if(match(l, '=')) return set_token(l, TOK_LESS_EQUAL);
            return set_token(l, TOK_LESS);
        
        case '>':
            if(match(l, '>')){
                if(match(l, '=')) return set_token(l, TOK_SHR_ASSIGN);
                return set_token(l, TOK_SHIFT_RIGHT);
            }
            if(match(l, '=')) return set_token(l, TOK_GREATER_EQUAL);
            return set_token(l, TOK_GREATER);
    }

    return set_token(l, TOK_ERR);
}