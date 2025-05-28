#include <stdio.h>
#include "ast.h"
#include "hashmap.h"
#include "eval.h"

int main() {
    HashMap *env = create_map(8);
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
    free_ast(and_op);
    free_ast(not_op);
    free_map(env);

    return 0;
}
