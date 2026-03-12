#include "include/lexer.h"

unsigned char peek(Lexer *l){
    return l->src[l->forward];
}

unsigned char advance(Lexer *l){
    l->forward++;
    int c = l->src[l->forward];
    
    l->col++;
    if(c == 0x4A){
        l->line++;
        l->col = 0;
    }
}