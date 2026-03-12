#include "include/lexer.h"

Lexer *init_lexer(const unsigned char *src){
    Lexer *l = (Lexer*)malloc(sizeof(Lexer));
    l->src = src;
    l->start = 0;
    l->forward = 0;
    l->line = 0;
    l->col = 0;
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

    l->forward++;
    unsigned char c = l->src[l->forward];

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
        if(is_space(c)){
            advance(l);
        }
        else if(c == '/'){
            if(peek_next(l) == '/'){
                while(peek(l) != '/n' && peek(l) != '\0'){
                    advance(l);
                }
            }
            if(peek_next(l) == '*'){
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
    unsigned int lenght = l->forward - l->start;
    unsigned char *buffer = (unsigned char*)malloc(lenght + sizeof(char));
    for(unsigned int i=0; i<lenght; i++){
        buffer[i] = l->src[l->start + i];
    }
    buffer[lenght] = '\0';
    return init_token(buffer, type, l->line, l->col);
}

Token* next_token(Lexer *l){
    skip_whitespace(l);

    l->start = l->forward;
    unsigned char c = peek(l);

    if(c == '\0'){
        return set_token(l, TOK_EOF);
    }
}