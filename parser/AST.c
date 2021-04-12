//
// Created by xen134 on 12/4/2021.
//

#include "AST.h"
#include "production.h"
#include "../lib/stack.h"
#include <malloc.h>

ast_node *make_atom(symbol *s) {
    ast_node *p = malloc(sizeof(ast_node));
    p->type = 0;
    p->value.atom = s;
    return p;
}

ast_node *make_expr() {
    ast_node *p = malloc(sizeof(ast_node));
    p->type = 1;
    p->value.expr = make_stack();
    return p;
}

void push_node(ast_node *n, ast_node *t) {
    if (n->type) {
        push(n->value.expr, t);
    }
}

void clean_ast(ast_node *n) {
    if (n->type) {
        clean(n->value.expr);
    }
}

void destruct_node(ast_node *n) {
    if (n->type == 1) {
        destruct(n->value.expr);
        free(n->value.expr);
    }
    free(n);
}

void destruct_tree(ast_node *n) {
    if (!n->type) destruct_node(n);
    else {
        for (int i = 0; i < n->value.expr->length; i++) {
            destruct_node(get(n->value.expr, i));
        }
        free(n);
    }
}