#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include "token.h"
#include "strings.h"
#include "type.h"

// workaround absurdo pq fiquei com preguiça de mudar o set_token. Outra hora eu mudo a função
typedef struct StartPointer{
    size_t pointer;
    size_t line;
    size_t col;
} StartPointer;

typedef struct Lexer{
    const unsigned char *src;
    StartPointer start;
    size_t forward;
    size_t line;
    size_t col;
    Arena *arena;
    StringInterner interner;
} Lexer;

Lexer *init_lexer(Arena *a, const unsigned char *src);
unsigned char peek(Lexer *l);
uint32_t match(Lexer *l, unsigned char to_match);

void advance(Lexer *l);
void skip_whitespace(Lexer *l);

Token* lex_literals(Lexer *l, unsigned char quote, TokenType type);

Token* set_token(Lexer *l, TokenType type);
Token* next_token(Lexer *l);

#endif