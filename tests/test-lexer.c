#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "lexer.h"

const char* token_type_to_str(TokenType type){
    switch(type){
        case TOK_EOF: return "TOK_EOF";
        case TOK_ERR: return "TOK_ERR";
        case TOK_ID: return "TOK_ID";
        case TOK_NUM_INT: return "TOK_NUM_INT";
        case TOK_NUM_FLOAT: return "TOK_NUM_FLOAT";
        case TOK_LIT_STRING: return "TOK_LIT_STRING";
        case TOK_LIT_CHAR: return "TOK_LIT_CHAR";
        case TOK_KW_CHAR: return "TOK_KW_CHAR";
        case TOK_KW_INT: return "TOK_KW_INT";
        case TOK_KW_SHORT: return "TOK_KW_SHORT";
        case TOK_KW_LONG: return "TOK_KW_LONG";
        case TOK_KW_FLOAT: return "TOK_KW_FLOAT";
        case TOK_KW_DOUBLE: return "TOK_KW_DOUBLE";
        case TOK_KW_VOID: return "TOK_KW_VOID";
        case TOK_KW_SIGNED: return "TOK_KW_SIGNED";
        case TOK_KW_UNSIGNED: return "TOK_KW_UNSIGNED";
        case TOK_KW_STATIC: return "TOK_KW_STATIC";
        case TOK_KW_EXTERN: return "TOK_KW_EXTERN";
        case TOK_KW_AUTO: return "TOK_KW_AUTO";
        case TOK_KW_VOLATILE: return "TOK_KW_VOLATILE";
        case TOK_KW_REGISTER: return "TOK_KW_REGISTER";
        case TOK_KW_INLINE: return "TOK_KW_INLINE";
        case TOK_KW_RESTRICT: return "TOK_KW_RESTRICT";
        case TOK_KW_GOTO: return "TOK_KW_GOTO";
        case TOK_KW_IF: return "TOK_KW_IF";
        case TOK_KW_ELSE: return "TOK_KW_ELSE";
        case TOK_KW_SWITCH: return "TOK_KW_SWITCH";
        case TOK_KW_CASE: return "TOK_KW_CASE";
        case TOK_KW_DEFAULT: return "TOK_KW_DEFAULT";
        case TOK_KW_BREAK: return "TOK_KW_BREAK";
        case TOK_KW_CONTINUE: return "TOK_KW_CONTINUE";
        case TOK_KW_DO: return "TOK_KW_DO";
        case TOK_KW_WHILE: return "TOK_KW_WHILE";
        case TOK_KW_FOR: return "TOK_KW_FOR";
        case TOK_KW_RETURN: return "TOK_KW_RETURN";
        case TOK_KW_ENUM: return "TOK_KW_ENUM";
        case TOK_KW_STRUCT: return "TOK_KW_STRUCT";
        case TOK_KW_UNION: return "TOK_KW_UNION";
        case TOK_KW_TYPEDEF: return "TOK_KW_TYPEDEF";
        case TOK_KW_SIZEOF: return "TOK_KW_SIZEOF";
        case TOK_LPAREN: return "TOK_LPAREN";
        case TOK_RPAREN: return "TOK_RPAREN";
        case TOK_LBRACKET: return "TOK_LBRACKET";
        case TOK_RBRACKET: return "TOK_RBRACKET";
        case TOK_LBRACE: return "TOK_LBRACE";
        case TOK_RBRACE: return "TOK_RBRACE";
        case TOK_SEMICOLON: return "TOK_SEMICOLON";
        case TOK_COMMA: return "TOK_COMMA";
        case TOK_DOT: return "TOK_DOT";
        case TOK_ARROW: return "TOK_ARROW";
        case TOK_QUESTION: return "TOK_QUESTION";
        case TOK_COLON: return "TOK_COLON";
        case TOK_ELLIPSIS: return "TOK_ELLIPSIS";
        case TOK_PLUS: return "TOK_PLUS";
        case TOK_MINUS: return "TOK_MINUS";
        case TOK_STAR: return "TOK_STAR";
        case TOK_SLASH: return "TOK_SLASH";
        case TOK_MOD: return "TOK_MOD";
        case TOK_INC: return "TOK_INC";
        case TOK_DEC: return "TOK_DEC";
        case TOK_EQUAL_EQUAL: return "TOK_EQUAL_EQUAL";
        case TOK_NOT_EQUAL: return "TOK_NOT_EQUAL";
        case TOK_LESS: return "TOK_LESS";
        case TOK_GREATER: return "TOK_GREATER";
        case TOK_LESS_EQUAL: return "TOK_LESS_EQUAL";
        case TOK_GREATER_EQUAL: return "TOK_GREATER_EQUAL";
        case TOK_LOGICAL_AND: return "TOK_LOGICAL_AND";
        case TOK_LOGICAL_OR: return "TOK_LOGICAL_OR";
        case TOK_LOGICAL_NOT: return "TOK_LOGICAL_NOT";
        case TOK_BIT_AND: return "TOK_BIT_AND";
        case TOK_BIT_OR: return "TOK_BIT_OR";
        case TOK_BIT_XOR: return "TOK_BIT_XOR";
        case TOK_BIT_NOT: return "TOK_BIT_NOT";
        case TOK_SHIFT_LEFT: return "TOK_SHIFT_LEFT";
        case TOK_SHIFT_RIGHT: return "TOK_SHIFT_RIGHT";
        case TOK_ASSIGN: return "TOK_ASSIGN";
        case TOK_PLUS_ASSIGN: return "TOK_PLUS_ASSIGN";
        case TOK_MINUS_ASSIGN: return "TOK_MINUS_ASSIGN";
        case TOK_STAR_ASSIGN: return "TOK_STAR_ASSIGN";
        case TOK_SLASH_ASSIGN: return "TOK_SLASH_ASSIGN";
        case TOK_MOD_ASSIGN: return "TOK_MOD_ASSIGN";
        case TOK_SHL_ASSIGN: return "TOK_SHL_ASSIGN";
        case TOK_SHR_ASSIGN: return "TOK_SHR_ASSIGN";
        case TOK_AND_ASSIGN: return "TOK_AND_ASSIGN";
        case TOK_XOR_ASSIGN: return "TOK_XOR_ASSIGN";
        case TOK_OR_ASSIGN: return "TOK_OR_ASSIGN";
        case TOK_KW_BOOL: return "TOK_KW_BOOL";
        case TOK_KW_COMPLEX: return "TOK_KW_COMPLEX";
        case TOK_KW_IMAGINARY: return "TOK_KW_IMAGINARY";
        default: return "OTHER/OP";
    }
}

int main(int argc, char **argv){
    if(argc < 2){
        printf("few arguments\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if(!file){
        printf("can't open the file for some reason\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    size_t arena_cap = 16 * 1024 * 1024;
    void *backing_mem = malloc(arena_cap);
    if(!backing_mem){
        printf("err: no memory!");
        fclose(file);
        return 1;
    }

    Arena compiler_arena;
    init_arena(&compiler_arena, backing_mem, arena_cap);

    unsigned char *buffer = (unsigned char *)arena_malloc(&compiler_arena, file_size + 1);
    if(!buffer){
        printf("Err: out of memory in arena!\n");
        fclose(file);
        free(backing_mem);
        return 1;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);

    for(long i = 0; i < file_size; i++){
        if(buffer[i] == '#'){
            while(i < file_size && buffer[i] != '\n' && (i == 0 || buffer[i-1] == '\n')){
                buffer[i] = ' ';
                i++;
            }
        }
    }

    Lexer *lexer = init_lexer(&compiler_arena, buffer);
    Token *tok;

    printf("%-15s | %-5s | %-5s | %s\n", "TYPE", "LINE", "COL", "LEX");
    printf("------------------------------------------------------------------\n");

    do{
        tok = next_token(lexer);
        
        printf("%-15s | %-5zu | %-5zu | '%s'\n", 
               token_type_to_str(tok->type), 
               tok->line,
               tok->col,
               tok->lex->data);

    }while(tok->type != TOK_EOF && tok->type != TOK_ERR);

    if(tok->type == TOK_ERR){
        printf("\nErr: on line %zu, col %zu. Invalid char: '%s'\n", 
               tok->line, tok->col, tok->lex->data);
    }
    else{
        printf("\nDone, no errors\n");
    }

    reset_arena(&compiler_arena);
    free(backing_mem);
    return 0;
}