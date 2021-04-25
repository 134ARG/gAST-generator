//
// Created by xen134 on 12/4/2021.
//

#ifndef CSCANNER_AST_H
#define CSCANNER_AST_H

#include "production.h"
#include "../lib/stack.h"

typedef enum {ATOM, EXPR, EMPTY, } node_type;

// struct used for expressing parsing tree
typedef struct ast_node{
    int type;
    size_t code;
    stack *expr;
    char *s;
} ast_node;

// make atom type node
ast_node *make_atom(symbol *s);

// make empty node
ast_node *make_empty();

// make expression type node (nonterminal)
ast_node *make_expr(symbol *s);

// add node to node expressing nonterminal symbol
void push_node(ast_node *n, ast_node *t);

// functions for cleaning
void clean_ast(ast_node *n);
void destruct_node(ast_node *n);
void destruct_tree(ast_node *n);

#endif //CSCANNER_AST_H
