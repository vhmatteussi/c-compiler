#include "include/token.h"

Token* init_token(unsigned char* value, TokenType type, unsigned short line, unsigned short col){
    Token *token = (Token*)calloc(1, sizeof(Token));
    token->lex = init_str(value);
    token->type = type;
    token->line = line;
    token->col = col;

    return token;
}