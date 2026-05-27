#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include "lexer.h"
#include "ast.h"
#include <stdio.h>

typedef struct Parser{
    Lexer *l;
    Token *current_token;
    Arena *arena;
    Node *ast;
    bool had_an_error;
} Parser;

Parser *init_parser(Lexer *l, Arena *a);
TokenType get_current_token(Parser *p);
void get_next_token(Parser *p);
bool match(Parser *p, TokenType type_to_match);

Node *translation_unit(Parser *p);
Node *external_declaration(Parser *p);
Node *function_definition(Parser *p);
Node *declaration_specifiers(Parser *p);
Node *storage_class_specifiers(Parser *p);
Node *type_specifier(Parser *p);
Node *struct_or_union_specifier(Parser *p);
Node *struct_or_union(Parser *p);
Node *struct_declaration_list(Parser *p);
Node *struct_declaration(Parser *p);
Node *specifier_qualifier_list(Parser *p);
Node *type_qualifier(Parser *p);
Node *struct_declarator_list(Parser *p);
Node *struct_declarator(Parser *p);
Node *declarator(Parser *p);
Node *pointer(Parser *p);
Node *type_qualifier_list(Parser *p);
Node *direct_declarator(Parser *p);
Node *assignment_expression(Parser *p);
Node *conditional_expression(Parser *p);
Node *logical_or_expression(Parser *p);
Node *logical_and_expression(Parser *p);
Node *inclusive_or_expression(Parser *p);
Node *exclusive_or_expression(Parser *p);
Node *and_expression(Parser *p);
Node *equality_expression(Parser *p);
Node *relational_expression(Parser *p);
Node *shift_expression(Parser *p);
Node *additive_expression(Parser *p);
Node *multiplicative_expression(Parser *p);
Node *cast_expression(Parser *p);
Node *unary_expression(Parser *p);
Node *postfix_expression(Parser *p);
Node *primary_expression(Parser *p);
Node *expression(Parser *p);
Node *argument_expression_list(Parser *p);
Node *type_name(Parser *p);
Node *abstract_declarator(Parser *p);
Node *direct_abstract_declarator(Parser *p);
Node *parameter_type_list(Parser *p);
Node *parameter_list(Parser *p);
Node *parameter_declaration(Parser *p);
Node *initializer_list(Parser *p);
Node *designation(Parser *p);
Node *designator_list(Parser *p);
Node *designator(Parser *p);
Node *constant_expression(Parser *p);
Node *initializer(Parser *p);
Node *unary_operator(Parser *p);
Node *assignment_operator(Parser *p);
Node *identifier_list(Parser *p);
Node *enum_specifier(Parser *p);
Node *enumerator_list(Parser *p);
Node *enumarator(Parser *p);
Node *typedef_name(Parser *p);
Node *function_specifier(Parser *p);
Node *declaration_list(Parser *p);
Node *declaration(Parser *p);
Node *init_declarator_list(Parser *p);
Node *init_declarator(Parser *p);
Node *compound_statement(Parser *p);
Node *block_item_list(Parser *p);
Node *block_item(Parser *p);
Node *statement(Parser *p);
Node *labeled_statement(Parser *p);
Node *expression_statement(Parser *p);
Node *selection_statement(Parser *p);
Node *iteration_statement(Parser *p);
Node *jump_statement(Parser *p);

#endif