//
// Created by xen134 on 12/4/2021.
//

#include "match.h"
#include "AST.h"
#include "../scanner/scanner_main.h"

ast_node *recursive_apply(symbol *s, int token) {
    if (s->type == TERMINAL) {
        if (s->value.token_id == token) {
            return make_atom(s);
        } else {
            return NULL;
        }
    }

    for (int i = 0; i < s->value.productions->length; i++) {
        stack *p = get(s->value.productions, i);
        symbol *first = get(p, 0);
        ast_node *tree = make_expr();
        ast_node *first_test = recursive_apply(first, token);

        if (first_test) {
            push_node(tree, first_test);
            for (int j = 1; j < p->length; j++) {
                symbol *t = get(p, j);
                token = next_token_s();
                ast_node *r = recursive_apply(t, token);
                if (!r) {
                    destruct_tree(tree);
                    return NULL;
                } else {
                    push_node(tree, r);
                }
            }
            return tree;
        }
    }
    return NULL;
}