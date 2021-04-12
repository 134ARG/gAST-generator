//
// Created by xen134 on 28/3/2021.
//

#include "expression.h"
#include "../data_structure/stack.h"
#include <malloc.h>
#include <string.h>
#include "scanner_globals.h"

expression *make_expression(int type, int funcode, void *data) {
    expression *p = malloc(sizeof(struct expression));
    p->funcode = funcode;
    p->type = type;
    p->value.param = data;
    return p;
}

void free_expression(struct expression *p) {
    if (p->type == ATOM) {
        free(p->value.str);
    } else {
        destruct(p->value.param);
        free(p->value.param);
    }
    free(p);
}

