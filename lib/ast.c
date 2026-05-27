#include "ast.h"

void init_node_list(NodesList *list){
    list->node = NULL;
    list->next = NULL;
}

void push_node_list(Parser *p, NodesList **list, Node *node){
    NodesList *entry = arena_malloc(p->arena, sizeof(NodesList));
    entry->node = node;
    entry->next = *list;
    *list = entry;
}

Node* init_node(Parser *p, NodeType type){
    Node *node = arena_calloc(p->arena, 1, sizeof(Node));
    node->node_type = type;
    node->tok = p->current_token;
    return node;
}

Node *ast_node_solo(Parser *p, NodeType type, Node *node){
    Node *n = init_node(p, type);
    n->NodeSolo = node;
    return n;
}

Node *ast_node_duo(Parser *p, NodeType type, Node *node_a, Node *node_b){
    Node *n = init_node(p, type);
    n->NodeDuo.node_a = node_a;
    n->NodeDuo.node_b = node_b;
    return n;
}

Node *ast_node_trio(Parser *p, NodeType type, Node *node_a, Node *node_b, Node *node_c){
    Node *n = init_node(p, type);
    n->NodeTrio.node_a = node_a;
    n->NodeTrio.node_b = node_b;
    n->NodeTrio.node_c = node_c;
    return n;
}

Node *ast_node_quartet(Parser *p, NodeType type, Node *node_a, Node *node_b, Node *node_c, Node *node_d){
    Node *n = init_node(p, type);
    n->NodeQuartet.node_a = node_a;
    n->NodeQuartet.node_b = node_b;
    n->NodeQuartet.node_c = node_c;
    n->NodeQuartet.node_d = node_d;
    return n;
}

Node *ast_type_node(Parser *p, NodeType type, TokenType token_type, Node *node){
    Node *n = init_node(p, type);
    n->TypeNode.node = node;
    n->TypeNode.type = token_type;
    return n;
}

Node *ast_type_node_duo(Parser *p, NodeType type, TokenType token_type, Node *node_a, Node *node_b){
    Node *n = init_node(p, type);
    n->TypeNodeDuo.node_a = node_a;
    n->TypeNodeDuo.node_b = node_b;
    n->TypeNodeDuo.type = token_type;
    return n;
}

Node *ast_token_solo(Parser *p, NodeType type, Token *token){
    Node *n = init_node(p, type);
    n->TokenSolo = token;
    return n;
}

Node *ast_token_node_solo(Parser *p, NodeType type, Token *token, Node *node){
    Node *n = init_node(p, type);
    n->TokenNodeSolo.node = node;
    n->TokenNodeSolo.token = token;
    return n;
}

Node *ast_token_node_duo(Parser *p, NodeType type, Token *token, Node *node_a, Node *node_b){
    Node *n = init_node(p, type);
    n->TokenNodeDuo.node_a = node_a;
    n->TokenNodeDuo.node_b = node_b;
    n->TokenNodeDuo.token = token;
    return n;
}

Node *ast_token_list(Parser *p, NodeType type, Token *token, NodesList list){
    Node *n = init_node(p, type);
    n->TokenList.token = token;
    n->TokenList.list = list;
    return n;
}

Node *ast_list_solo(Parser *p, NodeType type, NodesList list){
    Node *n = init_node(p, type);
    n->ListSolo = list;
    return n;
}

Node *ast_list_node_solo(Parser *p, NodeType type, NodesList list, Node *node){
    Node *n = init_node(p, type);
    n->ListNodeSolo.node = node;
    n->ListNodeSolo.list = list;
    return n;
}