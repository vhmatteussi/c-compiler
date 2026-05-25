#include "type.h"

bool is_digit(unsigned char c){
    return ((unsigned char)(c - '0') < _MAX_NUM);
}

bool is_lower(unsigned char c){
    return ((unsigned char)(c - 'a') < _MAX_CHAR);
}

bool is_upper(unsigned char c){
    return ((unsigned char)(c - 'A') < _MAX_CHAR);
}

bool is_alpha(unsigned char c){
    return (is_lower(c) || is_upper(c) || c == '_');
}

bool is_alnum(unsigned char c){
    return (is_digit(c) || is_alpha(c));
}

bool is_alpha_hex(unsigned char c){
    return (((unsigned char)(c - 'a') < _MAX_HEX_CHAR) || ((unsigned char)(c - 'A') < _MAX_HEX_CHAR));
}

bool is_hex(unsigned char c){
    return (is_alpha_hex(c) || is_digit(c));
}

bool is_octal(unsigned char c){
    return ((unsigned char)(c - '0') < _MAX_OCTAL_NUM);
}

bool is_in_escape_list(unsigned char c){
    switch(c){
        case '"':
        case '\'':
        case '?':
        case '\\':
        case 'a':
        case 'b':
        case 'f':
        case 'n':
        case 'r':
        case 't':
        case 'v':
            return true;
            break;
        default:
            return false;
    }
}

bool is_whitespace(unsigned char c){
    // basically, it undeflows 0x00 to 0xff, so \0 isn't considered an whitespace
    return ((unsigned char)(c - 1) < ' ');
}