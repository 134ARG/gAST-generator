//
// Created by xen134 on 28/3/2021.
//

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "sparser.h"
#include "../lib/sscanner.h"
#include "../lib/stack.h"
#include "../lib/error_report.h"
#include "expression.h"
#include "scanner_globals.h"

extern struct stack regex;

/*
 * used for parser error report
 */
static void parse_error(const char *fmt, ...) {
    va_list valist;
    va_start(valist, fmt);
    general_error("Error while parsing s-script", get_lineno(), fmt, valist);
    va_end(valist);
}

void new_regex(expression *p) {
    push(&regex, p);
}

/*
 * parse the script file and generate token definition
 */
void parse_script() {
    // nest and expr_stack are used to handle nested S-expression
    int nest = 0;
    int unit = next_token();
    struct stack *expr_stack = make_stack();

    while (unit != EOSCAN) {
        if (unit == LPAREN) {
            nest++;
            unit = next_token();
            if (unit != SYMBOL) {
                parse_error("no function name specified.\n");
            }

            int code = get_funcode(current_text());
            if (code == -1) {
                parse_error("no such function found: %s\n", current_text());
            }
            expression *p = make_expression(EXPR, code, make_stack());
            if (nest == 1) {        // new token
                new_regex(p);
            } else {
                expression *current = top(expr_stack);
                push(current->value.param, p);
            }
            push(expr_stack, p);

        } else if (unit == RPAREN) {
            nest--;
            pop(expr_stack);
            if (nest == 0) {
                unit = next_token();
                if (unit != SYMBOL) {
                    parse_error("%s: No token name specified.", current_text());
                }
                add_token(current_text());
            }
        } else {
            expression *p = top(expr_stack);
            push(p->value.param, make_expression(ATOM, -1, strdup(current_text())));
        }
        unit = next_token();
    }
}

/*
 * main function for loading token definitions
 */
void sparse_main(const char *path) {
    open_file(path);
    parse_script();
    clean_scan();
}