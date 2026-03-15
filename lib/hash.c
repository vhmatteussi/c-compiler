#include "include/hash.h"

uint32_t fnv1a_32(const void* data, size_t lenght){
    const uint8_t* bytes = (const uint8_t*)data;
    uint32_t hash = FNV_OFFSET_32;

    for(size_t i=0; i<lenght; i++){
        hash ^= bytes[i];
        hash *= FNV_PRIME_32;
    }
    return hash;
}

uint32_t fnv1a_32_str(const char* str){
    uint32_t hash = FNV_OFFSET_32;

    while(*str){
        hash ^= (uint8_t)*str++;
        hash *= FNV_PRIME_32;
    }
    return hash;
}

uint64_t fnv1a_64(const void* data, size_t lenght){
    const uint8_t* bytes = (const uint8_t*)data;
    uint64_t hash = FNV_OFFSET_64;

    for(size_t i=0; i<lenght; i++){
        hash ^= bytes[i];
        hash *= FNV_PRIME_64;
    }
    return hash;
}

uint64_t fnv1a_64_str(const char* str){
    uint64_t hash = FNV_OFFSET_64;

    while(*str){
        hash ^= (uint8_t)*str++;
        hash *= FNV_PRIME_64;
    }
    return hash;
}