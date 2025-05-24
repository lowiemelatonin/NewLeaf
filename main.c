#include "ast.h"

int main(){
    ASTNode *var = create_var_node("x");
    ASTNode *value = create_int_node(5);
    ASTNode *assign = create_assign_node(var, value);
    free_ast(assign);
    return 0;
}