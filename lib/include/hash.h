#ifndef COMPILER_HASH_H
#define COMPILER_HASH_H

#include <stdint.h>
#include <stddef.h>

/*
    Os tamanhos 64 ou 32 de inteiro, não representam ou dependem do tamanho da palavra.
    Estou usando esse macro, apenas pq o algoritmo de 32bits funciona de forma mais eficiente em
    sistemas com tamanho de página de 32 bits.
*/
#if defined __x86_64__ && !defined __ILP32__
    typedef uint64_t hash_t;
    #define fnv1a(data, lenght) fnv1a_64(data, lenght)
    #define fnv1a_str(str) fnv1a_64_str(str)
#else
    typedef uint32_t hash_t;
    #define fnv1a(data, lenght) fnv1a_32(data, lenght)
    #define fnv1a_str(str) fnv1a_32_str(str)
#endif

// Numeros mágicos do fowler-noll-vo
static const uint32_t FNV_OFFSET_32 = 0x811c9dc5; 
static const uint32_t FNV_PRIME_32 = 0x01000193; 

static const uint64_t FNV_OFFSET_64 = 0xcbf29ce484222325ULL;
static const uint64_t FNV_PRIME_64 = 0x00000100000001B3ULL;

uint32_t fnv1a_32(const void* data, size_t lenght);
uint32_t fnv1a_32_str(const char* str); // dead

uint64_t fnv1a_64(const void* data, size_t lenght);
uint64_t fnv1a_64_str(const char* str); // dead

#endif