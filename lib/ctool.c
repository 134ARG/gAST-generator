//
// Created by xen134 on 21/3/2021.
//

#include "ctool.h"
#include <malloc.h>
#include <string.h>
#include <ctype.h>

int islparen(int ch) {
    return ch == '(';
}

int isrparen(int ch) {
    return ch == ')';
}

int iscolon(int ch) {
    return ch == ':';
}

int issemi(int ch) {
    return ch == ';';
}

int isbar(int ch) {
    return ch == '|';
}

int issymbol(int ch) {
    return !(isblank(ch) || iscntrl(ch) || isrparen(ch)
            || iscolon(ch) || issemi(ch) || isbar(ch));
}

char *copy_string(const char *str) {
    char *p = malloc(sizeof(char)*(strlen(str)+1));
    strcpy(p, str);
    return p;
}
