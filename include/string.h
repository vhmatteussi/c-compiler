#ifndef COMPILER_STRING_H
#define COMPILER_STRING_H

#include "alloc.h"

typedef struct String{
    unsigned int length;
    char data[]; // flex array funciona melhor com o meu malloc, é mais rápido para alocar só 1 vez
} String;

// talvez eu bote inline no meu compilador, caso contrário eu já tokenizei, dai é só ignorar no parser
static inline unsigned int raw_strsize(const char *s){
    // é mais rápido operar os ponteiros do que incrementar um contador, já q eu não vou adicionar esse tipo de melhoria no compilador
    const char *s_aux = s;
    while(*s) s++;
    return (unsigned int)(s - s_aux);
}

static inline String* init_str(const char *content){
    unsigned int lenght = raw_strsize(content);
    String *new_str = (String*)malloc(sizeof(String) + lenght + sizeof(char)); // usei sizeof(char) só pra não deixar 'um' número mágico
    // vou tratar qualquer caso de panic direto no malloc
    new_str->length = lenght;

    char *dest = new_str->data;
    const char *src = content;

    while(*src) *dest++ = *src++; // copia *src (content) para o ponteiro que aponta pro conteúdo da String new_str
    *dest = 0x00; // o fim do while necessáriamente vai ser em lenght+1, então dá só pra jogar o \0 aqui, mas já vai ter um em lenght se tudo der certo
    return new_str;
}

static inline String* str_cat(const String *s1, const String *s2){
    unsigned int lenght = s1->length + s2->length;
    String *new_str = (String*)malloc(sizeof(String) + lenght + sizeof(char));
    new_str->length = lenght;

    char *dest = new_str->data;
    const char *src = s1->data;
    for(unsigned int i=0; i<s1->length; i++){
        *dest++ = *src++;
    }

    src = s2->data;
    for(unsigned int i=0; i<s2->length; i++){
        *dest++ = *src++;
    }

    *dest = 0x00;

    return new_str;
}

static inline int str_cmp(const String *s1, const String *s2){
    if(s1->length != s2->length) return (s1->length > s2->length) ? -1 : -2; // tamanhos diferentes
    if(s1 == s2) return 1; // msm ponteiro
    const char *p1 = s1->data;
    const char *p2 = s2->data;
    for(unsigned int i=0; i<s1->length; i++){
        if(p1[i] != p2[i]) return 0;
    }
    return 1;
    // -1 s1 é maior, -2 s2 é maior, 1 são iguais, 0 são diferentes de mesmo tamanho.
}

#endif