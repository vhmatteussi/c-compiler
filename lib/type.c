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

static const uint64_t escape_mask[4] = {
    (UINT64_C(1) << '"')       | (UINT64_C(1) << '\'')     | (UINT64_C(1) << '?'),                                      // 0 - 63
    (UINT64_C(1) << ('\\'-64)) | (UINT64_C(1) << ('a'-64)) | (UINT64_C(1) << ('b'-64)) | (UINT64_C(1) << ('f'-64)) |    // 64 - 127
    (UINT64_C(1) << ('n'-64))  | (UINT64_C(1) << ('r'-64)) | (UINT64_C(1) << ('t'-64)) | (UINT64_C(1) << ('v'-64)),
    UINT64_C(0),                                                                                                        // 128 - 191
    UINT64_C(0)                                                                                                         // 192 - 255
};

bool is_in_escape_list(unsigned char c){
    // will not write down how it works, look at my fun little drawing instead:
    // https://excalidraw.com/#json=Os8zNbPbNxhyE6jIt16mM,ZMHJHF0ysKZ6McKMvRuiyA
    return ((escape_mask[c >> 6] >> (c & 63)) & 1);
}

bool is_whitespace(unsigned char c){
    // basically, it undeflows 0x00 to 0xff, so \0 isn't considered a/an (english is hard) whitespace
    return ((unsigned char)(c - 1) < ' ');
}