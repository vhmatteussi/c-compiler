#include "parser.h"

Token *get_current_token(Parser *p){
    return p->current_token;
}

void get_next_token(Parser *p){
    p->current_token = next_token(p->l);
}

bool match(Parser *p, TokenType type_to_match){
    if(get_current_token(p)->type == type_to_match){
        get_next_token(p);
        return true;
    }
    return false;
}

Node *translation_unit(Parser *p){
    NodesList *list = NULL;
    while(get_current_token(p)->type != TOK_EOF){
        Node *element = external_declaration(p);
        if(!element){
            return NULL;
        }
        push_node_list(p, &list, element);
    }
    if(list == NULL){
        return NULL;
    }
    return ast_list_solo(p, NODE_TRANSLATION_UNIT, *list);
}

Node *external_declaration(Parser *p){
    Node *child = function_definition(p);
    if(child){
        return child;
    }
    child = declaration(p);
    if(child){
        return child;
    }
    return NULL;
}

Node *function_definition(Parser *p){
    Node *dec_specifiers = declaration_specifiers(p);
    if(!dec_specifiers){
        return NULL;
    }
    Node *dec = declarator(p);
    if(!dec){
        return NULL;
    }
    Node *dec_list = declaration_list(p);
    Node *st = compound_statement(p);
    if(!st){
        return NULL;
    }
    return ast_node_quartet(p, NODE_FUNCTION_DEF, dec_specifiers, dec, dec_list, st);
}

Node *declaration_specifiers(Parser *p){
    NodesList *list = NULL;
    Node *spec;
    while(true){
        if(spec = storage_class_specifiers(p));
        else if(spec = type_specifier(p));
        else if(spec = type_qualifier(p));
        else if(spec = function_specifier(p));
        else{
            break;
        }
        nodelist_push(p, &list, spec);
    }
    if(list == NULL){
        return NULL;
    }
    return node_list_solo(p, NODE_DECLARATION_SPECIFIERS, *list);
}

Node *storage_class_specifiers(Parser *p){
    Token *tok = get_current_token(p);
    switch(tok->type){
        case TOK_KW_TYPEDEF:
        case TOK_KW_EXTERN:
        case TOK_KW_STATIC:
        case TOK_KW_AUTO:
        case TOK_KW_REGISTER:
            get_next_token(p);
            return ast_token_solo(p, NODE_STORAGE_CLASS, tok);
        default:
            return NULL;
    }
    return NULL;
}

Node *type_specifier(Parser *p){
    Token *tok = get_current_token(p);
    switch(tok->type){
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
            get_next_token(p);
            return ast_token_solo(p, NODE_TYPE_SPECIFIER, tok);
        default: 
            break;
    }
    if(tok = struct_or_union_specifier(p));
    else if(tok = enum_specifier(p));
    else if(tok = typedef_name(p));
    if(tok){
        return ast_token_solo(p, NODE_TYPE_SPECIFIER, tok);
    }
    return NULL;
}

Node *struct_or_union_specifier(Parser *p){
    if(!struct_or_union(p)){
        return NULL;
    }

    bool has_id = match(p, TOK_ID);
    if(match(p, TOK_LBRACE)){
        if(!struct_declaration_list(p)){
            return NULL;
        }
        if(!match(p, TOK_RBRACE)){
            return NULL;
        }
        return true;
    }
    if(!has_id){
        return NULL;
    }
    return true;
}

Node *struct_or_union(Parser *p){
    Token *tok = get_current_token(p);
    switch(tok->type){
        case TOK_KW_STRUCT:
        case TOK_KW_UNION:
            get_next_token(p);
            return ast_token_solo(p, NODE_TYPE_SPECIFIER, tok);
        default:
            return NULL;
    }
}

Node *struct_declaration_list(Parser *p){
    if(!struct_declaration(p)){
        return NULL;
    }
    while(struct_declaration(p));
    return true;
}

Node *struct_declaration(Parser *p){
    if(!specifier_qualifier_list(p)){
        return NULL;
    }
    if(!struct_declarator_list(p)){
        return NULL;
    }
    if(!match(p, TOK_SEMICOLON)){
        return NULL;
    }
    return true;
}

Node *specifier_qualifier_list(Parser *p){
    if(type_specifier(p)){
        specifier_qualifier_list(p);
        return true;
    }
    if(type_qualifier(p)){
        specifier_qualifier_list(p);
        return true;
    }
    return NULL;
}

Node *type_qualifier(Parser *p){
    Token *tok = get_current_token(p);
    switch(tok->type){
        case TOK_KW_CONST:
        case TOK_KW_RESTRICT:
        case TOK_KW_VOLATILE:
            get_next_token(p);
            ast_token_solo(p, NODE_TYPE_QUALIFIER, tok);
        default:
            return NULL;
    }
    return NULL;
}

Node *struct_declarator_list(Parser *p){
    if(!struct_declarator(p)){
        return NULL;
    }
    while(match(p, TOK_COMMA)){
        if(!struct_declarator(p)){
            return NULL;
        }
    }
    return true;
}

Node *struct_declarator(Parser *p){
    bool has_declarator = declarator(p);
    if(match(p, TOK_COLON)){
        if(!constant_expression(p)){
            return NULL;
        }
        return true;
    }
    if(!has_declarator){
        return NULL;
    }
    return true;
}

Node *declarator(Parser *p){
    Node *ptr = pointer(p);
    Node *direct_dec = direct_declarator(p);
    if(!direct_dec){
        return NULL;
    }
    return ast_node_duo(p, NODE_DECLARATOR, ptr, direct_dec);
}

Node *pointer(Parser *p){
    if(!match(p, TOK_STAR)){
        return NULL;
    }
    type_qualifier_list(p);
    pointer(p);
    return true;
}

Node *type_qualifier_list(Parser *p){
    NodesList *list = NULL;
    Node *element = type_qualifier(p);
    if(!element){
        return NULL;
    }
    while(element){
        push_node_list(p, &list, element);
        element = type_qualifier(p);
    }
    return ast_list_solo(p, NODE_TYPE_QUALIFIER, *list);
}

Node *direct_declarator(Parser *p){
    bool has_base_case = false;
    if(match(p, TOK_ID)){
        has_base_case = true;
    }
    else if(match(p, TOK_LPAREN)){
        if(!declarator(p)){
            return NULL;
        }
        if(!match(p, TOK_RPAREN)){
            return NULL;
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
                    return NULL;
                }
            }
            else if(type_qualifier_list(p)){
                if(match(p, TOK_KW_STATIC)){
                    if(!assignment_expression(p)){
                        return NULL;
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
                return NULL;
            }
            continue_recursion = true;
            has_base_case = true;
        }
        else if(match(p, TOK_LPAREN)){
            if(!parameter_type_list(p)){
                identifier_list(p);
            }
            if(!match(p, TOK_RPAREN)){
                return NULL;
            }
            continue_recursion = true;
            has_base_case = true;
        }
    }
    return has_base_case;
}

Node *assignment_expression(Parser *p){
    if(conditional_expression(p)){
        return true;
    }
    if(unary_expression(p)){
        if(!assignment_operator(p)){
            return NULL;
        }
        if(!assignment_expression(p)){
            return NULL;
        }
        return true;
    }
    return NULL;
}

Node *conditional_expression(Parser *p){
    if(!logical_or_expression(p)){
        return NULL;
    }
    if(match(p, TOK_QUESTION)){
        if(!expression(p)){
            return NULL;
        }
        if(!match(p, TOK_COLON)){
            return NULL;
        }
        if(!conditional_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *logical_or_expression(Parser *p){
    if(!logical_and_expression(p)){
        return NULL;
    }
    while(match(p, TOK_LOGICAL_OR)){
        if(!logical_and_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *logical_and_expression(Parser *p){
    if(!inclusive_or_expression(p)){
        return NULL;
    }
    while(match(p, TOK_LOGICAL_AND)){
        if(!inclusive_or_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *inclusive_or_expression(Parser *p){
    if(!exclusive_or_expression(p)){
        return NULL;
    }
    while(match(p, TOK_BIT_OR)){
        if(!exclusive_or_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *exclusive_or_expression(Parser *p){
    if(!and_expression(p)){
        return NULL;
    }
    while(match(p, TOK_BIT_XOR)){
        if(!and_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *and_expression(Parser *p){
    if(!equality_expression(p)){
        return NULL;
    }
    while(match(p, TOK_BIT_AND)){
        if(!equality_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *equality_expression(Parser *p){
    if(!relational_expression(p)){
        return NULL;
    }
    while(match(p, TOK_EQUAL_EQUAL) || match(p,TOK_NOT_EQUAL)){
        if(!relational_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *relational_expression(Parser *p){
    if(!shift_expression(p)){
        return NULL;
    }
    while(match(p, TOK_LESS) || match(p, TOK_GREATER) || match(p, TOK_LESS_EQUAL) || match(p, TOK_GREATER_EQUAL)){
        if(!shift_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *shift_expression(Parser *p){
    if(!additive_expression(p)){
        return NULL;
    }
    while(match(p, TOK_SHIFT_LEFT) || match(p, TOK_SHIFT_RIGHT)){
        if(!additive_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *additive_expression(Parser *p){
    if(!multiplicative_expression(p)){
        return NULL;
    }
    while(match(p, TOK_PLUS) || match(p, TOK_MINUS)){
        if(!multiplicative_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *multiplicative_expression(Parser *p){
    if(!cast_expression(p)){
        return NULL;
    }
    while(match(p, TOK_STAR) || match(p, TOK_SLASH) || match(p, TOK_MOD)){
        if(!cast_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *cast_expression(Parser *p){
    if(unary_expression(p)){
        return true;
    }
    if(match(p, TOK_LPAREN)){
        if(!type_name(p)){
            return NULL;
        }
        if(!match(p, TOK_RPAREN)){
            return NULL;
        }
        if(!cast_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *unary_expression(Parser *p){
    if(postfix_expression(p)){
        return true;
    }
    if(match(p, TOK_INC)){
        if(!unary_expression(p)){
            return NULL;
        }
        return true;
    }
    if(match(p, TOK_DEC)){
        if(!unary_expression(p)){
            return NULL;
        }
        return true;
    }
    if(unary_operator(p)){
        if(!cast_expression(p)){
            return NULL;
        }
        return true;
    }
    if(match(p, TOK_KW_SIZEOF)){
        if(unary_expression(p)){
            return true;
        }
        if(match(p, TOK_LPAREN)){
            if(!type_name(p)){
                return NULL;
            }
            if(!match(p, TOK_RPAREN)){
                return NULL;
            }
        }
        return true;
    }
    return NULL;
}

Node *postfix_expression(Parser *p){
    Node *base = primary_expression(p);
    if(!base){
        if(!match(p, TOK_LPAREN)){
            return NULL;
        }
        Node *tn = type_name(p);
        if(!tn){
            return NULL;
        }
        if(!match(p, TOK_RPAREN)){
            return NULL;
        }
        if(!match(p, TOK_LBRACE)){
            return NULL;
        }
        Node *init = initializer_list(p);
        if(!init){
            return NULL;
        }
        match(p, TOK_COMMA);
        if(!match(p, TOK_RBRACE)){
            return NULL;
        }
        base = ast_node_solo(p, NODE_INITIALIZER, init);
    }
    while(true){
        if(match(p, TOK_LBRACKET)){
            Node *idx = expression(p);
            if(!idx){
                return NULL;
            }
            if(!match(p, TOK_RBRACKET)){
                return NULL;
            }
            base = ast_node_duo(p, NODE_EXP_INDEX, base, idx);
        }
        else if(match(p, TOK_LPAREN)){
            Node *args = argument_expression_list(p);
            if(!match(p, TOK_RPAREN)){
                return NULL;
            }
            base = ast_node_duo(p, NODE_EXP_CALL, base, args);
        }
        else if(match(p, TOK_DOT)){
            if(get_current_token(p)->type != TOK_ID){
                return NULL;
            }
            Token *member = get_current_token(p);
            get_next_token(p);
            base = ast_token_node_solo(p, NODE_EXP_MEMBER, member, base);
        }
        else if(match(p, TOK_ARROW)){
            if(get_current_token(p)->type != TOK_ID){
                return NULL;
            }
            Token *member = get_current_token(p);
            get_next_token(p);
            base = ast_token_node_solo(p, NODE_EXP_ARROW, member, base);
        }
        else if(match(p, TOK_INC)){
            base = ast_node_solo(p, NODE_EXP_POST_INC, base);
        }
        else if(match(p, TOK_DEC)){
            base = ast_node_solo(p, NODE_EXP_POST_DEC, base);
        }
        else{
            break;
        }
    }
    return base;
}

Node *primary_expression(Parser *p){
    Token *tok = p->current_token;
    switch(tok->type){
        case TOK_ID:
            get_next_token(p);
            return ast_token_solo(p, NODE_EXP_IDENTIFIER, tok);
        case TOK_NUM_INT:
            get_next_token(p);
            return ast_token_solo(p, NODE_NUM_INT, tok);
        case TOK_NUM_FLOAT:
            get_next_token(p);
            return ast_token_solo(p, NODE_NUM_FLOAT, tok);
        case TOK_LIT_CHAR:
            get_next_token(p);
            return ast_token_solo(p, NODE_LIT_CHAR, tok);
        case TOK_LIT_STRING:
            get_next_token(p);
            return ast_token_solo(p, NODE_LIT_STRING, tok);
        case TOK_LPAREN:
            get_next_token(p);
            Node *exp = expression(p);
            if(!exp){
                return NULL;
            }
            if(!match(p, TOK_RPAREN)){
                return NULL;
            }
            return exp;
        default:
            return NULL;
    }
}

Node *expression(Parser *p){
    if(assignment_expression(p)){
        return true;
    }
    while(match(p, TOK_COMMA)){
        if(!assignment_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *argument_expression_list(Parser *p){
    if(assignment_expression(p)){
        return true;
    }
    while(match(p, TOK_COMMA)){
        if(!assignment_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *type_name(Parser *p){
    if(!specifier_qualifier_list(p)){
        return NULL;
    }
    abstract_declarator(p);
    return true;
}

Node *abstract_declarator(Parser *p){
    bool has_pointer = pointer(p);
    if(direct_abstract_declarator(p)){
        return true;
    }
    if(!has_pointer){
        return NULL;
    }
    return true;
}

Node *direct_abstract_declarator(Parser *p){ 
    bool has_base_case = false;
    if(match(p, TOK_LPAREN)){
        if(abstract_declarator(p)){
            if(!match(p, TOK_RPAREN)){
                return NULL;
            }
            has_base_case = true;
        }
        else{
            parameter_list(p);
            if(!match(p, TOK_RPAREN)){
                return NULL;
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
                    return NULL;
                }
            }
            else{
                type_qualifier_list(p);
                if(match(p, TOK_KW_STATIC)){
                    if(!assignment_expression(p)){
                        return NULL;
                    }
                }
                else{
                    assignment_expression(p);
                }
            }
            if(!match(p, TOK_RBRACKET)){
                return NULL;
            }
            continue_recursion = true;
            has_base_case = true;
        }
        else if(match(p, TOK_LPAREN)){
            parameter_type_list(p);
            if(!match(p, TOK_RPAREN)){
                return NULL;
            }
            continue_recursion = true;
            has_base_case = true;
        }
    }
    return has_base_case;
}

Node *parameter_type_list(Parser *p){
    if(!parameter_list(p)){
        return NULL;
    }
    while(match(p, TOK_COMMA)){
        if(!match(p, TOK_ELLIPSIS)){
            return NULL;
        }
    }
    return true;
}

Node *parameter_list(Parser *p){
    if(!parameter_declaration(p)){
        return NULL;
    }
    while(match(p, TOK_COMMA)){
        if(!parameter_declaration(p)){
            return NULL;
        }
    }
    return true;
}

Node *parameter_declaration(Parser *p){
    if(!declaration_specifiers(p)){
        return NULL;
    }
    if(declarator(p)){
        return true;
    }
    abstract_declarator(p);
    return true;
}

Node *initializer_list(Parser *p){
    designation(p);
    if(!initializer(p)){
        return NULL;
    }
    while(match(p, TOK_COMMA)){
        designation(p);
        if(!initializer(p)){
            return NULL;
        }
    }
    return true;
}

Node *designation(Parser *p){
    if(!designator_list(p)){
        return NULL;
    }
    if(!match(p, TOK_ASSIGN)){
        return NULL;
    }
    return true;
}

Node *designator_list(Parser *p){
    if(!designator(p)){
        return NULL;
    }
    while(designator(p));
    return true;
}

Node *designator(Parser *p){
    if(match(p, TOK_LBRACKET)){
        if(!constant_expression(p)){
            return NULL;
        }
        if(!match(p, TOK_RBRACKET)){
            return NULL;
        }
        return true;
    }
    if(match(p, TOK_DOT)){
        if(!match(p, TOK_ID)){
            return NULL;
        }
        return true;
    }
    return NULL;
}

Node *constant_expression(Parser *p){
    if(!conditional_expression(p)){
        return NULL;
    }
    return true;
}

Node *initializer(Parser *p){
    if(assignment_expression(p)){
        return true;
    }
    if(match(p, TOK_LBRACE)){
        if(!initializer_list(p)){
            return NULL;
        }
        match(p, TOK_COMMA);
        if(!match(p, TOK_RBRACE)){
            return NULL;
        }
        return true;
    }
    return NULL;
}

Node *unary_operator(Parser *p){
    Token *tok = get_current_token(p);
    switch(tok->type){
        case TOK_BIT_AND:
        case TOK_STAR:
        case TOK_PLUS:
        case TOK_MINUS:
        case TOK_BIT_NOT:
        case TOK_LOGICAL_NOT:
            get_next_token(p);
            return ast_token_solo(p, NODE_EXP_UNARY, tok);
        default:
            return NULL;
    }
    return NULL;
}

Node *assignment_operator(Parser *p){
    Token *tok = get_current_token(p);
    switch(tok->type){
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
            get_next_token(p);
            return ast_token_solo(p, NODE_EXP_ASSIGN, tok);
        default:
            return NULL;
    }
    return NULL;
}

Node *identifier_list(Parser *p){

    if(!match(p, TOK_ID)){
        return NULL;
    }
    while(match(p, TOK_COMMA)){
        if(!match(p, TOK_ID)){
            return NULL;
        }
    }
    return true;
}

Node *enum_specifier(Parser *p){
    if(!match(p, TOK_KW_ENUM)){
        return NULL;
    }
    bool has_id = match(p, TOK_ID);
    if(match(p, TOK_LBRACE)){
        if(!enumerator_list(p)){
            return NULL;
        }
        match(p, TOK_COMMA);
        if(!match(p, TOK_RBRACE)){
            return NULL;
        }
        return true;
    }
    if(!has_id){
        return NULL;
    }
    return true;
}

Node *enumerator_list(Parser *p){
    if(!enumarator(p)){
        return NULL;
    }
    while(match(p, TOK_COMMA)){
        if(!enumarator(p)){
            return NULL;
        }
    }
    return true;
}

Node *enumarator(Parser *p){
    if(!match(p, TOK_ID)){
        return NULL;
    }
    if(match(p, TOK_ASSIGN)){
        if(!constant_expression(p)){
            return NULL;
        }
    }
    return true;
}

Node *typedef_name(Parser *p){
    if(match(p, TOK_ID)){
        Node *id = get_current_token(p);
        return id;
    }
    return NULL;
}

Node *function_specifier(Parser *p){
    Token *tok = get_current_token(p);
    if(tok->type == TOK_KW_INLINE){
        return ast_token_solo(p, NODE_FUNCTION_SPECIFIER, tok);
    }
    return NULL;
}

Node *declaration_list(Parser *p){
    NodesList *list = NULL;
    Node *element = declaration(p);
    if(!element){
        return NULL;
    }
    while(element){
        push_node_list(p, &list, element);
        element = declaration(p);
    }
    return ast_list_solo(p, NODE_DECLARATION, *list);
}

Node *declaration(Parser *p){
    Node *dec_specifiers = declaration_specifiers(p);
    if(!dec_specifiers){
        return NULL;
    }
    Node *init_dec_list = init_declarator_list(p);
    if(!match(p, TOK_SEMICOLON)){
        return NULL;
    }
    return ast_node_duo(p, NODE_DECLARATION, dec_specifiers, init_dec_list);
}

Node *init_declarator_list(Parser *p){
    NodesList *list = NULL;
    Node *element = init_declarator(p);
    if(!element){
        return NULL;
    }
    get_next_token(p);
    while(match(p, TOK_COMMA)){
        push_node_list(p, &list, element);
        element = init_declarator(p);
        if(!element){
            return NULL;
        }
    }
    return ast_list_solo(p, NODE_INIT_DECLARATOR_LIST, *list);
}

Node *init_declarator(Parser *p){
    Node *dec = declarator(p);
    if(!dec){
        return NULL;
    }
    Node *init = NULL;
    if(match(p, TOK_ASSIGN)){
        init = initializer(p);
        if(!init){
            return NULL;
        }
    }
    return ast_node_duo(p, NODE_INIT_DECLARATOR, dec, init);
}

Node *compound_statement(Parser *p){
    if(match(p, TOK_LBRACE)){
        block_item_list(p);
        if(!match(p, TOK_RBRACE)){
            return NULL;
        }
        return true;
    }
    return NULL;
}

Node *block_item_list(Parser *p){
    NodesList *list = NULL;
    Node *element = block_item(p);
    if(!element){
        return NULL;
    }
    while(element){
        push_node_list(p, &list, element);
        element = block_item(p);
    }
    return ast_list_solo(p, NODE_ST_COMPOUND, *list);
}

Node *block_item(Parser *p){
    Node *child = declaration(p);
    if(child){
        return child;
    }
    Node *child = statement(p);
    if(child){
        return child;
    }
    return NULL;
}

Node *statement(Parser *p){
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
    return NULL;
}

Node *labeled_statement(Parser *p){
    if(match(p, TOK_ID)){
        Token *label = get_current_token(p);
        if(!match(p, TOK_COLON)){
            return NULL;
        }
        Node *st = statement(p);
        if(!st){
            return NULL;
        }
        return ast_token_node_solo(p, NODE_ST_LABEL, label, st);
    }
    if(match(p, TOK_KW_CASE)){
        Node *exp = constant_expression(p);
        if(!exp){
            return NULL;
        }
        if(!match(p, TOK_COLON)){
            return NULL;
        }
        Node *st = statement(p);
        if(!st){
            return NULL;
        }
        return ast_node_duo(p, NODE_ST_CASE, exp, st);
    }
    if(match(p, TOK_KW_DEFAULT)){
        if(!match(p, TOK_COLON)){
            return NULL;
        }
        Node *st = statement(p);
        if(!st){
            return NULL;
        }
        return ast_node_solo(p, NODE_ST_DEFAULT, st);
    }
    return NULL;
}

Node *expression_statement(Parser *p){
    Node *exp = expression(p);
    if(!match(p, TOK_SEMICOLON)){
        return NULL;
    }
    return ast_node_solo(p, NODE_ST_EXPR, exp);
}

Node *selection_statement(Parser *p){
    if(match(p, TOK_KW_IF)){
        if(!match(p, TOK_LPAREN)){
            return NULL;
        }
        Node *exp = expression(p);
        if(!exp){
            return NULL;
        }
        if(!match(p, TOK_RPAREN)){
            return NULL;
        }
        Node *st = statement(p);
        if(!st){
            return NULL;
        }
        Node *st_else = NULL;
        if(match(p, TOK_KW_ELSE)){
            st_else = statement(p);
            if(!st_else){
                return NULL;
            }
        }
        return ast_node_trio(p, NODE_ST_IF, exp, st, st_else);
    }
    if(match(p, TOK_KW_SWITCH)){
        if(!match(p, TOK_LPAREN)){
            return NULL;
        }
        Node *exp = expression(p); 
        if(!exp){
            return NULL;
        }
        if(!match(p, TOK_RPAREN)){
            return NULL;
        }
        Node *st = statement(p);
        if(!st){
            return NULL;
        }
        return ast_node_duo(p, NODE_ST_SWITCH, exp, st);
    }
    return NULL;
}

Node *iteration_statement(Parser *p){
    if(match(p, TOK_KW_WHILE)){
        if(!match(p, TOK_LPAREN)){
            return NULL;
        }
        Node *exp = expression(p);
        if(!exp){
            return NULL;
        }
        if(!match(p, TOK_RPAREN)){
            return NULL;
        }
        Node *st = statement(p);
        if(!st){
            return NULL;
        }
        return ast_node_duo(p, NODE_ST_WHILE, exp, st);
    }
    if(match(p, TOK_KW_DO)){
        Node *st = statement(p);
        if(!st){
            return NULL;
        }
        if(!match(p, TOK_KW_WHILE)){
            return NULL;
        }
        if(!match(p, TOK_LPAREN)){
            return NULL;
        }
        Node *exp = expression(p);
        if(!exp){
            return NULL;
        }
        if(!match(p, TOK_RPAREN)){
            return NULL;
        }
        if(!match(p, TOK_SEMICOLON)){
            return NULL;
        }
        return ast_node_duo(p, NODE_ST_DO_WHILE, st, exp);
    }
    if(match(p, TOK_KW_FOR)){
        if(!match(p, TOK_LPAREN)){
            return NULL;
        }
        Node *dec_or_exp = declaration(p);
        Node *exp_cond = NULL;
        Node *exp_post = NULL; 
        if(!declaration(p)){
            dec_or_exp = expression(p);
            if(!match(p, TOK_SEMICOLON)){
                return NULL;
            }
            exp_cond = expression(p);
            if(!match(p, TOK_SEMICOLON)){
                return NULL;
            }
            exp_post = expression(p);
        }
        else{
            exp_cond = expression(p);
            if(!match(p, TOK_SEMICOLON)){
                return NULL;
            }
            exp_post = expression(p);
        }
        if(!match(p, TOK_RPAREN)){
            return NULL;
        }
        Node *st = statement(p);
        if(!st){
            return NULL;
        }
        return ast_node_quartet(p, NODE_ST_FOR, dec_or_exp, exp_cond, exp_post, st);
    }
    return NULL;
}

Node *jump_statement(Parser *p){
    if(match(p, TOK_KW_GOTO)){
        Token *label = p->current_token;
        if(!match(p, TOK_ID)){
            return NULL;
        }
        if(!match(p, TOK_SEMICOLON)){
            return NULL;
        }
        return ast_token_solo(p, NODE_ST_GOTO, label);
    }
    if(match(p, TOK_KW_CONTINUE)){
        if(!match(p, TOK_SEMICOLON)){
            return NULL;
        }
        return init_node(p, NODE_ST_CONTINUE);
    }
    if(match(p, TOK_KW_BREAK)){
        if(!match(p, TOK_SEMICOLON)){
            return NULL;
        }
        return init_node(p, NODE_ST_BREAK);
    }
    if(match(p, TOK_KW_RETURN)){
        Node *exp = expression(p);
        if(!match(p, TOK_SEMICOLON)){
            return NULL;
        }
        return ast_node_solo(p, NODE_ST_RETURN, exp);
    }
    return NULL;
}