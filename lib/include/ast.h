#include "parser.h"

typedef struct Parser Parser;

// Read {main}/other_and_docs/ast_call_list.md

typedef enum{
    // Other Stuff
    NODE_TRANSLATION_UNIT,
    NODE_FUNCTION_DEF,
    NODE_DECLARATION,
    NODE_DECLARATION_SPECIFIERS,
    NODE_INIT_DECLARATOR,
    NODE_INIT_DECLARATOR_LIST,
    // Specifiers
    NODE_TYPE_SPECIFIER,
    NODE_STORAGE_CLASS,
    NODE_TYPE_QUALIFIER,
    NODE_FUNCTION_SPECIFIER,
    // Structs and shit
    NODE_STRUCT_SPECIFIER,
    NODE_UNION_SPECIFIER,
    NODE_STRUCT_DECLARATION,
    NODE_STRUCT_DECLARATOR,
    NODE_ENUM_SPECIFIER,
    NODE_ENUMERATOR,
    // Declarators
    NODE_DECLARATOR,
    NODE_DIRECT_DECLARATOR,
    NODE_POINTER,
    NODE_ABSTRACT_DECLARATOR,
    NODE_PARAM_LIST,
    NODE_PARAM_DECLARATION,
    // Constants
    NODE_EXP_IDENTIFIER,
    NODE_NUM_INT,
    NODE_NUM_FLOAT,
    NODE_LIT_CHAR,
    NODE_LIT_STRING,
    // Expressions and ops
    NODE_EXP_BINARY,
    NODE_EXP_UNARY,
    NODE_EXP_TERNARY,
    NODE_EXP_CAST,
    NODE_EXP_CALL,
    NODE_EXP_INDEX,
    NODE_EXP_MEMBER,
    NODE_EXP_ARROW,
    NODE_EXP_ASSIGN,
    NODE_EXP_SIZEOF,
    NODE_EXP_POST_INC,
    NODE_EXP_POST_DEC,
    NODE_EXP_PRE_INC,
    NODE_EXP_PRE_DEC,
    NODE_EXP_COMMA,
    // Statements
    NODE_ST_COMPOUND,
    NODE_ST_EXPR,
    NODE_ST_IF,
    NODE_ST_SWITCH,
    NODE_ST_WHILE,
    NODE_ST_DO_WHILE,
    NODE_ST_FOR,
    NODE_ST_GOTO,
    NODE_ST_CONTINUE,
    NODE_ST_BREAK,
    NODE_ST_RETURN,
    NODE_ST_LABEL,
    NODE_ST_CASE,
    NODE_ST_DEFAULT,
    // Initializers
    NODE_INITIALIZER,
    NODE_INITIALIZER_LIST,
    NODE_DESIGNATION,
} NodeType;

typedef struct NodesList{
    Node *node;
    NodesList *next;
} NodesList;

typedef struct Node{
    NodeType node_type;
    Token *tok;
    struct{
        Node *NodeSolo;
        NodesList ListSolo;
        Token *TokenSolo;
        struct{
            Node *node_a;
            Node *node_b;
        } NodeDuo;
        struct{
            Node *node_a;
            Node *node_b;
            Node *node_c;
        } NodeTrio;
        struct{
            Node *node_a;
            Node *node_b;
            Node *node_c;
            Node *node_d;
        } NodeQuartet;
        struct{
            TokenType type;
            Node *node;
        } TypeNode;
        struct{
            TokenType type;
            Node *node_a;
            Node *node_b;
        } TypeNodeDuo;
        struct{
            Token *token;
            Node *node;
        } TokenNodeSolo;
        struct{
            Token *token;
            Node *node_a;
            Node *node_b;
        } TokenNodeDuo;
        struct{
            Token *token;
            NodesList list;
        }TokenList;
        struct{
            NodesList list;
            Node *node;
        } ListNodeSolo;
        struct{
            NodesList list_a;
            NodesList list_b;
        } ListDuo;
    };
} Node;

void init_node_list(NodesList *list);
void push_node_list(Parser *p, NodesList **list, Node *node);
Node* init_node(Parser *p, NodeType type);

Node *ast_node_solo(Parser *p, NodeType type, Node *node);
Node *ast_node_duo(Parser *p, NodeType type, Node *node_a, Node *node_b);
Node *ast_node_trio(Parser *p, NodeType type, Node *node_a, Node *node_b, Node *node_c);
Node *ast_node_quartet(Parser *p, NodeType type, Node *node_a, Node *node_b, Node *node_c, Node *node_d);

Node *ast_type_node(Parser *p, NodeType type, TokenType token_type, Node *node);
Node *ast_type_node_duo(Parser *p, NodeType type, TokenType token_type, Node *node_a, Node *node_b);

Node *ast_token_solo(Parser *p, NodeType type, Token *token);
Node *ast_token_node_solo(Parser *p, NodeType type, Token *token, Node *node);
Node *ast_token_node_duo(Parser *p, NodeType type, Token *token, Node *node_a, Node *node_b);
Node *ast_token_list(Parser *p, NodeType type, Token *token, NodesList list);

Node *ast_list_solo(Parser *p, NodeType type, NodesList list);
Node *ast_list_node_solo(Parser *p, NodeType type, NodesList list, Node *node);
Node *ast_list_duo(Parser *p, NodeType type, NodesList list_a, NodesList list_b);