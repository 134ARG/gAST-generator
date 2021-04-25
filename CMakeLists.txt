cmake_minimum_required(VERSION 3.17)
project(gAST-generator C)

set(CMAKE_C_STANDARD 11)

add_executable(parserBin scanner/main_disabled.c ./scanner/expression.c ./scanner/expression.h lib/sstring.c lib/sstring.h lib/stack.c lib/stack.h lib/sscanner.c lib/sscanner.h lib/ctool.c lib/ctool.h ./scanner/sparser.c ./scanner/sparser.h ./scanner/spredicate.c ./scanner/spredicate.h ./scanner/scanner_main.c ./scanner/scanner_main.h scanner/scanner_globals.c scanner/scanner_globals.h parser/production.c parser/production.h parser/parser_globals.c parser/parser_globals.h parser/sparser.c parser/sparser.h main.c parser/parser_main.c parser/parser_main.h parser/AST.c parser/AST.h parser/match.c parser/match.h parser/configuration.c parser/configuration.h)