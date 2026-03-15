#ifndef COMPILER_STRINGS_H
#define COMPILER_STRINGS_H

#include "alloc.h"
#include "hash.h"
#include "token.h"

typedef struct String{
    uint32_t length;
    TokenType kw_type;
    char data[]; // flex array funciona melhor com o meu malloc de arena, é mais rápido para alocar só 1 vez
} String;

#define STRING_INTERN_CAP 1024

typedef struct StringIntern{
    String *entries[STRING_INTERN_CAP];
} StringInterner;

uint32_t rawstr_size(const char *s);

String *init_string(Arena *a, const char *content);
String *string_cat(Arena *a, const String *s1, const String *s2);
uint32_t string_cmp(const String *s1, const String *s2); // -1 s1 é maior, -2 s2 é maior, 1 são iguais, 0 são diferentes de mesmo tamanho.

void init_interner(Arena *a, StringInterner *interner);
String *intern_string(Arena *a, StringInterner *interner, const char *str, uint32_t lenght);

#endif