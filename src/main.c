#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

unsigned char *read_source(Arena *a, const char *file_path){
    FILE *file = fopen(file_path, "rb");
    if(file == NULL){
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if(file_size < 1){
        fclose(file);
        return NULL;
    }

    unsigned char *file_buffer = arena_malloc(a, file_size + 1);
    if(file_buffer == NULL){
        fclose(file);
        return NULL;
    }

    fread(file_buffer, 1, file_size, file);
    file_buffer[file_size] = '\0';
    
    fclose(file);
    return file_buffer;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("few arguments\n");
        exit(1);
    }

    if(argc > 3){
        printf("many arguments\n");
        exit(2);
    }

    const char *file_path = argv[1];
    size_t arena_cap = 32 * 1024 * 1024; // 32MB
    void *backing_mem = malloc(arena_cap);
    if(backing_mem == NULL){
        printf("out of memory\n");
    }

    Arena compiler_arena;
    init_arena(&compiler_arena, backing_mem, arena_cap);

    unsigned char *source_code = read_source(&compiler_arena, file_path);
    if(source_code == NULL){
        printf("couldn't read this file\n");
        free(backing_mem);
        exit(3);
    }

    Lexer *lexer = init_lexer(&compiler_arena, source_code);
    Parser p;
    p.l = lexer;
    advance(&p);

    bool parse = translation_unit(&p);
    printf("\n");
    if(parse && get_current_token(&p)){
        printf("no syntax errors found\n");
    }

    free(backing_mem);
    exit(0);
}