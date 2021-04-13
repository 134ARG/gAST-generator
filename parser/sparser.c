//
// Created by xen134 on 8/4/2021.
//

#include "sparser.h"
#include "../lib/sscanner.h"
#include "parser_globals.h"
#include "production.h"
#include <stdio.h>

static void parse_error(const char *message) {
    printf("lineno: %d: Error while parsing s-script: %s\n", lineno, message);
    fprintf(stderr, "lineno: %d: Error while parsing s-script: %s\n", lineno, message);
    exit(1);
}

static void parse_s() {
    int token;

    while ((token = next_token()) && token != EOSCAN) {
        if (token == SYMBOL) {
            symbol *s = get_symbol(Text);
            token = next_token();
            if (token != COLON) parse_error("No specifier.");
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
                    printf("text is: %s\n", Text);
                    parse_error("Not a symbol.");

                }
                extend_by_name(s, Text);
            }
        }
    }
}

void p_sparse_main(const char *path) {
    init_pglobals();
    open_file(path);
    parse_s();
    clean_scan();
//    destruct_pglobals();
}