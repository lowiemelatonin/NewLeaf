#include <stdio.h>
#include "ast.h"
#include "hashmap.h"
#include "eval.h"

int main() {
    HashMap *env = create_map(8);

    // Teste: operação binária (3 + 4)
    ASTNode *left = create_int_node(3);
    ASTNode *right = create_int_node(4);
    ASTNode *add = create_binary_op_node(left, right, "+");

    Value result_add = eval(add, env);
    printf("Resultado de 3 + 4: %d\n", result_add.data.i);

    // Teste: operação unária (-7)
    ASTNode *seven = create_int_node(7);
    ASTNode *neg = create_unary_op_node(seven, "-");

    Value result_neg = eval(neg, env);
    printf("Resultado de -7: %d\n", result_neg.data.i);

    // Teste: operação binária booleana (1 && 0)
    ASTNode *bool1 = create_bool_node(1);
    ASTNode *bool0 = create_bool_node(0);
    ASTNode *and_op = create_binary_op_node(bool1, bool0, "&&");

    Value result_and = eval(and_op, env);
    printf("Resultado de 1 && 0: %d\n", result_and.data.i);

    // Teste: operação unária lógica (!1)
    ASTNode *not_op = create_unary_op_node(bool1, "!");
    Value result_not = eval(not_op, env);
    printf("Resultado de !1: %d\n", result_not.data.i);

    // Libera a memória
    free_ast(add);
    free_ast(neg);
    free_ast(and_op);
    free_ast(not_op);
    free_map(env);

    return 0;
}
