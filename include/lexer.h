#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include "token.h"
#include "type.h"

typedef struct Lexer{
    const unsigned char *src;
    size_t start;
    size_t forward;
    size_t line;
    size_t col;
} Lexer;

Lexer *init_lexer(const unsigned char *src);
unsigned char peek(Lexer *l);
int match(Lexer *l, unsigned char to_match);

void advance(Lexer *l);
void skip_whitespace(Lexer *l);

Token* set_token(Lexer *l, TokenType type);
Token* next_token(Lexer *l);

#endif