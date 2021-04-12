//
// Created by xen134 on 6/4/2021.
//

#include "production.h"
#include "../lib/stack.h"
#include "./AST.h"
#include "../scanner/scanner_globals.h"
#include "../scanner/scanner_main.h"
#include "parser_globals.h"
#include <time.h>
#include <stdlib.h>

symbol *make_symbol(size_t code, symbol_type type) {
    symbol *p = malloc(sizeof(struct symbol));
    p->code = code;
    p->type = type;
    if (type == NONTERMINAL) {
        p->value.productions = make_stack();
//        push(p->value.productions, make_stack());
    } else {
        p->value.token_id = -1;
    }
    return p;
}

void extend_production(symbol *s, symbol *p) {
    push(top(s->value.productions), p);
}

void extend_by_name(symbol *s, const char *name) {
//    symbol *s = top(&symbols);
    if (s->type == TERMINAL) return;

    size_t token_code = get_token_code(name);
    if (token_code == -1) {
        extend_production(s, get_symbol(name));

    } else {
        symbol *p = make_symbol(-1, TERMINAL);
        p->value.token_id = token_code;
        extend_production(s, p);
    }
}

void extend_by_stack(symbol *s, stack *symbol_stack, size_t start, size_t end) {
    copy_stack(symbol_stack, top(s->value.productions), start, end);
}


void new_production(symbol *s) {
    push(s->value.productions, make_stack());
}


void iterate_productions(symbol *s, void (*f)(symbol *s, stack *p)) {
    if (s->type == TERMINAL) return;
    stack *ps = s->value.productions;
    for (int i = 0; i < ps->length; i++) {
        stack *p = get(ps, i);
        f(s, p);
    }
}

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
    free(p);
    return tmp;
}


void reduce_left_recursion(symbol *s) {
    if (s->type == TERMINAL) return;
    stack *ps = s->value.productions;
    stack *non_recursion = make_stack();
    stack *recursion = make_stack();

    for (size_t i = 0; i < ps->length; i++) {
        stack *p = get(ps, i);
        symbol *first = get(p, 0);
        if (s->code == first->code) {
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

            copy_stack(non_recursion, tmp_a->value.productions, 0, non_recursion->length);

            new_production(tmp_b);
            copy_stack(p, top(tmp_b->value.productions), 1, p->length);
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
        destruct(s->value.productions);
    }
    free(s);
}

int has_empty(symbol *s) {
    for (int i = 0; i < s->value.productions->length; i++) {
        stack *p = get(s->value.productions, i);
        if (p->length == 0) return 1;
    }
    return 0;
}

stack *predict(symbol *s) {
    if (s->type == TERMINAL) return NULL;

}

void parse_table() {

}

symbol *recursive_apply(symbol *s, int token, ast_node *result) {
    if (s->type == TERMINAL) {
        if (s->value.token_id == token) {
            push_node(result, make_atom(s));
            return s;
        } else {
            clean_ast(result);
            return NULL;
        }
    }

    for (int i = 0; i < s->value.productions->length; i++) {
        stack *p = get(s->value.productions, i);
        symbol *first = get(p, 0);

        if (recursive_apply(first, token, result)) {
            for (int j = 1; j < p->length; j++) {
                symbol *t = get(p, j);
                token = next_token_s();
                if (token != -1) {
                    symbol *r = recursive_apply(t, token, result);
                    if (!r) {
                        clean(result);
                        return NULL;
                    }
                } else {
                    clean(result);
                    return NULL;
                }

            }
            push(result, s);
            return s;
        }
    }
    return NULL;
}

