#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#include <stddef.h>

#define TOKEN_LIST \
        X(TOK_EOF) \
        X(TOK_ERR) \
        X(TOK_HASH) \
        X(TOK_HASH_HASH) \
        X(TOK_ID) \
        X(TOK_KW_CHAR) \
        X(TOK_KW_INT) \
        X(TOK_KW_SHORT) \
        X(TOK_KW_LONG) \
        X(TOK_KW_FLOAT) \
        X(TOK_KW_DOUBLE) \
        X(TOK_KW_VOID) \
        X(TOK_KW_SIGNED) \
        X(TOK_KW_UNSIGNED) \
        X(TOK_KW_STATIC) \
        X(TOK_KW_EXTERN) \
        X(TOK_KW_AUTO) \
        X(TOK_KW_VOLATILE) \
        X(TOK_KW_REGISTER) \
        X(TOK_KW_GOTO) \
        X(TOK_KW_IF) \
        X(TOK_KW_ELSE) \
        X(TOK_KW_SWITCH) \
        X(TOK_KW_CASE) \
        X(TOK_KW_DEFAULT) \
        X(TOK_KW_BREAK) \
        X(TOK_KW_CONTINUE) \
        X(TOK_KW_DO) \
        X(TOK_KW_WHILE) \
        X(TOK_KW_FOR) \
        X(TOK_KW_RETURN) \
        X(TOK_KW_ENUM) \
        X(TOK_KW_STRUCT) \
        X(TOK_KW_UNION) \
        X(TOK_KW_TYPEDEF) \
        X(TOK_KW_SIZEOF) \
        X(TOK_KW_INLINE) \
        X(TOK_KW_RESTRICT) \
        X(TOK_KW_BOOL) \
        X(TOK_KW_COMPLEX) \
        X(TOK_KW_PRAGMA) \
        X(TOK_NUM_INT) \
        X(TOK_NUM_FLOAT) \
        X(TOK_LIT_CHAR) \
        X(TOK_LIT_STRING) \
        X(TOK_LPAREN) \
        X(TOK_RPAREN) \
        X(TOK_LBRACKET) \
        X(TOK_RBRACKET) \
        X(TOK_LBRACE) \
        X(TOK_RBRACE) \
        X(TOK_SEMICOLON) \
        X(TOK_COMMA) \
        X(TOK_DOT) \
        X(TOK_ARROW) \
        X(TOK_QUESTION) \
        X(TOK_COLON) \
        X(TOK_ELLIPSIS) \
        X(TOK_PLUS) \
        X(TOK_MINUS) \
        X(TOK_STAR) \
        X(TOK_SLASH) \
        X(TOK_MOD) \
        X(TOK_INC) \
        X(TOK_DEC) \
        X(TOK_EQUAL_EQUAL) \
        X(TOK_NOT_EQUAL) \
        X(TOK_LESS) \
        X(TOK_GREATER) \
        X(TOK_LESS_EQUAL) \
        X(TOK_GREATER_EQUAL) \
        X(TOK_LOGICAL_AND) \
        X(TOK_LOGICAL_OR) \
        X(TOK_LOGICAL_NOT) \
        X(TOK_BIT_AND) \
        X(TOK_BIT_OR) \
        X(TOK_BIT_XOR) \
        X(TOK_BIT_NOT) \
        X(TOK_SHIFT_LEFT) \
        X(TOK_SHIFT_RIGHT) \
        X(TOK_ASSIGN) \
        X(TOK_PLUS_ASSIGN) \
        X(TOK_MINUS_ASSIGN) \
        X(TOK_STAR_ASSIGN) \
        X(TOK_SLASH_ASSIGN) \
        X(TOK_MOD_ASSIGN) \
        X(TOK_SHL_ASSIGN) \
        X(TOK_SHR_ASSIGN) \
        X(TOK_AND_ASSIGN) \
        X(TOK_XOR_ASSIGN) \
        X(TOK_OR_ASSIGN)

typedef struct String String;

typedef enum TokenType{
    #define X(name) name,
        TOKEN_LIST
    #undef X
} TokenType;

typedef struct Token{
    TokenType type;
    String *lex;
    size_t line;
    size_t col;
} Token;

#endif