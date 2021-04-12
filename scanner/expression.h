//
// Created by xen134 on 28/3/2021.
// Definition and relevant operations of S-expression
//

#ifndef CSCANNER_EXPRESSION_H
#define CSCANNER_EXPRESSION_H


enum expr_type {
    ATOM = 1,
    EXPR = 2,
};

typedef struct expression {
    char *name;
    int funcode;
    int type;
    union {
        struct stack *param;
        char *str;
    } value;
} expression;

expression *make_expression(int type, int funcode, void *data);
void free_expression(struct expression *p);

#endif //CSCANNER_EXPRESSION_H
