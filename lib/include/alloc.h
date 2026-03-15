#ifndef COMPILER_ALLOC_H
#define COMPILER_ALLOC_H

#include <stdint.h>
#include <stddef.h>

// Truque pra saber o alinhamento máximo, já que não tem _Alignas e _Alignof no C99
// Pega o alinhamento dos tipos fundamentais, melhor do que assumir um alinhamento para uma arquitetura
typedef union MaxAlign{
    long long i;        // 8
    long double d;      // 8 ou 16
    void *p;            // 8
    void (*fn)(void);   // 8
} MaxAlign;

// max(8, 8 ou 16, 8, 8) -> 8 : 16
static const size_t ARENA_ALIGNMENT = sizeof(MaxAlign);

typedef struct Arena{
    uint8_t *buffer;
    size_t capacity;
    size_t offset;
} Arena;

void init_arena(Arena *a, void *backing_buffer, size_t capacity);
void *arena_malloc(Arena *a, size_t size);
void *arena_calloc(Arena *a, size_t count, size_t size);
void reset_arena(Arena *a);

#endif