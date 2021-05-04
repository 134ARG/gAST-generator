//
// Created by xen134 on 6/4/2021.
//

#include "production.h"
#include "./AST.h"
#include "../scanner/scanner_globals.h"
#include "parser_globals.h"
#include <time.h>
#include <stdlib.h>

// symbol constructor
symbol *make_symbol(size_t code, symbol_type type) {
    symbol *p = malloc(sizeof(struct symbol));
    p->code = code;
    p->type = type;
    if (type == NONTERMINAL) {
        p->productions = make_stack();
//        push(p->value.productions, make_stack());
    }
    return p;
}

// adding symbol to top production
void extend_production(symbol *s, symbol *p) {
    push(top(s->productions), p);
}

// integrated operation for symbol construction and extension of production
void extend_by_name(symbol *s, const char *name) {
    if (s->type == TERMINAL) return;

    size_t token_code = get_token_code(name);
    if (token_code == -1) {
        extend_production(s, get_symbol(name));
    } else {
        symbol *p = make_symbol(-1, TERMINAL);
        p->code = token_code;
        extend_production(s, p);
    }
}

// unused
void extend_empty(symbol *s) {
    if (s->type == TERMINAL) return;
    extend_production(s, make_symbol(0, EMPTY));
}

// unused
void extend_by_stack(symbol *s, stack *symbol_stack, size_t start, size_t end) {
    copy_stack(symbol_stack, top(s->productions), start, end);
}

// add new production stack
void new_production(symbol *s) {
    push(s->productions, make_stack());
}

// iterator. not used
void iterate_productions(symbol *s, void (*f)(symbol *s, stack *p)) {
    if (s->type == TERMINAL) return;
    stack *ps = s->productions;
    for (int i = 0; i < ps->length; i++) {
        stack *p = get(ps, i);
        f(s, p);
    }
}

// generating tmp nonterminal for reduction. see report for details
symbol *gensym() {
    time_t t = time(NULL);
    time_t base = '0';
    int size = sizeof(time_t);

    for (int i = 0; i < size; i++) {
        t += base;
        base = (base << 8);
    }
    char *p = malloc(size + 1);
    p[size] = '\0';
    symbol *tmp = get_symbol(p);
    tmp->type = TMP;
    free(p);
    return tmp;
}

// as the name shows
void reduce_left_recursion(symbol *s) {
    if (s->type == TERMINAL) return;
    stack *ps = s->productions;
    stack *non_recursion = make_stack();
    stack *recursion = make_stack();

    for (size_t i = 0; i < ps->length; i++) {
        stack *p = get(ps, i);
        symbol *first = get(p, 0);
        if (first && s->code == first->code && s->type == NONTERMINAL && first->type == NONTERMINAL) {
            push(recursion, p);
        } else {
            push(non_recursion, p);
        }
    }

    if (recursion->length != 0) {
        for (size_t i = 0; i < recursion->length; i++) {
            stack *p = get(recursion, i);
            symbol *tmp_a = gensym();
            symbol *tmp_b = gensym();

            copy_stack(non_recursion, tmp_a->productions, 0, non_recursion->length);

            new_production(tmp_b);
            copy_stack(p, top(tmp_b->productions), 1, p->length);
            extend_production(tmp_b, tmp_b);
            new_production(tmp_b);

            clean(ps);
            new_production(s);
            extend_production(s, tmp_a);
            extend_production(s, tmp_b);
        }
    }

    destruct(non_recursion);
    destruct(recursion);
    free(non_recursion);
    free(recursion);
}

void destruct_symbol(symbol *s) {
    if (s->type == NONTERMINAL) {
        destruct(s->productions);
    }
    free(s);
}

// whether the symbol contains empty (lambda) production
int has_empty(symbol *s) {
    for (int i = 0; i < s->productions->length; i++) {
        stack *p = get(s->productions, i);
        if (p->length == 0) return 1;
    }
    return 0;
}




