//
// Created by xen134 on 7/4/2021.
//

#include "parser_globals.h"
#include "production.h"
#include "../lib/ctool.h"
#include "../scanner/tokens.h"
#include <string.h>
#include "stdio.h"

// unused currently. Will be in assignment 4
stack matched_text;
// strings for token names
stack symbol_names;
// symbol structs
stack symbols;

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

// if exists, return symbol. If not, insert new one
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

symbol *get_symbol_by_code(size_t code) {
    return get(&symbols, code);
}

size_t symbols_length() {
    return symbols.length;
}

const char *get_symbol_name(size_t code) {
    get(&symbol_names, code);
}

// only for debug: print every nonterminal
void debug_print_symbol() {
    for (int i = 0; i < symbols.length; i++) {
        printf("%d: Symbol Name:%s\n", i, (char *)get(&symbol_names, i));
        symbol *s = get(&symbols, i);
        stack *p = s->productions;
        for (int j = 0; j < p->length; j++) {
            stack *p1 = get(p, j);
            for (int k = 0; k < p1->length; k++) {
                symbol *s1 = get(p1, k);
                if (s1->type == TERMINAL) {
                    printf("\t%lu: %s ", s1->code, get_token_name(s1->code));
                } else {
                    printf("\t%lu: %s ", s1->code, get_symbol_name(s1->code));
                }

            }
            printf("\nProduction finished.\n");
        }
    }
}

stack *symbols_stack() {
    return &symbols;
}

