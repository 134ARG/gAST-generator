cmake_minimum_required(VERSION 3.17)
project(gAST-generator C)

set(CMAKE_C_STANDARD 11)

add_executable(comBin ./scanner/expression.c ./scanner/expression.h lib/sstring.c lib/sstring.h lib/stack.c lib/stack.h lib/lexer.c lib/lexer.h lib/ctool.c lib/ctool.h scanner/token_parser.c scanner/token_parser.h ./scanner/functions.c ./scanner/functions.h scanner/scanner.c scanner/scanner.h scanner/tokens.c scanner/tokens.h parser/production.c parser/production.h parser/parser_globals.c parser/parser_globals.h parser/sparser.c parser/sparser.h main.c parser/parser_main.c parser/parser_main.h parser/AST.c parser/AST.h parser/match.c parser/match.h lib/error_report.c lib/error_report.h code_gen/code_gen.c code_gen/code_gen.h)