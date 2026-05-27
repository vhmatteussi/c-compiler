#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include "lexer.h"
//#include "ast.h"
#include <stdio.h>

// workaround to be removed
// change on-the-fly to token list iteration
typedef struct ParserState{
    Token *current_token;
    size_t lexer_start;
    size_t lexer_forward;
    size_t lexer_line;
    size_t lexer_col;
} ParserState;

typedef struct Parser{
    Lexer *l;
    Token *current_token;
    Arena *arena;
    //Node *ast;
    bool had_an_error;
    int error_count;
} Parser;

Parser *init_parser(Arena *a, Lexer *l);
TokenType get_current_token(Parser *p);
void get_next_token(Parser *p);
bool match(Parser *p, TokenType type_to_match);
void parser_error(Parser *p, const char *msg);
void panic_mode(Parser *p);

void parser_save_state(Parser *p, ParserState *state);
void parser_restore_state(Parser *p, ParserState *state);

bool translation_unit(Parser *p);
bool external_declaration(Parser *p);
bool function_definition(Parser *p);
bool declaration_specifiers(Parser *p);
bool storage_class_specifiers(Parser *p);
bool type_specifier(Parser *p);
bool struct_or_union_specifier(Parser *p);
bool struct_or_union(Parser *p);
bool struct_declaration_list(Parser *p);
bool struct_declaration(Parser *p);
bool specifier_qualifier_list(Parser *p);
bool type_qualifier(Parser *p);
bool struct_declarator_list(Parser *p);
bool struct_declarator(Parser *p);
bool declarator(Parser *p);
bool pointer(Parser *p);
bool type_qualifier_list(Parser *p);
bool direct_declarator(Parser *p);
bool assignment_expression(Parser *p);
bool conditional_expression(Parser *p);
bool logical_or_expression(Parser *p);
bool logical_and_expression(Parser *p);
bool inclusive_or_expression(Parser *p);
bool exclusive_or_expression(Parser *p);
bool and_expression(Parser *p);
bool equality_expression(Parser *p);
bool relational_expression(Parser *p);
bool shift_expression(Parser *p);
bool additive_expression(Parser *p);
bool multiplicative_expression(Parser *p);
bool cast_expression(Parser *p);
bool unary_expression(Parser *p);
bool postfix_expression(Parser *p);
bool primary_expression(Parser *p);
bool expression(Parser *p);
bool argument_expression_list(Parser *p);
bool type_name(Parser *p);
bool abstract_declarator(Parser *p);
bool direct_abstract_declarator(Parser *p);
bool parameter_type_list(Parser *p);
bool parameter_list(Parser *p);
bool parameter_declaration(Parser *p);
bool initializer_list(Parser *p);
bool designation(Parser *p);
bool designator_list(Parser *p);
bool designator(Parser *p);
bool constant_expression(Parser *p);
bool initializer(Parser *p);
bool unary_operator(Parser *p);
bool assignment_operator(Parser *p);
bool identifier_list(Parser *p);
bool enum_specifier(Parser *p);
bool enumerator_list(Parser *p);
bool enumarator(Parser *p);
bool typedef_name(Parser *p);
bool function_specifier(Parser *p);
bool declaration_list(Parser *p);
bool declaration(Parser *p);
bool init_declarator_list(Parser *p);
bool init_declarator(Parser *p);
bool compound_statement(Parser *p);
bool block_item_list(Parser *p);
bool block_item(Parser *p);
bool statement(Parser *p);
bool labeled_statement(Parser *p);
bool expression_statement(Parser *p);
bool selection_statement(Parser *p);
bool iteration_statement(Parser *p);
bool jump_statement(Parser *p);

#endif