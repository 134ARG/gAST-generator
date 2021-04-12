//
// Created by xen134 on 8/4/2021.
//

#include "parser_globals.h"
#include "sparser.h"
#include "parser_main.h"
#include "../scanner/scanner_globals.h"
#include "../scanner/sparser.h"

int main() {

    init_globals();
    sparse_main("./test.tokens");

    p_sparse_main("./test.productions");
    debug_print_symbol();

    parser_main("./test.source");

    return 0;
}