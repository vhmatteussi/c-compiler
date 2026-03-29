#include "alloc.h"

void init_arena(Arena *a, void *backing_buffer, size_t capacity){
    a->buffer = (uint8_t*)backing_buffer;
    a->capacity = capacity;
    a->offset = 0;
}

void *arena_malloc(Arena *a, size_t size){
    size_t aligned_size = _ALIGN_UP(size, _ARENA_ALIGNMENT);
    if(a->offset + aligned_size > a->capacity){
        // fodeow
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
        for(size_t i=0; i<total_size; i++){
            // meh, if it works it works
            ((uint8_t*)p)[i] = 0x00;
        }
    }
    return p;
}

void reset_arena(Arena *a){
    a->offset = 0;
}