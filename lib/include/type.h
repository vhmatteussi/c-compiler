#ifndef COMPILER_TYPE_H
#define COMPILER_TYPE_H

#include <stdint.h>

// algumas funções da ctype, porém sem usar a técnica de bitmasks que a ctype.h usa

static const uint8_t MAX_NUM = 10;
static const uint8_t MAX_CHAR = 26;

// algumas dessas funções não se comportam igual ao ctype e servem só pro lexer

uint32_t is_digit(unsigned char c);
uint32_t is_lower(unsigned char c);
uint32_t is_upper(unsigned char c);
uint32_t is_alpha(unsigned char c);
uint32_t is_whitespace(unsigned char c);

#endif