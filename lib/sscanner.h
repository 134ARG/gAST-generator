//
// Created by xen134 on 28/3/2021.
//

#ifndef CSCANNER_SSCANNER_H
#define CSCANNER_SSCANNER_H

extern int lineno;
extern char *Text;

enum token_type {
    EOSCAN = 265,
    SYMBOL = 259,
    COLON = 260,
    SEMICOLON = 261,
    LPAREN = 262,
    RPAREN = 263,
    BAR = 264,

};

enum token_type next_token();
char next_char(int fallback);
int open_file(const char *path);
void clean_scan();

#endif //CSCANNER_SSCANNER_H
