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
    p->offset = 0;
    return p;
}

ast_node *make_expr_with_symbol(symbol *s) {
    ast_node *p = malloc(sizeof(ast_node));
    p->type = 1;
    p->code = s->code;
    p->expr = make_stack();
    p->offset = 0;
    return p;
}

ast_node *make_expr() {
    ast_node *p = malloc(sizeof(ast_node));
    p->type = 1;
    p->code = -1;
    p->expr = make_stack();
    p->offset = 0;
    return p;
}

ast_node *make_empty() {
    ast_node *p = malloc(sizeof(ast_node));
    p->type = EMPTY;
    p->offset = 0;
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

void copy_node(ast_node *s, ast_node *d) {
    d->type = s->type;
    d->code = s->code;
    d->s = s->s;
    d->offset = s->offset;
    destruct(d->expr);
    free(d->expr);
    d->expr = s->expr;
}

int is_token(ast_node *n) {
    if (n->type == ATOM || n->type == VAR || n->type == OP || n->type == IMMD || n->type == ARR || n->type == ASSIGN) {
        return 1;
    } else {
        return 0;
    }
}

ast_node *get_nth(ast_node *n, size_t index) {
    if (!n->expr) return NULL;
    if (index > n->expr->length) {
        return NULL;
    } else {
        return get(n->expr, index);
    }
}

ast_node *make_expr_with_stack(stack *s) {
    ast_node *p = malloc(sizeof(ast_node));
    p->type = 1;
    p->code = -1;
    p->expr = s;
    p->offset = 0;
    return p;
}

ast_node *make_node(int type) {
    ast_node *p = malloc(sizeof(ast_node));
    p->type = type;
    p->code = -1;
    p->expr = NULL;
    p->offset = 0;
    return p;
}
