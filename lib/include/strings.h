#ifndef COMPILER_STRINGS_H
#define COMPILER_STRINGS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "alloc.h"
#include "token.h"

// cool numbers idk
#define _INTERN_INITIAL_CAP 256
#define _INTERN_CAP_THRESHOLD 0.8f

#if defined(__x86_64__) || defined(_M_AMD64)
    typedef uint64_t hash_t;
    #define _FNV_OFFSET UINT64_C(0xcbf29ce484222325)
    #define _FNV_PRIME UINT64_C(0x00000100000001B3)
#else
    typedef uint32_t hash_t;
    #define _FNV_OFFSET UINT32_C(0x811c9dc5)
    #define _FNV_PRIME UINT32_C(0x01000193)
#endif

typedef struct String{
    uint32_t length;
    TokenType kw_type;
    char data[];
} String;

typedef struct StringIntern{
    uint32_t cap;
    uint32_t entry_count;
    String **entries;
} StringIntern;

// i have some really bad news to myself if i ever need to use this elsewhere
hash_t fnv1a(const void *data, size_t lenght); // Fowler-Noll-Vo hash function

uint32_t rawstr_size(const char *s);

String *init_string(Arena *a, const char *content);
String *string_cat(Arena *a, const String *s1, const String *s2);
uint32_t string_cmp(const String *s1, const String *s2);

void init_intern(Arena *a, StringIntern *intern);
String *intern_string(Arena *a, StringIntern *intern, const char *str, uint32_t lenght);

#endif