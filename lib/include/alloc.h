#ifndef COMPILER_ALLOC_H
#define COMPILER_ALLOC_H

#include <stdint.h>
#include <stddef.h>

// truque pra saber o alinhamento máximo pq meu compilador não vai saber o que é _Alignas e _Alignof
typedef union MaxAlign{
    long long i;
    long double d;
    void *p;
    void (*fn)(void);
} MaxAlign;

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