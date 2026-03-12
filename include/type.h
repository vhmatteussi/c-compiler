#ifndef COMPILER_TYPE_H
#define COMPILER_TYPE_H

// algumas funções da ctype, porém sem usar a técnica de bitmasks que a ctype.h usa

#define MAX_NUM 10
#define MAX_CHAR 26

static inline int is_digit(unsigned char c){
    return (unsigned char)(c - '0') < MAX_NUM;
}

static inline int is_lower(unsigned char c){
    return (unsigned char)(c - 'a') < MAX_CHAR;
}

static inline int is_upper(unsigned char c){
    return (unsigned char)(c - 'A') < MAX_CHAR;
}

static inline int is_alpha(unsigned char c){
    return is_lower(c) || is_upper(c) || c == '_'; // essa função só vai ser usada para tokenizar identifiers então n tem problema incluir _
}

static inline int is_space(unsigned char c){
    return (unsigned char)(c - 1) < ' '; // todo o resto antes do espaço é lixo, menos o \0. Então se o c for 0x00 da um underflow e vira 255
}

#endif