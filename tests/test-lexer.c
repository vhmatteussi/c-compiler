#include <stdio.h>
#include "include/token.h"
#include "include/lexer.h"

const char* token_type_to_str(TokenType type){
    switch(type){
        case TOK_EOF: return "TOK_EOF";
        case TOK_ERR: return "TOK_ERR";
        case TOK_ID: return "TOK_ID";
        case TOK_NUM_INT: return "TOK_NUM_INT";
        case TOK_NUM_FLOAT: return "TOK_NUM_FLOAT";
        case TOK_LIT_STRING: return "TOK_LIT_STRING";
        case TOK_LIT_CHAR: return "TOK_LIT_CHAR";

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

        default: return "OTHER/OP";
    }
}

int main(int argc, char **argv){
    (void)argc;

    FILE *file = fopen(argv[1], "rb");
    if(!file){
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *)malloc(file_size + 1);
    if(!buffer){
        printf("Err: out of memory in arena!\n");
        fclose(file);
        return 1;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);

    for(long i = 0; i < file_size; i++){
        if(buffer[i] == '#'){
            while(i < file_size && buffer[i] != '\n'){
                buffer[i] = ' ';
                i++;
            }
        }
    }

    Lexer *lexer = init_lexer(buffer);
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

    reset_arena();
    return 0;
}