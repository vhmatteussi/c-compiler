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
Token *next_token(Lexer *l);

#endif