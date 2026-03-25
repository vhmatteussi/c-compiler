#ifndef COMPILER_TYPE_H
#define COMPILER_TYPE_H

#include <stdint.h>

static const uint8_t MAX_NUM = 10;
static const uint8_t MAX_CHAR = 26;
static const uint8_t MAX_HEX_CHAR = 8;
static const uint8_t MAX_OCTAL_CHAR = 8;

uint32_t is_digit(unsigned char c);             // '0' -> '9'
uint32_t is_lower(unsigned char c);             // 'a' -> 'z'
uint32_t is_upper(unsigned char c);             // 'A' -> 'Z'
uint32_t is_alpha(unsigned char c);             // is_lower || is_upper || '_'
uint32_t is_alnum(unsigned char c);             // is_digit || is_alpha
uint32_t is_alpha_hex(unsigned char c);         // 'a' -> 'f' || 'A' -> 'F'
uint32_t is_hex(unsigned char c);               // '0' -> '9' || is_alpha_hex
uint32_t is_octal(unsigned char c);             // '0' -> '7'
uint32_t is_in_escape_list(unsigned char c);    // ', ", ?, \, a, b, f, n, r, t 
uint32_t is_whitespace(unsigned char c);        // SOH (0x01) -> SPACE (0x20)

#endif