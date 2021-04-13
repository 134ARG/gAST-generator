//
// Created by xen134 on 12/4/2021.
//

#include "match.h"
#include "AST.h"
#include "../scanner/scanner_main.h"
#include "../scanner/scanner_globals.h"
#include "parser_globals.h"
#include "../lib/sscanner.h"
#include "../lib/ctool.h"
#include <stdio.h>

static void match_error(const char *message) {
    printf("lineno: %d: Error while matching source: %s\n", lineno, message);
    fprintf(stderr, "lineno: %d: Error while matching source: %s\n", lineno, message);
    exit(1);
}

ast_node *recursive_apply(symbol *s, int *token) {
    if (s->type == TERMINAL) {
        if (s->value.token_id == *token) {
            ast_node *n = make_atom(s);
            n->s = copy_string(text);
            *token = next_token_s();
            return n;
        }
        return NULL;
    }

    for (int i = 0; i < s->value.productions->length; i++) {
        stack *p = get(s->value.productions, i);
        symbol *first = get(p, 0);
        if (first == NULL) continue;
        ast_node *tree = make_expr(s);
        ast_node *first_test = recursive_apply(first, token);

        if (first_test) {
            if (first_test->type != EMPTY)
                push_node(tree, first_test);

            for (int j = 1; j < p->length; j++) {
                symbol *t = get(p, j);
                ast_node *r = recursive_apply(t, token);
                if (!r) {
                    match_error("Not LL(1) grammar.");
                }
                if (r->type != EMPTY) push_node(tree, r);
            }
            return tree;
        }
    }
    if (has_empty(s)) return make_empty();
    return NULL;
}