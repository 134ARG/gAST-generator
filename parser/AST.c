//
// Created by xen134 on 12/4/2021.
//

#include "AST.h"
#include "production.h"
#include <malloc.h>

ast_node *make_atom(symbol *s) {
    ast_node *p = malloc(sizeof(ast_node));
    p->type = 0;
    p->code = s->code;
    return p;
}

ast_node *make_expr(symbol *s) {
    ast_node *p = malloc(sizeof(ast_node));
    p->type = 1;
    p->code = s->code;
    p->expr = make_stack();
    return p;
}

ast_node *make_empty() {
    ast_node *p = malloc(sizeof(ast_node));
    p->type = EMPTY;
    return p;
}

void push_node(ast_node *n, ast_node *t) {
    if (n->type == EXPR) {
        push(n->expr, t);
    }
}

void clean_ast(ast_node *n) {
    if (n->type == EXPR) {
        clean(n->expr);
    }
}

void destruct_node(ast_node *n) {
    if (n->type == EXPR) {
        destruct(n->expr);
        free(n->expr);
    }
    free(n);
}

void destruct_tree(ast_node *n) {
    if (n->type == EXPR) {
        for (int i = 0; i < n->expr->length; i++) {
            destruct_node(get(n->expr, i));
        }
        free(n);
    }
    else {
        destruct_node(n);
    }
}