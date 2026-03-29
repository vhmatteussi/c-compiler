#include "strings.h"

hash_t fnv1a(const void *data, size_t lenght){
    const uint8_t* bytes = (const uint8_t*)data;
    hash_t hash = _FNV_OFFSET;

    for(size_t i=0; i<lenght; i++){
        hash ^= bytes[i];
        hash *= _FNV_PRIME;
    }
    return hash;
}

uint32_t rawstr_size(const char *s){
    const char *s_aux = s;
    while(*s){
        s++;
    }
    return ((uint32_t)(s - s_aux));
}

String *init_string(Arena *a, const char *content){
    uint32_t lenght = rawstr_size(content);
    String *new_str = (String*)arena_malloc(a, sizeof(String) + lenght + 1);
    if(!new_str){
        return NULL;
    }
    new_str->length = lenght;
    char *dest = new_str->data;
    const char *src = content;
    while(*src){
        *dest++ = *src++;
    }
    *dest = '\0';
    return new_str;
}

String *string_cat(Arena *a, const String *s1, const String *s2){
    uint32_t lenght = s1->length + s2->length;
    String *new_str = (String*)arena_malloc(a, sizeof(String) + lenght + 1);
    if(!new_str){
        return NULL;
    }
    new_str->length = lenght;
    char *dest = new_str->data;
    const char *src = s1->data;
    for(uint32_t i=0; i<s1->length; i++){
        *dest++ = *src++;
    }
    src = s2->data;
    for(uint32_t i=0; i<s2->length; i++){
        *dest++ = *src++;
    }
    *dest = '\0';
    return new_str;
}

uint32_t string_cmp(const String *s1, const String *s2){
    if(s1->length > s2->length){
        return -1;
    }
    else if(s1->length < s2->length){
        return -2;
    }
    else if(s1 == s2){
        return 1;
    }
    const char *p1 = s1->data;
    const char *p2 = s2->data;
    for(uint32_t i=0; i<s1->length; i++){
        if(p1[i] != p2[i]){
            return 0;
        }
    }
    return 1;
}

static inline uint32_t string_eq_raw(String *s1, const char *raw_str, uint32_t lenght){
    if(s1->length != lenght){
        return 0;
    }
    for(uint32_t i=0; i<lenght; i++){
        if(s1->data[i] != raw_str[i]){
            return 0;
        }
    }
    return 1;
}

static inline void resize_intern(Arena *a, StringIntern *intern){
    // horrible code, i will need to change it eventually
    uint32_t cap = intern->cap;
    String **entries = intern->entries;

    uint32_t new_cap = cap * 2;
    String **new_entries = arena_calloc(a, new_cap, sizeof(String*));

    intern->cap = new_cap;
    intern->entry_count = 0;
    intern->entries = new_entries;

    for(uint32_t i=0; i<cap; i++){
        String *str = entries[i];
        if(!str){
            continue;
        }

        hash_t hash = fnv1a(str->data, str->length);
        uint32_t idx = hash % intern->cap;

        while(intern->entries[idx] != NULL){
            idx = (idx + 1) % intern->cap;
        }

        intern->entries[idx] = str;
        intern->entry_count++;
    }
}

void init_intern(Arena *a, StringIntern *intern){
    intern->cap = _INTERN_INITIAL_CAP;
    intern->entry_count = 0;
    intern->entries = arena_calloc(a, intern->cap, sizeof(String*));
}

String *intern_string(Arena *a, StringIntern *intern, const char *str, uint32_t lenght){
    if((float)intern->entry_count / intern->cap > _INTERN_CAP_THRESHOLD){
        resize_intern(a, intern);
    }

    hash_t hash = fnv1a(str, lenght);
    uint32_t idx = hash % intern->cap;

    while(intern->entries[idx] != NULL){
        String *exists = intern->entries[idx];
        if(string_eq_raw(exists, str, lenght)){
            return exists;
        }
        idx = (idx + 1) % intern->cap;
    }

    String *new_str = (String*)arena_malloc(a, sizeof(String) + lenght + 1);
    
    if(!new_str){
        return NULL;
    }

    new_str->length = lenght;
    new_str->kw_type = TOK_ID;
    char *dest = new_str->data;

    for(uint32_t i=0; i<lenght; i++){
        dest[i] = str[i];
    }
    dest[lenght] = '\0';
    intern->entries[idx] = new_str;
    intern->entry_count++;
    return new_str;
}