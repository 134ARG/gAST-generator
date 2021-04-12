//
// Created by xen134 on 12/4/2021.
//

#include "parser_main.h"
#include "production.h"
#include "parser_globals.h"
#include "../scanner/scanner_main.h"
#include "../lib/stack.h"
#include <stdio.h>

void parser_main(const char *path) {
    init_scan(path);
    stack *s = make_stack();

    int token = next_token_s();

    while (token != -1) {

        for (int i = 0; i < symbols.length; i++) {
            symbol *r = recursive_apply(get(&symbols, i), token, s);
            if (r) {
                for (int j = 0; j < s->length; j++) {
                    symbol *t = get(s, j);
                    if (t->code == -1) printf("token: %ld: \n", t->value.token_id);
                    else printf("non-terminal: %ld: \n", t->code);
                }
                printf("----\n");
            }
        }
        token = next_token_s();
    }

}