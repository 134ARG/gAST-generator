//
// Created by xen134 on 8/4/2021.
//

#include "sparser.h"
#include "../lib/sscanner.h"
#include "../lib/error_report.h"
#include "parser_globals.h"
#include "production.h"

static void parse_error(const char *fmt, ...) {
    va_list valist;
    va_start(valist, fmt);
    general_error("Error while parsing non-terminal definitions", get_lineno(), fmt, valist);
    va_end(valist);
}

// parser for nonterminal definition file
static void parse_script() {
    int token;

    while ((token = next_token()) && token != EOSCAN) {
        if (token == SYMBOL) {
            symbol *s = get_symbol(current_text());
            token = next_token();
            if (token != COLON) parse_error("No specifier.\n");
            new_production(s);
            while ((token = next_token()) && token != SEMICOLON) {
                if (token == BAR) {
                    new_production(s);
                    token = next_token();
                    if (token == BAR) {
                        //extend_empty(s);
                        continue;
                    } else if (token == SEMICOLON) {
                        break;
                    }
                }
                if (token != SYMBOL) {
                    parse_error("Not a symbol: %s\n", current_text());

                }
                extend_by_name(s, current_text());
            }
        }
    }
}

// booster
void p_sparse_main(const char *path) {
    init_pglobals();
    open_file(path);
    parse_script();
    for (int i = 0; i < symbols_length(); i++) {
        reduce_left_recursion(get(symbols_stack(), i));
    }
    clean_scan();
//    destruct_pglobals();
}