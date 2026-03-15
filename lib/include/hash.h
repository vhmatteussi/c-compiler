#ifndef COMPILER_HASH_H
#define COMPILER_HASH_H

#include <stdint.h>
#include <stddef.h>

#if UINTPTR_MAX == 0xffffffffffffffff
    typedef uint64_t hash_t;
    #define fnv1a(data, lenght) fnv1a_64(data, lenght)
    #define fnv1a_str(str) fnv1a_64_str(str)
#else
    typedef uint32_t hash_t;
    #define fnv1a(data, lenght) fnv1a_32(data, lenght)
    #define fnv1a_str(str) fnv1a_32_str(str)
#endif

static const uint32_t FNV_OFFSET_32 = 0x811c9dc5; 
static const uint32_t FNV_PRIME_32 = 0x01000193; 

static const uint64_t FNV_OFFSET_64 = 0xcbf29ce484222325ULL;
static const uint64_t FNV_PRIME_64 = 0x00000100000001B3ULL;

uint32_t fnv1a_32(const void* data, size_t lenght);
uint32_t fnv1a_32_str(const char* str);

uint64_t fnv1a_64(const void* data, size_t lenght);
uint64_t fnv1a_64_str(const char* str);

#endif