#ifndef COMPILER_KEYWORDS_H
#define COMPILER_KEYWORDS_H

#include "token.h"
#define KW_HASH_CAP 64 //cap 2x maior do que a quantidade de kw do C para evitar colisões na tabela hash

typedef struct KeywordEnty{
    const char *key;
    TokenType type;
} KeywordEnty;

static KeywordEnty kw_table[KW_HASH_CAP];

// to usando o lose lose do kernighan e ritchie como placeholder pq eu não conheço algortimos bons no momento
static inline unsigned long hash_ll(const unsigned char *str, unsigned int lenght){
    unsigned long hash=0;
    
    for(unsigned int i=0; i<lenght; i++){
        hash += str[i];
    }

    return hash;
}

static inline void insert_kw(const char *key, TokenType type){
    unsigned int lenght = raw_strsize(key);
    unsigned long idx = hash_ll((const unsigned char*)key, lenght) % KW_HASH_CAP;

    while(kw_table[idx].key != NULL){
        idx = (idx + 1) % KW_HASH_CAP;
    }

    kw_table[idx].key = key;
    kw_table[idx].type = type;
}

static inline void init_kw(void){
    insert_kw("char", TOK_KW_CHAR);
    insert_kw("int", TOK_KW_INT);
    insert_kw("short", TOK_KW_SHORT);
    insert_kw("long", TOK_KW_LONG);
    insert_kw("float", TOK_KW_FLOAT);
    insert_kw("double", TOK_KW_DOUBLE);
    insert_kw("void", TOK_KW_VOID);
    insert_kw("bool", TOK_KW_BOOL);
    insert_kw("signed", TOK_KW_SIGNED);
    insert_kw("unsgined", TOK_KW_UNSIGNED);
    insert_kw("static", TOK_KW_STATIC);
    insert_kw("extern", TOK_KW_EXTERN);
    insert_kw("auto", TOK_KW_AUTO);
    insert_kw("register", TOK_KW_REGISTER);
    insert_kw("inline", TOK_KW_INLINE);
    insert_kw("restrict", TOK_KW_RESTRICT);
    insert_kw("goto", TOK_KW_GOTO);
    insert_kw("if", TOK_KW_IF);
    insert_kw("else", TOK_KW_ELSE);
    insert_kw("switch", TOK_KW_SWITCH);
    insert_kw("case", TOK_KW_CASE);
    insert_kw("default", TOK_KW_DEFAULT);
    insert_kw("break", TOK_KW_BREAK);
    insert_kw("continue", TOK_KW_CONTINUE);
    insert_kw("do", TOK_KW_DO);
    insert_kw("while", TOK_KW_WHILE);
    insert_kw("for", TOK_KW_FOR);
    insert_kw("return", TOK_KW_RETURN);
    insert_kw("enum", TOK_KW_ENUM);
    insert_kw("struct", TOK_KW_STRUCT);
    insert_kw("union", TOK_KW_UNION);
    insert_kw("typedef", TOK_KW_TYPEDEF);
    insert_kw("sizeof", TOK_KW_SIZEOF);
}

static inline TokenType kw_lookup(const unsigned char *str, unsigned int lenght){
    unsigned long idx = hash_ll(str, lenght) % KW_HASH_CAP;
    
    while(kw_table[idx].key != NULL){
        unsigned int kw_len = raw_strsize(kw_table[idx].key);
        if(kw_len == lenght){
            int match = 1;
            // criar uma rawstr_cmp ou usar o meu tipo string mais tarde
            for(unsigned int i=0; i<lenght; i++){
                if(kw_table[idx].key[i] != str[i]){
                    match = 0;
                    break;
                }
            }
            if(match){
                return kw_table[idx].type;
            }
        }
        idx = (idx + 1) % KW_HASH_CAP;
    }
    return TOK_ID;
}

#endif