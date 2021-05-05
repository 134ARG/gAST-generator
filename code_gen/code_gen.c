//
// Created by xen134 on 2/5/2021.
//

#include "code_gen.h"
#include "../lib/stack.h"
#include "../parser/parser_globals.h"
#include "../scanner/scanner_globals.h"
#include "../parser/AST.h"
#include <string.h>
#include <stdio.h>

#define TMP_REG 1		// register id for holding tmp values
#define ACC_REG 2		// register id for storing accumulated results
#define FP "s8"
FILE *output = NULL;
int stack_offset = 32;
size_t max_var_offset = 0;
stack variables;

typedef enum {OBJ, IMMEDIATE} var_type;

typedef struct variable {
    char *name;
    int offset;
    int type;
} variable;

variable *make_variable(const char *name, int type, size_t num) {
    variable *p = malloc(sizeof(variable));
    p->type = type;
    p->offset = max_var_offset;
    max_var_offset += num*4;
    return p;
}

variable *get_variable(const char *name) {
    for (int i = 0; i < variables.length; i++) {
        variable *v = get(&variables, i);
        if (!strcmp(name, v->name)) {
            return v;
        }
    }
    return NULL;

}

void add_to_list(variable *v) {
    push(&variables, v);
}

int op_index[] = {1, 2, 3, 4, 5, 5, 6, 6, 6, 6, 7, 7, 8, 8, 9, 9, 10};

// function for generating load (lw) operation
void gen_load(variable *o, int tmp_reg, int index) {
    if (output == NULL) return;
    if (o->type == OBJ || o->type == TMP_VAL) {
        fprintf(output, "lw $t%d, %d($%s)\n", tmp_reg, o->offset + stack_offset, FP);
    }
}

void gen_li(int immd, int tmp_reg) {
    if (output == NULL) return;
    fprintf(output, "li $t%d, %d\n", tmp_reg, immd);
}

// function for generating save (sw) operation
void gen_save(variable *o, int tmp_reg, int index) {
    if (output == NULL) return;
    if (o->type == OBJ) {
        fprintf(output, "sw $t%d, %d($%s)\n", tmp_reg, o->offset + stack_offset + index, FP);
    }
}

return_type show_variables(void *p, size_t i) {
    variable *v = p;
    printf("%s : %d : %d\n", v->name, v->type, v->offset);
    return (return_type) {0, 0};
}

return_type select_decl(void *p, size_t i) {
    if (((ast_node *)p)->type == DECL) {
        return (return_type) {p, 1};
    }else {
        return (return_type) {0, 0};
    }
}

return_type add_symbol(void *p, size_t i) {
    ast_node *n = p;
    if (n->type == VAR) {
       add_to_list(make_variable(n->s, OBJ, 1));
    } else if (n->type == ARR) {
        char *rem = NULL;
        size_t size = strtol(get_nth(n, 1)->s, &rem, 10);
        add_to_list(make_variable(n->s, OBJ, size));
    } else if (n->type == ASSIGN) {
        char *rem = NULL;
        int initial_val = (int)strtol(get_nth(n, 1)->s, &rem, 10);
        variable *v = make_variable(n->s, OBJ, 1);
        add_to_list(v);

        gen_li(initial_val, TMP_REG);
        gen_save(v, TMP_REG, 0);
    } else {
        printf("errorrrrrrrrrrr!\n");
    }
    return (return_type) {0, 0};
}

stack *gen_symbol_list(ast_node *tree) {
    init_stack(&variables);
    ast_node *decl = find(select_decl, tree->expr);
    destruct_and_free(map(add_symbol, decl->expr));
}

void gen_if_stmt(ast_node *if_stmt) {

}

void gen_single_ari(ast_node *op, ast_node *operand1, ast_node *operand2) {
    int reg1, reg2;
    variable *op1 = NULL, *op2 = NULL;
    if (operand1) {
        op1 = get_variable(operand1->s);
        if (!op1) printf("errororororororororo!\n");
    }

    if (operand2) {
        op2 = get_variable(operand2->s);
        if (!op2) printf("errororororororororo!\n");
    }

    if (operand1 == NULL && operand2 != NULL) {
        gen_load(op2, TMP_REG, operand2->offset);
        reg1 = ACC_REG;
        reg2 = TMP_REG;
    } else if (operand1 != NULL && operand2 == NULL) {
        gen_load(op1, TMP_REG, operand1->offset);
        reg1 = TMP_REG;
        reg2 = ACC_REG;
    } else {
        gen_load(op1, TMP_REG, operand1->offset);
        gen_load(op2, ACC_REG, operand2->offset);
        reg1 = TMP_REG;
        reg2 = ACC_REG;
    }

    if (is_terminal(op, "MINUS")) {
        fprintf(output, "sub $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "PLUS")) {
        fprintf(output, "sub $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "OROR") || is_terminal(op, "OR_OP")) {
        fprintf(output, "sub $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    }
}

void gen_exp(ast_node *ari) {
    stack *raw = ari->expr;
    stack *operator = make_stack();
    stack *operand = make_stack();

    for (int i = 0; i < raw->length; i++) {
        ast_node *current = get(raw, i);
        if (current->type == OP) push(operator, current);
        else push(operator, current);

        if (operand->length == 2) {
            ast_node *op = pop(operator);

        }
    }

}

void debug_print_variables() {
    destruct_and_free(map(show_variables, &variables));
}







/*
 * Shared function for identifying symbols
 */

int is_nonterminal(ast_node *p, const char *str) {
    if (p->type != EXPR) return 0;
    const char *name = get_symbol_name(p->code);
    if (name) return !strcmp(str, name);
    else return 0;
}

int is_terminal(ast_node *p, const char *str) {
    if (!p) return 0;
    if (p->type != ATOM) return 0;
    const char *name = get_token_name(p->code);
    return !strcmp(str, name);
}


/*
 * For if statement
 *
 */
return_type purge_identifier(void *p, size_t i) {
    if (is_terminal(p, "IF") || is_terminal(p, "ELSE")
        || is_terminal(p, "LBRACE") || is_terminal(p, "RBRACE")
        || is_terminal(p, "SEMI")) {
        return (return_type) {0, 0};
    } else {
        return (return_type) {p, 1};
    }
}

ast_node *statement_reassigner(ast_node *n) {
    ast_node *r = make_expr_with_stack(map(purge_identifier, n->expr));
//    r->type = IF;
//    r->s = strdup("IF");
    return r;
}

/*
 * For declarations:
 * Flatten and store all info needed for variable declaration.
 */

return_type extract_ID(void *p, size_t i) {
    ast_node *n = p;
    if (is_terminal(p, "ID")) {
        if (is_terminal(get_nth(p, 0), "ASSIGN")) n->type = ASSIGN;
        else if (is_terminal(get_nth(p, 0), "LSQUARE")) n->type = ARR;
        else n->type = VAR;
        return (return_type){p, 1};
    } else {
        return (return_type) {0, 0};
    }
}

ast_node *declration_reassigner(ast_node *n) {
    ast_node *r = make_expr_with_stack(map(extract_ID, n->expr));
    r->type = DECL;
    return r;
}


/*
 * For arithmetic operation:
 * convert infix expressions to prefix ones and
 * insert tmp_value indicator at proper position for
 * fetching / storing tmp values in assembly code.
 */

int opcmp(ast_node *op1, ast_node *op2) {
    if (!is_nonterminal(op1, "operator") || !is_nonterminal(op2, "operator")) return 0;
    symbol *op = get_symbol_by_code(op1->code);
    int id1, id2;
    for (int i = 0; i < op->productions->length; i++) {
        symbol *s = top(get(op->productions, i));
        if (s->code == ((ast_node *)top(op1->expr))->code) {
            id1 = i;
        }
        if (s->code == ((ast_node *)top(op2->expr))->code) {
            id2 = i;
        }
    }
    return op_index[id1] - op_index[id2];
}

int is_operator(void *p) {
    is_nonterminal(p, "operator");
}

int is_operand(void *p) {
    return !is_operator(p);
}

return_type assign_index(void *p, size_t i) {
    if (is_operator(p))
        return (return_type){(void *)(i-1), 1};
    else
        return (return_type){0,0};
}

void sort(stack *s, stack *indexes) {
    for (int i = 0; i < indexes->length; i++) {
        size_t min = (size_t)get(indexes, i);
        size_t min_idx = i;
        for (int j = i; j < indexes->length; j++) {
            size_t current = (size_t) get(indexes, j);
            if (current < min) {
                min = current;
                min_idx = j;
            }
        }
        swap(s, i, min_idx);
        swap(indexes, i, min_idx);

    }
}

ast_node *exp_to_prefix(ast_node *exp) {
    stack *operands = filter(is_operand, exp->expr);
    stack *operators = filter(is_operator, exp->expr);
    stack *indexes = map(assign_index, exp->expr);
    ast_node *r = make_expr();
//    r->type = ARI;

    int prev = 0;
    for (int i = 1; i < operators->length; i++) {
        int current = i;
        while (prev >= 0 && opcmp(get(operators, prev), get(operators, current)) >= 0) {
//            swap(operators, current, prev);
//            set(indexes, current, get(indexes, prev)+1);
            set(indexes, current, get(indexes, prev));
            set(indexes, prev, get(indexes, prev) + 1);
//            current = prev;
            prev = prev - 1;
        }
//        prev = current;
    }
    sort(operators, indexes);

    int op_i = 0, operand_i = 0;
    for (size_t i = 0; i < operands->length + operators->length; i++) {
        if (op_i >= operators->length || i != (size_t)get(indexes, op_i)) {
            ast_node *operand = get(operands, operand_i++);
            if (operand->type == EXPR) {
                push_node(r, make_node(TMP_VAL));
                cat_stack(r->expr, exp_to_prefix(operand)->expr);
            }
            else {
                if (is_terminal(operand, "INT_NUM")) operand->type = IMMD;
                else if (operand->type == ARR) {
                    operand->offset = atoi(get_nth(operand, 1)->s);
                } else {
                    operand->type = VAR;
                }
                push_node(r, operand);
            }
        } else {
            ast_node *operator = get_nth(get(operators, op_i++), 0);
            operator->type = OP;
            push_node(r, operator);
        }
    }

    return r;
}

/*
 * Shared function for flatting parser tree.
 * Handle some special cases for the convenience of generating
 * actual AST later.
 */

ast_node *flatten(ast_node *exp) {
    if (exp->type == ATOM) return NULL;
    ast_node *s = make_expr();
    s->type = EXPR;
    int rec_flag = 0;

    for (int i = 0; i < exp->expr->length; i++) {
        ast_node *node = get_nth(exp, i);
        if ((node->type != EXPR && node->type != TMP)|| is_operator(node)) {
            if (is_terminal(node, "LPAR") ) rec_flag = 1;
            else if (is_terminal(node, "RPAR") ) rec_flag = 0;
            else if (is_terminal(node, "LBRACE") || is_terminal(node, "RBRACE")) ;
            else if (is_terminal(node, "MINUS") && i == 0) {
                ast_node *neg_one = make_atom(make_symbol(get_token_code("INT_NUM"), TERMINAL));
                neg_one->s = strdup("-1");
                ast_node *op = make_expr_with_symbol(get_symbol("operator"));
                ast_node *mul = make_atom(make_symbol(get_token_code("MUL_OP"), TERMINAL));
                mul->s = strdup("*");
                push_node(op, mul);
                push_node(s, neg_one);
                push_node(s, op);
            } else {
                push_node(s, node);
            }
        }
        else {
            if (is_nonterminal(node, "index")
                || is_nonterminal(node, "declaration_tail")
                || is_nonterminal(node, "assign_op")) {
                ast_node *p = get_nth(s, 0);
                if (is_nonterminal(node, "index")) {p->type = ARR; }
                else if (is_nonterminal(node, "assign_op")) {p->type = ASSIGN; }
                p->expr = make_stack();
                cat_stack(p->expr, flatten(node)->expr);
            }
            else if (!rec_flag
                    && !is_nonterminal(node, "code_block")
                    && !is_nonterminal(node, "if_statement")
                    && !is_nonterminal(node, "do_while_statement")) cat_stack(s->expr, flatten(node)->expr);
            else if (is_nonterminal(node, "code_block")) push_node(s, node);
            else {
                ast_node *l = flatten(node);
//                if (is_nonterminal(node, "exp")) {
//                    l->type = ARI;
//                    l->s = strdup("ARI");
//                }
                push_node(s, l);
            }

        }
    }
    return s;
}

void show_leaves(ast_node *s) {
    for (int i = 0; i < s->expr->length; i++) {
        ast_node *n = get_nth(s, i);
        const char *name = NULL;
        if (n->type == ATOM) name = get_token_name(n->code);
        else if (n->type == EXPR) name = get_symbol_name(n->code);
        else {
            printf("error!\n");
            return;
        }
        printf("%s, ", name);
        printf("\n");
    }
}

void show_prefix(stack *s) {
    for (int i = 0; i < s->length; i++) {
        ast_node *n = get(s, i);
        if (n == NULL) printf("NULL, ");
        else if (n->type == ATOM) printf("%s: %s, ", get_token_name(n->code), n->s);
        else (printf("%s: %s, ", get_symbol_name(n->code), n->s));
    }
}