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

#define TMP 1		// register id for holding tmp values
#define ACC 2		// register id for storing accumulated results
#define FP "s8"
FILE *output = NULL;
int stack_offset = 32;

int op_index[] = {1, 2, 3, 4, 5, 5, 6, 6, 6, 6, 7, 7, 8, 8, 9, 9, 10};

//// function for generating load (lw) operation
//void gen_load(ast_node *o, int tmp_reg) {
//    if (output == NULL) return;
//    if (o->type == SYMBOL || o->type == TMP_VAL) {
//        fprintf(output, "lw $t%d, %d($%s)\n", tmp_reg, o->offset + stack_offset, FP);
//    } else if (o->type == IMMEDIATE) {
//        fprintf(output, "li $t%d, %d\n", tmp_reg, o->value.immd);
//    }
//
//    return;
//}

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
    if (is_terminal(p, "ID")) {
        ((ast_node *)p)->type = VAR;
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

ast_node *exp_to_prefix(ast_node *exp) {
    stack *operands = filter(is_operand, exp->expr);
    stack *operators = filter(is_operator, exp->expr);
    stack *indexes = map(assign_index, exp->expr);
    ast_node *r = make_expr();
    r->type = ARI;

    int prev = 0;
    for (int i = 1; i < operators->length; i++) {
        int current = i;
        while (prev >= 0 && opcmp(get(operators, prev), get(operators, current)) >= 0) {
            swap(operators, current, prev);
            set(indexes, current, get(indexes, prev)+1);
            current = prev;
            prev = current - 1;
        }
        prev = current;
    }

    int op_i = 0, operand_i = 0;
    for (size_t i = 0; i < operands->length + operators->length; i++) {
        if (op_i >= operators->length || i != (size_t)get(indexes, op_i)) {
            ast_node *operand = get(operands, operand_i++);
            if (operand->type == ARI) {
                push_node(r, NULL);
                cat_stack(r->expr, operand->expr);
            }
            else {
                if (is_nonterminal(operand, "INT_NUM")) operand->type = IMMD;
                else operand->type = VAR;
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
                ast_node *p = get_nth(node, 0);
                cat_stack(p->expr, flatten(node)->expr);
            }
            else if (!rec_flag
                    && !is_nonterminal(node, "code_block")
                    && !is_nonterminal(node, "if_statement")
                    && !is_nonterminal(node, "do_while_statement")) cat_stack(s->expr, flatten(node)->expr);
            else if (is_nonterminal(node, "code_block")) push_node(s, node);
            else {
                ast_node *l = flatten(node);
                if (is_nonterminal(node, "exp")) {
                    l->type = ARI;
                    l->s = strdup("ARI");
                }
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