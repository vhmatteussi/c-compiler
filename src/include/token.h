#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#include <stddef.h>

// C be like:
typedef struct String String;

typedef enum TokenType{
    // Special
    TOK_EOF,
    TOK_ERR,
    TOK_HASH,
    TOK_HASH_HASH,
    // Identifier
    TOK_ID,
    // Keywords
    TOK_KW_CHAR,
    TOK_KW_INT,
    TOK_KW_SHORT,
    TOK_KW_LONG,
    TOK_KW_FLOAT,
    TOK_KW_DOUBLE,
    TOK_KW_VOID,
    TOK_KW_SIGNED,
    TOK_KW_UNSIGNED,
    TOK_KW_STATIC,
    TOK_KW_EXTERN,
    TOK_KW_AUTO,
    TOK_KW_VOLATILE,
    TOK_KW_REGISTER,
    TOK_KW_GOTO,
    TOK_KW_IF,
    TOK_KW_ELSE,
    TOK_KW_SWITCH,
    TOK_KW_CASE,
    TOK_KW_DEFAULT,
    TOK_KW_BREAK,
    TOK_KW_CONTINUE,
    TOK_KW_DO,
    TOK_KW_WHILE,
    TOK_KW_FOR,
    TOK_KW_RETURN,
    TOK_KW_ENUM,
    TOK_KW_STRUCT,
    TOK_KW_UNION,
    TOK_KW_TYPEDEF,
    TOK_KW_SIZEOF,
    TOK_KW_INLINE,
    TOK_KW_RESTRICT,
    TOK_KW_BOOL,
    TOK_KW_COMPLEX,
    TOK_KW_IMAGINARY,
    // Numbers
    TOK_NUM_INT,
    TOK_NUM_FLOAT,
    // Literals
    TOK_LIT_CHAR,
    TOK_LIT_STRING,
    // Punctuators
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACKET,
    TOK_RBRACKET,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_SEMICOLON,
    TOK_COMMA,
    TOK_DOT,
    TOK_ARROW,
    TOK_QUESTION,
    TOK_COLON,
    TOK_ELLIPSIS,
    // Operators
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_MOD,
    TOK_INC,
    TOK_DEC,
    TOK_EQUAL_EQUAL,
    TOK_NOT_EQUAL,
    TOK_LESS,
    TOK_GREATER,
    TOK_LESS_EQUAL,
    TOK_GREATER_EQUAL,
    TOK_LOGICAL_AND,
    TOK_LOGICAL_OR,
    TOK_LOGICAL_NOT,
    TOK_BIT_AND,
    TOK_BIT_OR,
    TOK_BIT_XOR,
    TOK_BIT_NOT,
    TOK_SHIFT_LEFT,
    TOK_SHIFT_RIGHT,
    TOK_ASSIGN,
    TOK_PLUS_ASSIGN,
    TOK_MINUS_ASSIGN,
    TOK_STAR_ASSIGN,
    TOK_SLASH_ASSIGN,
    TOK_MOD_ASSIGN,
    TOK_SHL_ASSIGN,
    TOK_SHR_ASSIGN,
    TOK_AND_ASSIGN,
    TOK_XOR_ASSIGN,
    TOK_OR_ASSIGN,
} TokenType;

typedef struct Token{
    TokenType type;
    String *lex;
    size_t line;
    size_t col;
} Token;

#endif