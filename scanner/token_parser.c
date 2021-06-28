//
// Created by xen134 on 28/3/2021.
//

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "token_parser.h"
#include "../lib/lexer.h"
#include "../lib/stack.h"
#include "../lib/error_report.h"
#include "expression.h"
#include "functions.h"
//#include "tokens.h"

/*
 * used for parser error report
 */
static void parse_error(const char *fmt, ...) {
    va_list valist;
    va_start(valist, fmt);
    general_error("Error while parsing token specification", get_lineno(), fmt, valist);
    va_end(valist);
}

void new_token(struct token_context* ctx, expression *p, const char *name) {
    push(ctx->token_definitions, p);
    push(ctx->token_names, strdup(name));
}

/*
 * parse the script file and generate token definition
 */
static struct token_context* parse_token() {
    int nest = 0;
    int unit = next_symbol();
    struct stack* expr_stack = make_stack();
    struct token_context* ctx = new_context();

    while (unit != EOSCAN) {
        if (unit == LPAREN) {
            nest++;
            unit = next_symbol();
            if (unit != TEXT_SEG) {
                parse_error("no function name specified.\n");
            }

            int code = get_funcode(current_text());
            if (code == -1) {
                parse_error("no such function found: %s\n", current_text());
            }
            expression *p = make_expression(EXPR, code, make_stack());
            if (nest != 1) {        // new token
                expression *current = top(expr_stack);
                push(current->value.param, p);
            }
            push(expr_stack, p);
        } else if (unit == RPAREN) {
            nest--;
            struct expression* p = pop(expr_stack);

            if (nest == 0 && p) {
                unit = next_symbol();
                if (unit != TEXT_SEG) {
                    parse_error("%s: No valid token name specified.", current_text());
                }
                new_token(ctx, p, current_text());
            } else if (nest == 0 && !p) {
                parse_error("Unmatched parentheses.");
            }
        } else {
            expression *p = top(expr_stack);
            push(p->value.param, make_expression(ATOM, -1, strdup(current_text())));
        }
        unit = next_symbol();
    }
}

/*
 * main function for loading token definitions
 */
struct token_context* read_token_definitions(const char *path) {
    open_file(path);
    struct token_context* ctx = parse_token();
    clean_scan();
    return ctx;
}

struct token_context *new_context() {
    struct token_context* p = malloc(sizeof(struct token_context));
    p->token_definitions = make_stack();
    p->token_names = make_stack();
    p->id = 0;
    return p;
}

void free_context(struct token_context** p) {
    if (*p != NULL) {
        destruct_and_free((*p)->token_names);
        destruct_and_free((*p)->token_definitions);
        free(*p);
        *p = NULL;
    }
}
