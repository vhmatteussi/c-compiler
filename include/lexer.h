#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

typedef struct Lexer{
    const unsigned char *src;
    unsigned short start;
    unsigned short forward;
    unsigned short line;
    unsigned short col;
} Lexer;

Lexer *init_lexer(const unsigned char *src);
unsigned char peek(Lexer *l);
unsigned char advance(Lexer *l);

#endif