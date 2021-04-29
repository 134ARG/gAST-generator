//
// Created by xen134 on 12/4/2021.
//

#include "parser_main.h"
#include "production.h"
#include "parser_globals.h"
#include "../scanner/scanner_main.h"
#include "../scanner/scanner_globals.h"
#include "../lib/sscanner.h"
#include "AST.h"
#include "match.h"
#include <stdio.h>


static void parse_error(const char *message) {
    printf("lineno: %d: Error while parsing: %s\n", get_lineno(), message);
    fprintf(stderr, "lineno: %d: Error while parsing: %s\n", get_lineno(), message);
    exit(1);
}

// function for printing the final parsing tree recursively
void print_tree(ast_node *tree, int nest) {
    if (!tree->type) {
        printf("%*c", 2*nest, ' ');
        printf("|-> terminal: %s", get_token_name(tree->code));
        printf(": %s\n", tree->s);
        return;
    } else {
        symbol *s = get_symbol_by_code(tree->code);
        if (s->type != TMP) {
            printf("%*c", 2 * nest, ' ');
            printf("|-> non-terminal: %s\n", get_symbol_name(tree->code));
            nest++;
        }

        for (int i = 0; i < tree->expr->length; i++) {
            ast_node *current = get(tree->expr, i);
            print_tree(current, nest);
        }
    }
}

// main procedure for parsing source
void parser_main(const char *path) {
    init_scan(path);

    int token = next_token_s();

    // read first nonterminal as starting point
    symbol *general = get_symbol_by_code(0);
    ast_node  *tree = recursive_apply(general, &token);
    if (tree) {
        if (tree->type != EMPTY) {
            print_tree(tree, 0);
            printf("----\n");
        }
    }

    if (token != -1) {
        parse_error("Unmatched token remains.");
    }

    destruct_tree(tree);
}