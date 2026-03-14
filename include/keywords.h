#ifndef COMPILER_KEYWORDS_H
#define COMPILER_KEYWORDS_H

#include "token.h"
#define KW_HASH_CAP 64 //cap maior do que a quantidade de kw do C para evitar colisões na tabela hash

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

#endif