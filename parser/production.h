//
// Created by xen134 on 6/4/2021.
//

#ifndef CSCANNER_PRODUCTION_H
#define CSCANNER_PRODUCTION_H

#include "../lib/stack.h"
#include "stdlib.h"

typedef enum {TERMINAL, NONTERMINAL, TMP, } symbol_type;

// not used
typedef struct production {
    stack *symbols;
} production;

// used for expressing nonterminals
typedef struct symbol {
    size_t code;
    symbol_type type;
    stack *productions;
} symbol;

symbol *make_symbol(size_t code, symbol_type type);
void destruct_symbol(symbol *s);
void new_production(symbol *s);
void extend_by_name(symbol *s, const char *name);
void extend_empty(symbol *s);
void reduce_left_recursion(symbol *s);
int has_empty(symbol *s);

#endif //CSCANNER_PRODUCTION_H
