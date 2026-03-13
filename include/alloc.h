#ifndef COMPILER_ALLOC_H
#define COMPILER_ALLOC_H

#include <stddef.h>
#include <stdint.h>

#define ALIGNMENT 8
#define ALIGN_UP(size, align) (((size) + ((align) - 1)) & ~((align) - 1))

#define ARENA_CAP 0x4000000 // 64MB

static uint8_t arena_buff[ARENA_CAP];
static size_t arena_offset = 0;

static inline void *malloc(size_t size){
    size_t aligned_size = ALIGN_UP(size, ALIGNMENT);

    if(arena_offset + aligned_size > ARENA_CAP){
        //panic
        return NULL;
    }

    void *p = &arena_buff[arena_offset];
    arena_offset += aligned_size;

    return p;
}

static inline void *calloc(size_t count, size_t size){
    size_t total_size = count * size;
    void *p = malloc(total_size);

    if(p){
        size_t aligned_size = ALIGN_UP(total_size, ALIGNMENT);
        size_t dwords = aligned_size / 8;
        uint64_t* p64 = (uint64_t*)p;

        for(size_t i=0; i<dwords; i++){
            p64[i] = 0x00;
        }
    }

    return p;
}

static inline void reset_arena(void){
    arena_offset = 0;
}

#endif