#include "include/type.h"

uint32_t is_digit(unsigned char c){
    return (unsigned char)(c - '0') < MAX_NUM;
}

uint32_t is_lower(unsigned char c){
    return (unsigned char)(c - 'a') < MAX_CHAR;
}

uint32_t is_upper(unsigned char c){
    return (unsigned char)(c - 'A') < MAX_CHAR;
}

uint32_t is_alpha(unsigned char c){
    return is_lower(c) || is_upper(c) || c == '_';
}

uint32_t is_whitespace(unsigned char c){
    return (unsigned char)(c - 1) < ' '; // todo o resto antes do espaço é lixo, menos o \0. Então se o c for 0x00 da um underflow e vira 255
}