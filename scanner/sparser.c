//
// Created by xen134 on 28/3/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include "sparser.h"
#include "../shared/sscanner.h"
#include "../data_structure/stack.h"
#include "expression.h"
#include "../shared/ctool.h"
#include "scanner_globals.h"

extern char *Text;
extern struct stack regex;
extern int lineno;

/*
 * used for parser error report
 */
static void parse_error(const char *message) {
    printf("lineno: %d: Error while parsing s-script: %s\n", lineno, message);
    fprintf(stderr, "lineno: %d: Error while parsing s-script: %s\n", lineno, message);
}

/*
 * parse the script file and generate token definition
 */
void parse_s() {
    // nest and expr_stack are used to handle nested S-expression
    int nest = 0;
    int unit = next_token();
    struct stack *expr_stack = make_stack();

    while (unit != EOSCAN) {
        if (unit == LPAREN) {
            nest++;
            unit = next_token();
            if (unit != SYMBOL) {
                parse_error("no function name specified.");
                exit(1);
            }

            int code = get_funcode(Text);
            if (code == -1) {
                parse_error("no such function found.");
                parse_error(Text);
                exit(1);
            }
            expression *p = make_expression(EXPR, code, make_stack());
            if (nest == 1) {        // new token
                push(&regex, p);
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
                    parse_error(Text);
                    parse_error("No token name specified.");
                    exit(1);
                }
                add_token_name(Text);
            }
        } else {
            expression *p = top(expr_stack);
            push(p->value.param, make_expression(ATOM, -1, copy_string(Text)));
        }
        unit = next_token();
    }
}

/*
 * main function for loading token definitions
 */
void sparse_main(const char *path) {
    open_file(path);
    parse_s();
    clean_scan();
}