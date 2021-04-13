//
// Created by xen134 on 12/4/2021.
//

#ifndef CSCANNER_AST_H
#define CSCANNER_AST_H

#include "production.h"
#include "../lib/stack.h"

typedef enum {ATOM, EXPR, EMPTY, } node_type;

typedef struct ast_node{
    int type;
    size_t code;
    stack *expr;
    char *s;
} ast_node;

ast_node *make_atom(symbol *s);
ast_node *make_empty();
ast_node *make_expr(symbol *s);
void push_node(ast_node *n, ast_node *t);
void clean_ast(ast_node *n);
void destruct_node(ast_node *n);
void destruct_tree(ast_node *n);

#endif //CSCANNER_AST_H
