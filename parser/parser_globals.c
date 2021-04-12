//
// Created by xen134 on 7/4/2021.
//

#include "parser_globals.h"
#include "production.h"
#include "../lib/ctool.h"
#include "../lib/stack.h"
#include "../scanner/scanner_globals.h"
#include <string.h>
#include "stdio.h"

stack matched_text;
stack symbol_names;
stack symbols;

//int lineno;

void init_pglobals() {
    init_stack(&matched_text);
    init_stack(&symbol_names);
    init_stack(&symbols);
}

void destruct_pglobals() {
    free_content(&matched_text);
    destruct(&matched_text);

    free_content(&symbol_names);
    destruct(&symbol_names);
}

void push_text(const char *str) {
    push(&matched_text, copy_string(str));
}

void clean_text() {
    free_content(&matched_text);
    clean(&matched_text);
}

symbol* get_symbol(const char *name) {
    for (size_t i = 0; i < symbol_names.length; i++) {
        if (!strcmp(name, get(&symbol_names, i))) {
            return get(&symbols, i);
        }
    }
    push(&symbol_names, copy_string(name));
    push(&symbols, make_symbol(symbols.length, NONTERMINAL));
    return top(&symbols);
}


void debug_print_symbol() {
    for (int i = 0; i < symbols.length; i++) {
        printf("%d: Symbol Name:%s\n", i, get(&symbol_names, i));
        symbol *s = get(&symbols, i);
        stack *p = s->value.productions;
        for (int j = 0; j < p->length; j++) {
            stack *p1 = get(p, j);
            for (int k = 0; k < p1->length; k++) {
                symbol *s1 = get(p1, k);
                if (s1->type == TERMINAL) {
                    printf("\t%lu: %s ", s1->value.token_id, get(&token_names, s1->value.token_id));
                } else {
                    printf("\t%lu: %s ", s1->code, get(&symbol_names, s1->code));
                }

            }
            printf("\nProduction finished.\n");
        }
    }
}