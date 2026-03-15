#include "alloc.h"

static inline size_t _align_up(size_t size, size_t align){
    return (size + align - 1) & ~(align - 1);
}

static inline void _memory_zero(void *p, size_t size){
    uint8_t *ptr = (uint8_t*)p;
    for(size_t i=0; i<size; i++){
        ptr[i] = 0;
    }
}

inline void init_arena(Arena *a, void *backing_buffer, size_t capacity){
    a->buffer = (uint8_t*)backing_buffer;
    a->capacity = capacity;
    a->offset = 0;
}

void *arena_malloc(Arena *a, size_t size){
    size_t aligned_size = _align_up(size, ARENA_ALIGNMENT);

    if(a->offset + aligned_size > a->capacity){
        //panic
        
        /*
            Código inútil mas engraçado para forçar um segfault
            volatile fala pro gcc não otimizar a variável de forma alguma
            quando ela aponta pra posição de memória 1, da segfault
            pq essa posição é reservada pelo OS
        */
        volatile int32_t *crash = 0;
        *crash = 1;
        
        return NULL;
    }

    void *p = &a->buffer[a->offset];
    a->offset += aligned_size;

    return p;
}

void *arena_calloc(Arena *a, size_t count, size_t size){
    size_t total_size = count * size;
    void *p = arena_malloc(a, total_size);

    if(p){
        _memory_zero(p, total_size);
    }

    return p;
}

void reset_arena(Arena *a){
    a->offset = 0;
}