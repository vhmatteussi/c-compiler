#include "parser.h"

void advance(Parser *p){
    p->current_token = next_token(p->l);
}

TokenType get_current_token(Parser *p){
    return p->current_token->type;
}

bool match(Parser *p, TokenType type_to_match){
    if(get_current_token(p) == type_to_match){
        advance(p);
        return true;
    }
    return false;
}

void error(Parser *p){
    printf("Syntax error at line %zu col %zu\n", p->l->line, p->l->col);
}

bool translation_unit(Parser *p){
    if(!external_declaration(p)){
        return false;
    }
    while(external_declaration(p));
    return true;
}

bool external_declaration(Parser *p){
    if(function_definition(p)){
        return true;
    }
    if(declaration(p)){
        return true;
    }
    return false;
}

bool function_definition(Parser *p){
    if(!declaration_specifiers(p)){
        return false;
    }
    if(!declarator(p)){
        return false;
    }
    declaration_list(p);
    if(!compound_statement(p)){
        return false;
    }
    return true;
}

bool declaration_specifiers(Parser *p){
    if(storage_class_specifiers(p)){
        declaration_specifiers(p);
        return true;
    }
    if(type_specifier(p)){
        declaration_specifiers(p);
        return true;
    }
    if(type_qualifier(p)){
        declaration_specifiers(p);
        return true;
    }
    if(function_specifier(p)){
        declaration_specifiers(p);
        return true;
    }
    return false;
}

bool storage_class_specifiers(Parser *p){
    switch(get_current_token(p)){
        case TOK_KW_TYPEDEF:
        case TOK_KW_EXTERN:
        case TOK_KW_STATIC:
        case TOK_KW_AUTO:
        case TOK_KW_REGISTER:
            advance(p);
            return true;
    }
    return false;
}

bool type_specifier(Parser *p){
    switch(get_current_token(p)){
        case TOK_KW_VOID:
        case TOK_KW_CHAR:
        case TOK_KW_SHORT:
        case TOK_KW_INT:
        case TOK_KW_LONG:
        case TOK_KW_FLOAT:
        case TOK_KW_DOUBLE:
        case TOK_KW_SIGNED:
        case TOK_KW_UNSIGNED:
        case TOK_KW_BOOL:
        case TOK_KW_COMPLEX:
            advance(p);
            return true;
    }
    if(struct_or_union_specifier(p)){
        return true;
    }
    if(enum_specifier(p)){
        return true;
    }
    if(typedef_name(p)){
        return true;
    }
    return false;
}

bool struct_or_union_specifier(Parser *p){
    if(!struct_or_union(p)){
        return false;
    }

    bool has_id = match(p, TOK_ID);
    if(match(p, TOK_LBRACE)){
        if(!struct_declaration_list(p)){
            return false;
        }
        if(!match(p, TOK_RBRACE)){
            return false;
        }
        return true;
    }
    if(!has_id){
        return false;
    }
    return true;
}

bool struct_or_union(Parser *p){
    switch(get_current_token(p)){
        case TOK_KW_STRUCT:
        case TOK_KW_UNION:
            advance(p);
            return true;
    }
    return false;
}

bool struct_declaration_list(Parser *p){
    if(!struct_declaration(p)){
        return false;
    }
    while(struct_declaration(p));
    return true;
}

bool struct_declaration(Parser *p){
    if(!specifier_qualifier_list(p)){
        return false;
    }
    if(!struct_declarator_list(p)){
        return false;
    }
    if(!match(p, TOK_SEMICOLON)){
        return false;
    }
    return true;
}

bool specifier_qualifier_list(Parser *p){
    if(type_specifier(p)){
        specifier_qualifier_list(p);
        return true;
    }
    if(type_qualifier(p)){
        specifier_qualifier_list(p);
        return true;
    }
    return false;
}

bool type_qualifier(Parser *p){
    switch(get_current_token(p)){
        case TOK_KW_CONST:
        case TOK_KW_RESTRICT:
        case TOK_KW_VOLATILE:
            advance(p);
            return true;
    }
    return false;
}

bool struct_declarator_list(Parser *p){
    if(!struct_declarator(p)){
        return false;
    }
    while(match(p, TOK_COMMA)){
        if(!struct_declarator(p)){
            return false;
        }
    }
    return true;
}

bool struct_declarator(Parser *p){
    bool has_declarator = declarator(p);
    if(match(p, TOK_COLON)){
        if(!constant_expression(p)){
            return false;
        }
        return true;
    }
    if(!has_declarator){
        return false;
    }
    return true;
}

bool declarator(Parser *p){
    pointer(p);
    if(!direct_declarator(p)){
        return false;
    }
    return true;
}

bool pointer(Parser *p){
    if(!match(p, TOK_STAR)){
        return false;
    }
    type_qualifier_list(p);
    pointer(p);
    return true;
}

bool type_qualifier_list(Parser *p){
    if(!type_qualifier(p)){
        return false;
    }
    while(type_qualifier(p));
    return true;
}

bool direct_declarator(Parser *p){
    bool has_base_case = false;
    if(match(p, TOK_ID)){
        has_base_case = true;
    }
    else if(match(p, TOK_LPAREN)){
        if(!declarator(p)){
            return false;
        }
        if(!match(p, TOK_RPAREN)){
            return false;
        }
        has_base_case = true;
    }
    bool continue_recursion = true;
    while(continue_recursion){
        continue_recursion = false;
        if(match(p, TOK_LBRACKET)){
            if(match(p, TOK_KW_STATIC)){
                type_qualifier_list(p);
                if(!assignment_expression(p)){
                    return false;
                }
            }
            else if(type_qualifier_list(p)){
                if(match(p, TOK_KW_STATIC)){
                    if(!assignment_expression(p)){
                        return false;
                    }
                }
                else if(!match(p, TOK_STAR)){
                    assignment_expression(p);
                }
            }
            else{
                if(!match(p, TOK_STAR)){
                    assignment_expression(p);
                }
            }
            if(!match(p, TOK_RBRACKET)){
                return false;
            }
            continue_recursion = true;
            has_base_case = true;
        }
        else if(match(p, TOK_LPAREN)){
            if(!parameter_type_list(p)){
                identifier_list(p);
            }
            if(!match(p, TOK_RPAREN)){
                return false;
            }
            continue_recursion = true;
            has_base_case = true;
        }
    }
    return has_base_case;
}

bool assignment_expression(Parser *p){
    if(conditional_expression(p)){
        return true;
    }
    if(unary_expression(p)){
        if(!assignment_operator(p)){
            return false;
        }
        if(!assignment_expression(p)){
            return false;
        }
        return true;
    }
    return false;
}

bool conditional_expression(Parser *p){
    if(!logical_or_expression(p)){
        return false;
    }
    if(match(p, TOK_QUESTION)){
        if(!expression(p)){
            return false;
        }
        if(!match(p, TOK_COLON)){
            return false;
        }
        if(!conditional_expression(p)){
            return false;
        }
    }
    return true;
}

bool logical_or_expression(Parser *p){
    if(!logical_and_expression(p)){
        return false;
    }
    while(match(p, TOK_LOGICAL_OR)){
        if(!logical_and_expression(p)){
            return false;
        }
    }
    return true;
}

bool logical_and_expression(Parser *p){
    if(!inclusive_or_expression(p)){
        return false;
    }
    while(match(p, TOK_LOGICAL_AND)){
        if(!inclusive_or_expression(p)){
            return false;
        }
    }
    return true;
}

bool inclusive_or_expression(Parser *p){
    if(!exclusive_or_expression(p)){
        return false;
    }
    while(match(p, TOK_BIT_OR)){
        if(!exclusive_or_expression(p)){
            return false;
        }
    }
    return true;
}

bool exclusive_or_expression(Parser *p){
    if(!and_expression(p)){
        return false;
    }
    while(match(p, TOK_BIT_XOR)){
        if(!and_expression(p)){
            return false;
        }
    }
    return true;
}

bool and_expression(Parser *p){
    if(!equality_expression(p)){
        return false;
    }
    while(match(p, TOK_BIT_AND)){
        if(!equality_expression(p)){
            return false;
        }
    }
    return true;
}

bool equality_expression(Parser *p){
    if(!relational_expression(p)){
        return false;
    }
    while(match(p, TOK_EQUAL_EQUAL) || match(p,TOK_NOT_EQUAL)){
        if(!relational_expression(p)){
            return false;
        }
    }
    return true;
}

bool relational_expression(Parser *p){
    if(!shift_expression(p)){
        return false;
    }
    while(match(p, TOK_LESS) || match(p, TOK_GREATER) || match(p, TOK_LESS_EQUAL) || match(p, TOK_GREATER_EQUAL)){
        if(!shift_expression(p)){
            return false;
        }
    }
    return true;
}

bool shift_expression(Parser *p){
    if(!additive_expression(p)){
        return false;
    }
    while(match(p, TOK_SHIFT_LEFT) || match(p, TOK_SHIFT_RIGHT)){
        if(!additive_expression(p)){
            return false;
        }
    }
    return true;
}

bool additive_expression(Parser *p){
    if(!multiplicative_expression(p)){
        return false;
    }
    while(match(p, TOK_PLUS) || match(p, TOK_MINUS)){
        if(!multiplicative_expression(p)){
            return false;
        }
    }
    return true;
}

bool multiplicative_expression(Parser *p){
    if(!cast_expression(p)){
        return false;
    }
    while(match(p, TOK_STAR) || match(p, TOK_SLASH) || match(p, TOK_MOD)){
        if(!cast_expression(p)){
            return false;
        }
    }
    return true;
}

bool cast_expression(Parser *p){
    if(unary_expression(p)){
        return true;
    }
    if(match(p, TOK_LPAREN)){
        if(!type_name(p)){
            return false;
        }
        if(!match(p, TOK_RPAREN)){
            return false;
        }
        if(!cast_expression(p)){
            return false;
        }
    }
    return true;
}

bool unary_expression(Parser *p){
    if(postfix_expression(p)){
        return true;
    }
    if(match(p, TOK_INC)){
        if(!unary_expression(p)){
            return false;
        }
        return true;
    }
    if(match(p, TOK_DEC)){
        if(!unary_expression(p)){
            return false;
        }
        return true;
    }
    if(unary_operator(p)){
        if(!cast_expression(p)){
            return false;
        }
        return true;
    }
    if(match(p, TOK_KW_SIZEOF)){
        if(unary_expression(p)){
            return true;
        }
        if(match(p, TOK_LPAREN)){
            if(!type_name(p)){
                return false;
            }
            if(!match(p, TOK_RPAREN)){
                return false;
            }
        }
        return true;
    }
    return false;
}

bool postfix_expression(Parser *p){
    bool has_base_case = false;
    if(primary_expression(p)){
        has_base_case = true;
    }
    else if(match(p, TOK_LPAREN)){
        if(!type_name(p)){
            return false;
        }
        if(!match(p, TOK_RPAREN)){
            return false;
        }
        if(!match(p, TOK_LBRACE)){
            return false;
        }
        if(!initializer_list(p)){
            return false;
        }
        match(p, TOK_COMMA);
        if(!match(p, TOK_RBRACE)){
            return false;
        }
        has_base_case = true;
    }
    bool continue_recursion = true;
    while(continue_recursion){
        continue_recursion = false;
        if(match(p, TOK_LBRACKET)){
            if(!expression(p)){
                return false;
            }
            if(!match(p, TOK_RBRACKET)){
                return false;
            }
            has_base_case = true;
            continue_recursion = true;
        }
        else if(match(p, TOK_LPAREN)){
            argument_expression_list(p);
            if(!match(p, TOK_RPAREN)){
                return false;
            }
            has_base_case = true;
            continue_recursion = true;
        }
        else if(match(p, TOK_DOT)){
            if(!match(p, TOK_ID)){
                return false;
            }
            has_base_case = true;
            continue_recursion = true;
        }
        else if(match(p, TOK_ARROW)){
            if(!match(p, TOK_ID)){
                return false;
            }
            has_base_case = true;
            continue_recursion = true;
        }
        else if(match(p, TOK_INC)){
            has_base_case = true;
            continue_recursion = true;
        }
        else if(match(p, TOK_DEC)){
            has_base_case = true;
            continue_recursion = true;
        }
    }
    return has_base_case;
}

bool primary_expression(Parser *p){
    if(match(p, TOK_ID)){
        return true;
    }
    if(match(p, TOK_NUM_INT) || match(p, TOK_NUM_FLOAT)){
        return true;
    }
    if(match(p, TOK_LIT_CHAR) || match(p, TOK_LIT_STRING)){
        return true;
    }
    if(match(p, TOK_LPAREN)){
        if(!expression(p)){
            return false;
        }
        if(!match(p, TOK_RPAREN)){
            return false;
        }
        return true;
    }
    return false;
}

bool expression(Parser *p){
    if(assignment_expression(p)){
        return true;
    }
    while(match(p, TOK_COMMA)){
        if(!assignment_expression(p)){
            return false;
        }
    }
    return true;
}

bool argument_expression_list(Parser *p){
    if(assignment_expression(p)){
        return true;
    }
    while(match(p, TOK_COMMA)){
        if(!assignment_expression(p)){
            return false;
        }
    }
    return true;
}

bool type_name(Parser *p){
    if(!specifier_qualifier_list(p)){
        return false;
    }
    abstract_declarator(p);
    return true;
}

bool abstract_declarator(Parser *p){
    bool has_pointer = pointer(p);
    if(direct_abstract_declarator(p)){
        return true;
    }
    if(!has_pointer){
        return false;
    }
    return true;
}

bool direct_abstract_declarator(Parser *p){ 
    bool has_base_case = false;
    if(match(p, TOK_LPAREN)){
        if(abstract_declarator(p)){
            if(!match(p, TOK_RPAREN)){
                return false;
            }
            has_base_case = true;
        }
        else{
            parameter_list(p);
            if(!match(p, TOK_RPAREN)){
                return false;
            }
            has_base_case = true;
        }
    }
    bool continue_recursion = true;
    while(continue_recursion){
        continue_recursion = false;
        if(match(p, TOK_LBRACKET)){
            if(match(p, TOK_STAR));
            else if(match(p, TOK_KW_STATIC)){
                type_qualifier_list(p);
                if(!assignment_expression(p)){
                    return false;
                }
            }
            else{
                type_qualifier_list(p);
                if(match(p, TOK_KW_STATIC)){
                    if(!assignment_expression(p)){
                        return false;
                    }
                }
                else{
                    assignment_expression(p);
                }
            }
            if(!match(p, TOK_RBRACKET)){
                return false;
            }
            continue_recursion = true;
            has_base_case = true;
        }
        else if(match(p, TOK_LPAREN)){
            parameter_type_list(p);
            if(!match(p, TOK_RPAREN)){
                return false;
            }
            continue_recursion = true;
            has_base_case = true;
        }
    }
    return has_base_case;
}

bool parameter_type_list(Parser *p){
    if(!parameter_list(p)){
        return false;
    }
    while(match(p, TOK_COMMA)){
        if(!match(p, TOK_ELLIPSIS)){
            return false;
        }
    }
    return true;
}

bool parameter_list(Parser *p){
    if(!parameter_declaration(p)){
        return false;
    }
    while(match(p, TOK_COMMA)){
        if(!parameter_declaration(p)){
            return false;
        }
    }
    return true;
}

bool parameter_declaration(Parser *p){
    if(!declaration_specifiers(p)){
        return false;
    }
    if(declarator(p)){
        return true;
    }
    abstract_declarator(p);
    return true;
}

bool initializer_list(Parser *p){
    designation(p);
    if(!initializer(p)){
        return false;
    }
    while(match(p, TOK_COMMA)){
        designation(p);
        if(!initializer(p)){
            return false;
        }
    }
    return true;
}

bool designation(Parser *p){
    if(!designator_list(p)){
        return false;
    }
    if(!match(p, TOK_ASSIGN)){
        return false;
    }
    return true;
}

bool designator_list(Parser *p){
    if(!designator(p)){
        return false;
    }
    while(designator(p));
    return true;
}

bool designator(Parser *p){
    if(match(p, TOK_LBRACKET)){
        if(!constant_expression(p)){
            return false;
        }
        if(!match(p, TOK_RBRACKET)){
            return false;
        }
        return true;
    }
    if(match(p, TOK_DOT)){
        if(!match(p, TOK_ID)){
            return false;
        }
        return true;
    }
    return false;
}

bool constant_expression(Parser *p){
    if(!conditional_expression(p)){
        return false;
    }
    return true;
}

bool initializer(Parser *p){
    if(assignment_expression(p)){
        return true;
    }
    if(match(p, TOK_LBRACE)){
        if(!initializer_list(p)){
            return false;
        }
        match(p, TOK_COMMA);
        if(!match(p, TOK_RBRACE)){
            return false;
        }
        return true;
    }
    return false;
}

bool unary_operator(Parser *p){
    switch(get_current_token(p)){
        case TOK_BIT_AND:
        case TOK_STAR:
        case TOK_PLUS:
        case TOK_MINUS:
        case TOK_BIT_NOT:
        case TOK_LOGICAL_NOT:
            advance(p);
            return true;
    }
    return false;
}

bool assignment_operator(Parser *p){
    switch(get_current_token(p)){
        case TOK_ASSIGN:
        case TOK_STAR_ASSIGN:
        case TOK_SLASH_ASSIGN:
        case TOK_MOD_ASSIGN:
        case TOK_PLUS_ASSIGN:
        case TOK_MINUS_ASSIGN:
        case TOK_SHL_ASSIGN:
        case TOK_SHR_ASSIGN:
        case TOK_AND_ASSIGN:
        case TOK_OR_ASSIGN:
        case TOK_XOR_ASSIGN:
            advance(p);
            return true;
    }
    return false;
}

bool identifier_list(Parser *p){
    if(!identifier(p)){
        return false;
    }
    while(match(p, TOK_COMMA)){
        if(!identifier(p)){
            return false;
        }
    }
    return true;
}

bool enum_specifier(Parser *p){
    if(!match(p, TOK_KW_ENUM)){
        return false;
    }
    bool has_id = match(p, TOK_ID);
    if(match(p, TOK_LBRACE)){
        if(!enumerator_list(p)){
            return false;
        }
        match(p, TOK_COMMA);
        if(!match(p, TOK_RBRACE)){
            return false;
        }
        return true;
    }
    if(!has_id){
        return false;
    }
    return true;
}

bool enumerator_list(Parser *p){
    if(!enumarator(p)){
        return false;
    }
    while(match(p, TOK_COMMA)){
        if(!enumarator(p)){
            return false;
        }
    }
    return true;
}

bool enumarator(Parser *p){
    if(!match(p, TOK_ID)){
        return false;
    }
    if(match(p, TOK_ASSIGN)){
        if(!constant_expression(p)){
            return false;
        }
    }
    return true;
}

bool typedef_name(Parser *p){
    if(!match(p, TOK_ID)){
        return false;
    }
    return true;
}

bool function_specifier(Parser *p){
    if(!match(p, TOK_KW_INLINE)){
        return false;
    }
    return true;
}

bool declaration_list(Parser *p){
    if(!declaration(p)){
        return false;
    }
    while(declaration(p));
    return true;
}

bool declaration(Parser *p){
    if(!declaration_specifiers(p)){
        return false;
    }
    init_declarator_list(p);
    if(!match(p, TOK_SEMICOLON)){
        return false;
    }
    return true;    
}

bool init_declarator_list(Parser *p){
    if(!init_declarator(p)){
        return false;
    }
    while(match(p, TOK_COMMA)){
        if(!init_declarator(p)){
            return false;
        }
    }
    return true;
}

bool init_declarator(Parser *p){
    if(!declarator(p)){
        return false;
    }
    if(match(p, TOK_ASSIGN)){
        if(!initializer(p)){
            return false;
        }
    }
    return true;
}

bool compound_statement(Parser *p){
    if(match(p, TOK_LBRACE)){
        block_item_list(p);
        if(!match(p, TOK_RBRACE)){
            return false;
        }
        return true;
    }
    return false;
}

bool block_item_list(Parser *p){
    if(!block_item(p)){
        return false;
    }
    while(block_item(p));
    return true;
}

bool block_item(Parser *p){
    if(declaration(p)){
        return true;
    }
    if(statement(p)){
        return true;
    }
    return false;
}

bool statement(Parser *p){
    if(labeled_statement(p)){
        return true;
    }
    if(compound_statement(p)){
        return true;
    }
    if(expression_statement(p)){
        return true;
    }
    if(selection_statement(p)){
        return true;
    }
    if(iteration_statement(p)){
        return true;
    }
    if(jump_statement(p)){
        return true;
    }
    return false;
}

bool labeled_statement(Parser *p){
    if(match(p, TOK_ID)){
        if(!match(p, TOK_COLON)){
            return false;
        }
        if(!statement(p)){
            return false;
        }
        return true;
    }
    if(match(p, TOK_KW_CASE)){
        if(!constant_expression(p)){
            return false;
        }
        if(!match(p, TOK_COLON)){
            return false;
        }
        if(!statement(p)){
            return false;
        }
        return true;
    }
    if(match(p, TOK_KW_DEFAULT)){
        if(!match(p, TOK_COLON)){
            return false;
        }
        if(!statement(p)){
            return false;
        }
        return true;
    }
    return false;
}

bool expression_statement(Parser *p){
    expression(p);
    if(!match(p, TOK_SEMICOLON)){
        return false;
    }
    return true;
}

bool selection_statement(Parser *p){
    if(match(p, TOK_KW_IF)){
        if(!match(p, TOK_LPAREN)){
            return false;
        }
        if(!expression(p)){
            return false;
        }
        if(!match(p, TOK_RPAREN)){
            return false;
        }
        if(!statement(p)){
            return false;
        }
        if(match(p, TOK_KW_ELSE)){
            if(!statement(p)){
                return false;
            }
        }
        return true;
    }
    if(match(p, TOK_KW_SWITCH)){
        if(!match(p, TOK_LPAREN)){
            return false;
        }
        if(!expression(p)){
            return false;
        }
        if(!match(p, TOK_RPAREN)){
            return false;
        }
        if(!statement(p)){
            return false;
        }
        return true;
    }
    return false;
}

bool iteration_statement(Parser *p){
    if(match(p, TOK_KW_WHILE)){
        if(!match(p, TOK_LPAREN)){
            return false;
        }
        if(!expression(p)){
            return false;
        }
        if(!match(p, TOK_RPAREN)){
            return false;
        }
        if(!statement(p)){
            return false;
        }
        return true;
    }
    if(match(p, TOK_KW_DO)){
        if(!statement(p)){
            return false;
        }
        if(!match(p, TOK_KW_WHILE)){
            return false;
        }
        if(!match(p, TOK_LPAREN)){
            return false;
        }
        if(!expression(p)){
            return false;
        }
        if(!match(p, TOK_RPAREN)){
            return false;
        }
        if(!statement(p)){
            return false;
        }
        if(!match(p, TOK_SEMICOLON)){
            return false;
        }
        return true;
    }
    if(match(p, TOK_KW_FOR)){
        if(!match(p, TOK_LPAREN)){
            return false;
        }
        if(!declaration(p)){
            expression(p);
            if(!match(p, TOK_SEMICOLON)){
                return false;
            }
            expression(p);
            if(!match(p, TOK_SEMICOLON)){
                return false;
            }
            expression(p);
        }
        else{
            expression(p);
            if(!match(p, TOK_SEMICOLON)){
                return false;
            }
            expression(p);
        }
        if(!match(p, TOK_RPAREN)){
            return false;
        }
        if(!statement(p)){
            return false;
        }
        return true;
    }
    return false;
}

bool jump_statement(Parser *p){
    if(match(p, TOK_KW_GOTO)){
        if(!match(p, TOK_ID)){
            return false;
        }
        if(!match(p, TOK_SEMICOLON)){
            return false;
        }
        return true;
    }
    if(match(p, TOK_KW_CONTINUE)){
        if(!match(p, TOK_SEMICOLON)){
            return false;
        }
        return true;
    }
    if(match(p, TOK_KW_BREAK)){
        if(!match(p, TOK_SEMICOLON)){
            return false;
        }
        return true;
    }
    if(match(p, TOK_KW_RETURN)){
        expression(p);
        if(!match(p, TOK_SEMICOLON)){
            return false;
        }
        return true;
    }
    return false;
}