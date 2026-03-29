#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include "token.h"
#include "strings.h"
#include "type.h"

typedef struct Lexer{
    const unsigned char *src;
    size_t start;
    size_t forward;
    size_t line;
    size_t col;
    Arena *arena;
    StringIntern intern;
} Lexer;

Lexer *init_lexer(Arena *a, const unsigned char *src);
unsigned char peek(Lexer *l);
uint32_t match(Lexer *l, unsigned char to_match);

void advance(Lexer *l);
uint32_t skip_whitespace(Lexer *l);

//Token* lex_literals(Lexer *l, unsigned char quote, TokenType type, size_t line, size_t col);

Token* set_token(Lexer *l, TokenType type, size_t line, size_t col);
Token* next_token(Lexer *l);

#endif