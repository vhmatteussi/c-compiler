#ifndef COMPILER_STRINGS_H
#define COMPILER_STRINGS_H

#include "alloc.h"
#include "hash.h"
#include "token.h"

typedef struct String{
    uint32_t length;
    TokenType kw_type;
    char data[]; // flex array funciona melhor, *str exigiria 2 alocações
} String;

// Capacidade inicial do interner, quanto menor melhor, mas precisa ser grande o suficiente
#define INTERN_INITIAL_CAP 256
// Razão entre entry_count / cap necessária para chamar o resize, qualquer float entre 0.5 0.9 parece ok
#define INTERN_CAP_THRESHOLD 0.8f

typedef struct StringIntern{
    uint32_t cap;
    uint32_t entry_count;
    String **entries;
} StringInterner;

uint32_t rawstr_size(const char *s);

String *init_string(Arena *a, const char *content);
String *string_cat(Arena *a, const String *s1, const String *s2);
uint32_t string_cmp(const String *s1, const String *s2); // -1: S1 é maior; -2: S2 é maior; 1: São iguais; 0: São diferentes de mesmo tamanho.

void init_interner(Arena *a, StringInterner *interner);
String *intern_string(Arena *a, StringInterner *interner, const char *str, uint32_t lenght);

#endif