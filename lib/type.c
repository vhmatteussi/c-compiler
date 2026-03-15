#include "type.h"

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

uint32_t is_alpha_hex(unsigned char c){
    return ((unsigned char)(c - 'a') < MAX_HEX_CHAR || (unsigned char)(c - 'A') < MAX_HEX_CHAR);
}

uint32_t is_hex(unsigned char c){
    return is_alpha_hex(c) || is_digit(c);
}

uint32_t is_octal(unsigned char c){
    return (unsigned char)(c - '0') < MAX_OCTAL_CHAR;
}

// comment of the year:
// https://excalidraw.com/#json=Os8zNbPbNxhyE6jIt16mM,ZMHJHF0ysKZ6McKMvRuiyA
static const uint64_t escape_mask[4] = {
    (1ULL<<'"')       | (1ULL<<'\'')     | (1ULL<<'?'),                             // 0 - 63
    (1ULL<<('\\'-64)) | (1ULL<<('a'-64)) | (1ULL<<('b'-64)) | (1ULL<<('f'-64)) |    // 64 - 127
    (1ULL<<('n'-64))  | (1ULL<<('r'-64)) | (1ULL<<('t'-64)) | (1ULL<<('v'-64)),
    0ULL,                                                                           // 128 - 191
    0ULL                                                                            // 192 - 255
};

uint32_t is_in_escape_list(unsigned char c){
    return (escape_mask[c >> 6] >> (c & 63)) & 1;
}

uint32_t is_whitespace(unsigned char c){
    // Todo o resto antes do espaço é lixo, menos o \0. Então se o c for 0x00 da um underflow e vira 0xFF
    return (unsigned char)(c - 1) < ' ';
}