//
// Created by xen134 on 21/3/2021.
// Some small functions used in scanner and parser
//

#ifndef SCANNER_CTOOL_H
#define SCANNER_CTOOL_H

int islparen(int ch);
int isrparen(int ch);
int iscolon(int ch);
int issemi(int ch);
int isbar(int ch);
int issymbol(int ch);

char *copy_string(const char *str);

#endif //SCANNER_CTOOL_H
