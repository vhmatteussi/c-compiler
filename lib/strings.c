#include "include/strings.h"

uint32_t rawstr_size(const char *s){
    const char *s_aux = s;
    while(*s){
        s++;
    }
    return (uint32_t)(s - s_aux);
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
        // copia *src (content) para o ponteiro que aponta pro conteúdo da String new_str
        *dest++ = *src++;
    }
    // o fim do while necessáriamente vai ser em lenght+1, então dá só pra jogar o \0 aqui, mas já vai ter um em lenght se tudo der certo
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

    // copia a primeira string
    const char *src = s1->data;
    for(uint32_t i=0; i<s1->length; i++){
        *dest++ = *src++;
    }

    // copia a segunda string
    src = s2->data;
    for(uint32_t i=0; i<s2->length; i++){
        *dest++ = *src++;
    }

    *dest = '\0';

    return new_str;
}

uint32_t string_cmp(const String *s1, const String *s2){
    if(s1->length != s2->length){
        return (s1->length > s2->length) ? -1 : -2; // tamanhos diferentes
    }

    // s1 e s2 são o mesmo ponteiro
    if(s1 == s2){
        return 1;
    }

    const char *p1 = s1->data;
    const char *p2 = s2->data;
    for(unsigned int i=0; i<s1->length; i++){
        if(p1[i] != p2[i]) return 0;
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

static void intern_keyword(Arena *a, StringInterner *interner, const char *str, TokenType type){
    uint32_t lenght = rawstr_size(str);
    String *kw_str = intern_string(a, interner, str, lenght);
    if(kw_str){
        kw_str->kw_type = type;
    }
}

void init_interner(Arena *a, StringInterner *interner){
    for(uint32_t i=0; i<STRING_INTERN_CAP; i++){
        interner->entries[i] = NULL;
    }

    intern_keyword(a, interner, "char", TOK_KW_CHAR);
    intern_keyword(a, interner, "int", TOK_KW_INT);
    intern_keyword(a, interner, "short", TOK_KW_SHORT);
    intern_keyword(a, interner, "long", TOK_KW_LONG);
    intern_keyword(a, interner, "float", TOK_KW_FLOAT);
    intern_keyword(a, interner, "double", TOK_KW_DOUBLE);
    intern_keyword(a, interner, "void", TOK_KW_VOID);
    intern_keyword(a, interner, "signed", TOK_KW_SIGNED);
    intern_keyword(a, interner, "unsgined", TOK_KW_UNSIGNED);
    intern_keyword(a, interner, "static", TOK_KW_STATIC);
    intern_keyword(a, interner, "extern", TOK_KW_EXTERN);
    intern_keyword(a, interner, "auto", TOK_KW_AUTO);
    intern_keyword(a, interner, "volatile", TOK_KW_VOLATILE);
    intern_keyword(a, interner, "register", TOK_KW_REGISTER);
    intern_keyword(a, interner, "goto", TOK_KW_GOTO);
    intern_keyword(a, interner, "if", TOK_KW_IF);
    intern_keyword(a, interner, "else", TOK_KW_ELSE);
    intern_keyword(a, interner, "switch", TOK_KW_SWITCH);
    intern_keyword(a, interner, "case", TOK_KW_CASE);
    intern_keyword(a, interner, "default", TOK_KW_DEFAULT);
    intern_keyword(a, interner, "break", TOK_KW_BREAK);
    intern_keyword(a, interner, "continue", TOK_KW_CONTINUE);
    intern_keyword(a, interner, "do", TOK_KW_DO);
    intern_keyword(a, interner, "while", TOK_KW_WHILE);
    intern_keyword(a, interner, "for", TOK_KW_FOR);
    intern_keyword(a, interner, "return", TOK_KW_RETURN);
    intern_keyword(a, interner, "enum", TOK_KW_ENUM);
    intern_keyword(a, interner, "struct", TOK_KW_STRUCT);
    intern_keyword(a, interner, "union", TOK_KW_UNION);
    intern_keyword(a, interner, "typedef", TOK_KW_TYPEDEF);
    intern_keyword(a, interner, "sizeof", TOK_KW_SIZEOF);
    intern_keyword(a, interner, "inline", TOK_KW_INLINE);
    intern_keyword(a, interner, "restrict", TOK_KW_RESTRICT);
    intern_keyword(a, interner, "_Bool", TOK_KW_BOOL);
    intern_keyword(a, interner, "_Complex", TOK_KW_COMPLEX);
    intern_keyword(a, interner, "_Imaginary", TOK_KW_IMAGINARY);
}

String *intern_string(Arena *a, StringInterner *interner, const char *str, uint32_t lenght){
    hash_t hash = fnv1a(str, lenght);
    uint32_t idx = hash % STRING_INTERN_CAP;

    while(interner->entries[idx] != NULL){
        String *exists = interner->entries[idx];
        if(string_eq_raw(exists, str, lenght)){
            return exists;
        }
        idx = (idx + 1) % STRING_INTERN_CAP;
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
    interner->entries[idx] = new_str;
    return new_str;
}