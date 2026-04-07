#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include "lexer.h"

typedef struct Parser{
    Token *current_token;
    // ast, todo
} Parser;

bool accept_token(Token *type);

// idk what to do
#endif