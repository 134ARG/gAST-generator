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
    printf("lineno: %d: Error while matching source: %s\n", get_lineno(), message);
    fprintf(stderr, "lineno: %d: Error while matching source: %s\n", get_lineno(), message);
    exit(1);
}

// main matching process following paring graph
ast_node *recursive_apply(symbol *s, int *token) {
    // base case for recursion
    if (s->type == TERMINAL) {
        if (s->code == *token) {
            ast_node *n = make_atom(s);
            n->s = copy_string(text);
            *token = next_token_s();
            return n;
        }
        return NULL;
    }

    // if input is nonterminal
    for (int i = 0; i < s->productions->length; i++) {
        stack *p = get(s->productions, i);
        symbol *first = get(p, 0);
        if (first == NULL) continue;
        ast_node *tree = make_expr(s);
        // first test. Refer to report for details
        ast_node *first_test = recursive_apply(first, token);

        if (first_test) {
            if (first_test->type != EMPTY)
                push_node(tree, first_test);

            for (int j = 1; j < p->length; j++) {
                symbol *t = get(p, j);
                ast_node *r = recursive_apply(t, token);
                if (!r) {
                    if (t->type == TERMINAL)
                        printf("symbol: %s\n", get_token_name(t->code));
                    else if (t->type == NONTERMINAL)
                        printf("symbol: %s\n", get_symbol_name(tree->code));
                    printf("text: %s\n", text);
                    match_error("Unfinished production or invalid LL(1) grammar.");
                }
                if (r->type != EMPTY) push_node(tree, r);
            }
            return tree;
        }
    }
    if (has_empty(s)) return make_empty();
    return NULL;
}