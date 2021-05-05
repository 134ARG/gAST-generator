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
#include <sys/time.h>
#include <unistd.h>

#define TMP_REG 1		// register id for holding tmp values
#define ACC_REG 2		// register id for storing accumulated results
#define FP "s8"
FILE *output = NULL;
int stack_offset = 32;
size_t max_tmp = 0;
size_t current_tmp = 0;
size_t max_var_offset = 0;
size_t current_var_offset = 0;
stack variables;

typedef enum {OBJ, IMMEDIATE} var_type;

void gen_statement(ast_node *statements);

void set_file(const char *file_name) {
    if (!file_name) output = stdout;
    else output = fopen(file_name, "w");
}

void code_gen_main(ast_node *tree, const char *output) {
    set_file(output);
    add_extern();
    gen_main_prologue();
    gen_symbol_list(tree);
    gen_statements(tree);
    gen_main_epilogue();
    copy_read();
    copy_write();
}



typedef struct variable {
    char *name;
    int offset;
    int type;
} variable;

variable *make_variable(const char *name, int type, size_t num) {
    variable *p = malloc(sizeof(variable));
    p->name = strdup(name);
    p->type = type;
    p->offset = current_var_offset;
    current_var_offset += num*4;
    return p;
}

variable *get_variable(const char *name) {
    for (int i = 0; i < variables.length; i++) {
        variable *v = get(&variables, i);
        if (!strcmp(name, v->name)) {
            return v;
        }
    }
    printf("NO\n");
    return NULL;
}

int get_offset(ast_node *n) {
    if (n->type == ARR || n->type == VAR || n->type == ASSIGN || n->type == ATOM){
        return get_variable(n->s)->offset;
    }
    else if (n->type == TMP_VAL){
        int r = n->offset + max_var_offset;
//        n->offset = 0;
        return max_var_offset;
    } else
        return 0;
}

void add_to_list(variable *v) {
    push(&variables, v);
}

int op_index[] = {1, 2, 3, 4, 5, 5, 6, 6, 6, 6, 7, 7, 8, 8, 9, 9, 10};

void gen_satements(ast_node *statements);

const char*gen_label() {
    char str[64];
    usleep(10);
    struct timeval start;
    gettimeofday(&start, NULL);

//    printf("!!!!!!!%lu\n", start.tv_sec*1000000+start.tv_usec);
    sprintf(str, "%lu", start.tv_sec*1000000+start.tv_usec);
    str[0] = 'A';
    return strdup(str);
}

// function for generating load (lw) operation
void gen_load(ast_node *o, int tmp_reg) {
    if (output == NULL) return;
    int offset = get_offset(o) + o->offset + stack_offset;
    if (o->type == VAR || o->type == TMP_VAL) {
        fprintf(output, "lw $t%d, %d($%s)\n", tmp_reg, offset, FP);
    } else if (o->type == ARR) {
        gen_exp(get_nth(o, 0));
        fprintf(output, "add $t3, $t%d, $zero\n", ACC_REG);
        fprintf(output, "addi $t4, $zero, 4\n");
        fprintf(output, "mul $t3, $t3, $t4\n");
        fprintf(output, "addi $t3, $t3, %d\n", get_offset(o)+stack_offset);
        fprintf(output, "add $t3, $t3, $%s\n", FP);
        fprintf(output, "lw $t%d, 0($t3)\n", tmp_reg);
    }
    else if (o->type == IMMD) {
        fprintf(output, "li $t%d, %d\n", tmp_reg, o->offset);
    }
}

void gen_li(int immd, int tmp_reg) {
    if (output == NULL) return;
    fprintf(output, "li $t%d, %d\n", tmp_reg, immd);
}

// function for generating save (sw) operation
void gen_save(ast_node *o, int tmp_reg) {
    if (output == NULL) return;
    int offset =  get_offset(o) + o->offset +stack_offset;
    if (o->type != IMMD) {
        fprintf(output, "sw $t%d, %d($%s)\n", tmp_reg, offset, FP);
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
        gen_save(n, TMP_REG);
    }
    else {
        printf("errorrrrrrrrrrr!\n");
    }
    return (return_type) {0, 0};
}

stack *gen_symbol_list(ast_node *tree) {
//    output = stdout;
    init_stack(&variables);
    ast_node *decl = find(select_decl, tree->expr);
    destruct_and_free(map(add_symbol, decl->expr));
}

void gen_if_stmt(ast_node *if_stmt) {
    stack *raw = if_stmt->expr;
    const char *label = gen_label();

    gen_exp(get(raw, 0));
    gen_li(0, TMP_REG);
    fprintf(output, "beq $t%d, $t%d, %s\n", TMP_REG, ACC_REG, label);
    gen_li(0, TMP_REG);
    gen_statement(get(raw, 1));
    fprintf(output, "%s:\n", label);
    if (raw->length > 2)
        gen_statement(get(raw, 2));
}

void gen_while(ast_node *while_stmt) {

    const char *begin = gen_label();
    stack *raw = while_stmt->expr;
    const char *end = gen_label();

    fprintf(output, "%s:\n", begin);
    gen_exp(get(raw, 1));
    gen_li(0, TMP_REG);
    fprintf(output, "beq $t%d, $t%d, %s\n", TMP_REG, ACC_REG, end);
    gen_li(0, TMP_REG);
    gen_statement(get(raw, 2));
    fprintf(output, "j %s\n", begin);
    fprintf(output, "%s:\n", end);
}

void gen_do_while(ast_node *do_while_stmt) {
    stack *raw = do_while_stmt->expr;
    const char *begin = gen_label();
    const char *end = gen_label();

    fprintf(output, "%s:\n", begin);
    gen_statement(get(raw, 1));
    gen_exp(get(raw, 3));
    gen_li(0, TMP_REG);
    fprintf(output, "beq $t%d, $t%d, %s\n", TMP_REG, ACC_REG, end);
    gen_li(0, TMP_REG);
    fprintf(output, "j %s\n", begin);
    fprintf(output, "%s:\n", end);
}

void gen_return() {
    fprintf(output, "li $v0, 10\nsyscall\n");
}

void gen_write(ast_node *write_stmt) {
    gen_exp(get_nth(write_stmt, 1));

    fprintf(output, "sw $t%d, 0($%s)\n", ACC_REG, FP);
    fprintf(output, "lw $a0, 0($%s)\n", FP);
    fprintf(output, "jal __write\n");
}

void gen_read(ast_node *read_stmt) {
    fprintf(output, "jal __read\n");
    fprintf(output, "add $t%d, $v0, $zero\n", ACC_REG);
    gen_save(get_nth(read_stmt, 1), ACC_REG);
}

void gen_assignment(ast_node *assign) {
    ast_node *start = get_nth(assign, 0);
    if (is_terminal(start, "LSQUARE")) {
        gen_exp(get_nth(assign, 1));
        fprintf(output, "add $t3, $t%d, $zero\n", ACC_REG);
        fprintf(output, "addi $t4, $zero, 4\n");
        fprintf(output, "mul $t3, $t3, $t4\n");
        fprintf(output, "addi $t3, $t3, %d\n", get_offset(assign)+stack_offset);
        fprintf(output, "add $t3, $t3, $%s\n", FP);
        gen_exp(get_nth(assign, 4));
        fprintf(output, "sw $t%d, 0($t3)\n", ACC_REG);
    } else {
        gen_exp(get_nth(assign, 1));
        gen_save(assign, ACC_REG);
    }
}

return_type select_generator(void *p, size_t i) {
    ast_node *n = p;
    if (n->type == IF) {
        gen_if_stmt(n);
    } else if (n->type == WHILE) {
        gen_while(n);
    } else if (n->type == DO) {
        gen_do_while(n);
    } else if (n->type == RETURN) {
        gen_return();
    } else if (n->type == WRITE) {
        gen_write(n);
    } else if (n->type == READ) {
        gen_read(n);
    } else if (n->type == ASSIGN) {
        gen_assignment(n);
    }
    return (return_type) {0, 0};
}

return_type select_stmt(void *p, size_t i) {
    ast_node *n = p;
    if (n->type == STATEMENT) {
        return (return_type) {p, 1};
    } else {
        return (return_type) {0, 0};
    }
}

void gen_statement(ast_node *statements) {
    destruct_and_free(map(select_generator, statements->expr));
}

void gen_statements(ast_node *tree) {
    ast_node *stmts = find(select_stmt, tree->expr);
    destruct_and_free(map(select_generator, stmts->expr));
}


void gen_single_ari(ast_node *op, ast_node *operand1, ast_node *operand2) {
    int reg1, reg2;

    if (operand1 == NULL && operand2 != NULL) {
        gen_load(operand2, TMP_REG);
        reg1 = ACC_REG;
        reg2 = TMP_REG;
    } else if (operand1 != NULL && operand2 == NULL) {
        gen_load(operand1, TMP_REG);
        reg1 = TMP_REG;
        reg2 = ACC_REG;
    } else {
        gen_load(operand1, TMP_REG);
        gen_load(operand2, ACC_REG);
        reg1 = TMP_REG;
        reg2 = ACC_REG;
    }

    if (is_terminal(op, "MINUS")) {
        fprintf(output, "sub $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "PLUS")) {
        fprintf(output, "add $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "OROR") || is_terminal(op, "OR_OP")) {
        fprintf(output, "or $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "ANDAND") || is_terminal(op, "AND_OP")) {
        fprintf(output, "and $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "EQ")) {
        fprintf(output, "seq $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "NOTEQ")) {
        fprintf(output, "sne $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "LT")) {
        fprintf(output, "slt $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "GT")) {
        fprintf(output, "sgt $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "LTEQ")) {
        fprintf(output, "sle $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "GTEQ")) {
        fprintf(output, "sge $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "SHL_OP")) {
        fprintf(output, "sllv $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "SHR_OP")) {
        fprintf(output, "srav $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "MUL_OP")) {
        fprintf(output, "mul $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "DIV_OP")) {
        fprintf(output, "div $t%d, $t%d, $t%d\n", ACC_REG, reg1, reg2);
    } else if (is_terminal(op, "NOT_OP")) {

    }

}

void gen_exp(ast_node *ari) {
//    output = stdout;
    stack *raw = ari->expr;
    stack *supp = make_stack();
    current_tmp = 0;
    int operand_count = 0;

    if (raw->length == 1) {
        gen_load(get_nth(ari, 0), ACC_REG);
        return;
    }

    for (int i = 0; i < raw->length; i++) {
        ast_node *current = get(raw, i);

        if (current->type == OP) {
            operand_count = 0;
            if (!top(supp) && supp->length) {
                pop(supp);
                ast_node *tmp = make_node(TMP_VAL);
                tmp->offset = current_tmp;
                current_tmp += 4;
                push(supp, tmp);
                gen_save(tmp, ACC_REG);
            }
        }
        else operand_count++;
        push(supp, current);

        while (operand_count == 2 && supp->length) {
            ast_node *operand2 = pop(supp);
            ast_node *operand1 = pop(supp);
            ast_node *op = pop(supp);

            gen_single_ari(op, operand1, operand2);
            if (supp->length) {
                if (((ast_node *)top(supp))->type != OP) operand_count = 2;
                else operand_count = 1;
                push(supp, NULL);
            }

        }
    }

    if (supp->length) exit(1);
}

void debug_print_variables() {
    printf("tmp_max: %lu\n", max_tmp);
    destruct_and_free(map(show_variables, &variables));
}


// function for generating the prologue part of the main function
void gen_main_prologue() {

    int stack_size = max_var_offset + max_tmp + stack_offset;

    fprintf(output, ".text\n.globl main\n.ent main\n");
    fprintf(output, "main:\n");
    fprintf(output, ".set noreorder\n");
    fprintf(output, ".frame $%s, %d, $ra\n", FP, stack_size);
    fprintf(output, ".cpload $t9\n");
    fprintf(output, ".set reorder\n");
    fprintf(output, "addi $sp, $sp, -%d\n", stack_size);
    fprintf(output, ".cprestore 24\n");
    fprintf(output, "sw $ra, 20($sp)\n");
    fprintf(output, "sw $%s, 28($sp)\n", FP);
    fprintf(output, "move $%s, $sp\n", FP);

}

// function for generating the epilogue part of the main function
// be aware that it should be used together with gem_main_prologue()
void gen_main_epilogue() {
    int stack_size = max_var_offset + max_tmp + stack_offset;
    fprintf(output, "main_$$epilogue:\n");
    fprintf(output, "move $sp, $%s\n", FP);
    fprintf(output, "lw $%s, 28($sp)\n", FP);
    fprintf(output, "lw $ra, 20($sp)\n");
    fprintf(output, "addi $sp, $sp, %d\n", stack_size);
    fprintf(output, "jr $ra\n");
    fprintf(output, ".end main\n");
}

void copy_write() {
    FILE *f = fopen("./write.S", "r");
    char ch;
    while (fread(&ch, sizeof(char), 1, f) != 0)
        fwrite(&ch, sizeof(char), 1, output);

}

void copy_read() {
    FILE *f = fopen("./read.S", "r");
    char ch;
    while (fread(&ch, sizeof(char), 1, f) != 0)
        fwrite(&ch, sizeof(char), 1, output);

}

void add_extern() {
    fprintf(output, ".extern scanf\n.extern printf\n");
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
    if (!is_token(p)) return 0;
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
    return r;
}

/*
 * For declarations:
 * Flatten and store all info needed for variable declaration.
 */

return_type extract_ID(void *p, size_t i) {
    int size = 0;
    ast_node *n = p;
    if (is_terminal(p, "ID")) {
        if (is_terminal(get_nth(p, 0), "ASSIGN")) n->type = ASSIGN;
        else if (is_terminal(get_nth(p, 0), "LSQUARE")) {
            n->type = ARR;
            max_var_offset += atoi(get_nth(n, 1)->s)*4-4;
        }
        else n->type = VAR;
        max_var_offset += 4;
//        add_symbol(p, i);
        return (return_type){p, 1};
    } else {
        return (return_type) {0, 0};
    }
}

ast_node *declration_reassigner(ast_node *n) {
//    init_stack(&variables);
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
    int tmp_usage = 0;
//    r->type = ARI;

    for (int i = 1; i < operators->length; i++) {
        int current = i;
        int prev = current - 1;
        while (prev >= 0 && opcmp(get(operators, prev), get(operators, current)) >= 0
                && get(indexes, prev) < get(indexes, current)) {
//            swap(operators, current, prev);
//            set(indexes, current, get(indexes, prev)+1);
            for (int j = 0; j <= i; j++) {
                if (get(indexes, j) > get(indexes, prev) && get(indexes, j) < get(indexes, current)) {
                    set(indexes, j, get(indexes, j) + 1);
                }
            }
            set(indexes, current, get(indexes, prev));
            set(indexes, prev, get(indexes, prev) + 1);
//            current = prev;
            prev = prev - 1;
        }
//        prev = current;
    }
    sort(operators, indexes);

    int op_i = 0, operand_i = 0;
    int operand_count = 0;
    for (size_t i = 0; i < operands->length + operators->length; i++) {
        if (op_i >= operators->length || i != (size_t)get(indexes, op_i)) {
            ast_node *operand = get(operands, operand_i++);
            operand_count++;
            if (operand->type == EXPR) {
                cat_stack(r->expr, exp_to_prefix(operand)->expr);
                if (operand_count >= 2) tmp_usage += 4;
            }
            else {
                if (is_terminal(operand, "INT_NUM")) {
                    operand->type = IMMD;
                    operand->offset = atoi(operand->s);
                }
//                else if (operand->type == ARR) {
//                    operand->offset = atoi(get_nth(operand, 1)->s);
//                }
                else if (operand->type != ARR) {
                    operand->type = VAR;
                }
                push_node(r, operand);
            }
        } else {
            if (operand_count >= 2) tmp_usage += 4;
            operand_count = 0;
            ast_node *operator = get_nth(get(operators, op_i++), 0);
            operator->type = OP;
            push_node(r, operator);
        }
    }

    if (tmp_usage > max_tmp) max_tmp = tmp_usage;

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
                if (is_nonterminal(node, "index")) {
                    p->type = ARR;
                    p->expr = make_stack();
                    ast_node *l = get_nth(node, 1);
                    ast_node *t = flatten(l);
                    t->code = get_symbol("exp")->code;
                    push_node(p, t);
                    continue;
                }
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