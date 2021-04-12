//
// Created by xen134 on 6/4/2021.
//

#ifndef CSCANNER_PRODUCTION_H
#define CSCANNER_PRODUCTION_H

#include "../data_structure/stack.h"
#include "stdlib.h"

typedef enum {TERMINAL, NONTERMINAL, } symbol_type;

typedef struct production {
    stack *symbols;
} production;

typedef struct symbol {
    size_t code;
    symbol_type type;
    union {
        int64_t token_id;
        stack *productions;
    } value;
} symbol;

symbol *make_symbol(size_t code, symbol_type type);
void destruct_symbol(symbol *s);
void new_production(symbol *s);
void extend_by_name(symbol *s, const char *name);
void reduce_left_recursion(symbol *s);
symbol *recursive_apply(symbol *s, int token, stack *result);

#endif //CSCANNER_PRODUCTION_H
