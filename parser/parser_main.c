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
        printf("%*c", 4*nest, ' ');
        printf("|-> terminal: %s", get(&token_names, tree->code));
        printf(": %s\n", tree->s);
        return;
    }

    printf("%*c", 4*nest, ' ');
    printf("|-> non-terminal: %s\n", get(&symbol_names, tree->code));
    for (int i = 0; i < tree->expr->length; i++) {
        ast_node *current = get(tree->expr, i);

        print_tree(current, nest+1);
    }
}

void parser_main(const char *path) {
    init_scan(path);
    stack *s = make_stack();

    int token = next_token_s();

    symbol *general = get(&symbols, 0);
    printf("2:%s\n", get(&symbol_names, 2));
    ast_node  *tree = recursive_apply(general, &token);
    if (tree) {
        if (tree->type != EMPTY) {
            print_tree(tree, 1);
            printf("----\n");
        }
    }

//    while (token != -1) {
//        ast_node *tree;
//        for (int i = 0; i < symbols.length; i++) {
//            tree = recursive_apply(get(&symbols, i), &token);
//            if (tree) {
//                if (tree->type != EMPTY) {
//                    print_tree(tree, 1);
//                    printf("----\n");
//                    break;
//                }
//            }
//        }
//    }

}