//
// Created by xen134 on 12/4/2021.
//

#include "parser_main.h"
#include "production.h"
#include "parser_globals.h"
#include "../scanner/scanner_main.h"
#include "../scanner/scanner_globals.h"
#include "parser_globals.h"
#include "../lib/stack.h"
#include "AST.h"
#include "match.h"
#include <stdio.h>

void print_tree(ast_node *tree, int nest) {
    if (!tree->type) {
        printf("%*c", nest, '\t');
        printf("terminal: %lu - %s\n", tree->code, get(&token_names, tree->code));
        return;
    }

    stack *tmp = make_stack();
    printf("%*c", nest, '\t');
    printf("non-terminal: %lu - %s\n", tree->code, get(&symbol_names, tree->code));
    for (int i = 0; i < tree->expr->length; i++) {
        ast_node *current = get(tree->expr, i);

        print_tree(current, nest+1);
    }
}

void parser_main(const char *path) {
    init_scan(path);
    stack *s = make_stack();

    int token = next_token_s();

    while (token != -1) {

        for (int i = 0; i < symbols.length; i++) {
            ast_node *tree = recursive_apply(get(&symbols, i), token);
            if (tree) {
                print_tree(tree, 1);
                printf("----\n");
            }
        }
        token = next_token_s();
    }

}