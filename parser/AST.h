//
// Created by xen134 on 12/4/2021.
//

#ifndef CSCANNER_AST_H
#define CSCANNER_AST_H

#include "production.h"
#include "../lib/stack.h"

typedef enum {ATOM, EXPR, EMPTY, DECL, VAR, ARR, STATEMENT, IF, WHILE, DO, RETURN, READ, WRITE, ASSIGN, OP, IMMD, ARI, TMP_VAL} node_type;

// struct used for expressing parsing tree
typedef struct ast_node{
    int type;
    size_t code;
    stack *expr;
    int offset;
    char *s;
} ast_node;

// make atom type node
ast_node *make_atom(symbol *s);

// make empty node
ast_node *make_empty();

// make expression type node (nonterminal)
ast_node *make_expr_with_symbol(symbol *s);
ast_node *make_expr_with_stack(stack *s);
ast_node *make_node(int type);
ast_node *make_expr();

// add node to node expressing nonterminal symbol
void push_node(ast_node *n, ast_node *t);

// functions for cleaning
void clean_ast(ast_node *n);
ast_node *get_nth(ast_node *n, size_t index);
void copy_node(ast_node *s, ast_node *d);
int is_token(ast_node *n);
void destruct_node(ast_node *n);
void destruct_tree(ast_node *n);

#endif //CSCANNER_AST_H
