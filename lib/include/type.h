#ifndef COMPILER_TYPE_H
#define COMPILER_TYPE_H

#include <stdbool.h>
#include <stdint.h>

#define _MAX_NUM 10
#define _MAX_CHAR 26
#define _MAX_HEX_CHAR 8
#define _MAX_OCTAL_NUM 8

bool is_digit(unsigned char c);             // '0' -> '9'
bool is_lower(unsigned char c);             // 'a' -> 'z'
bool is_upper(unsigned char c);             // 'A' -> 'Z'
bool is_alpha(unsigned char c);             // is_lower || is_upper || '_'
bool is_alnum(unsigned char c);             // is_digit || is_alpha
bool is_alpha_hex(unsigned char c);         // 'a' -> 'f' || 'A' -> 'F'
bool is_hex(unsigned char c);               // '0' -> '9' || is_alpha_hex
bool is_octal(unsigned char c);             // '0' -> '7'
bool is_in_escape_list(unsigned char c);    // ', ", ?, \, a, b, f, n, r, t 
bool is_whitespace(unsigned char c);        // SOH (0x01) -> SPACE (0x20)

unsigned char to_upper(unsigned char c);

#endif