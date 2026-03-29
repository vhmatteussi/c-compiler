#ifndef COMPILER_ALLOC_H
#define COMPILER_ALLOC_H

#include <stdint.h>
#include <stddef.h>

// C99 stuff
typedef union _MaxAlign{
    long long i;
    long double d;
    void *p;
    void (*fn)(void);
} _MaxAlign;

#define _ALIGN_OF(type) ((size_t)&(((struct{char c; type member;}*)0)->member))     // yeah
#define _ARENA_ALIGNMENT _ALIGN_OF(_MaxAlign)                                       // returns 8 or 16, hopefully
#define _ALIGN_UP(size, align) (((size) + (align) - 1) & ~((align) - 1))            // align arena's pointer

typedef struct Arena{
    uint8_t *buffer;
    size_t capacity;
    size_t offset;
} Arena;

void init_arena(Arena *a, void *backing_buffer, size_t capacity);   // inits arena, who would've guessed
void *arena_malloc(Arena *a, size_t size);                          // it does what it says
void *arena_calloc(Arena *a, size_t count, size_t size);            // yup
void reset_arena(Arena *a);                                         // a->offset = 0;

#endif