#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode *create_int_node(int value){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = INT_NODE;
    node->value = value;
    return node;
}

ASTNode *create_float_node(float fvalue){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = FLOAT_NODE;
    node->fvalue = fvalue;
    return node;
}

ASTNode *create_bool_node(int bvalue){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = BOOL_NODE;
    node->bvalue = bvalue ? 1 : 0;
    return node;
}

ASTNode *create_string_node(char *str){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = STR_NODE;
    node->str = strdup(str);
    return node;
}

ASTNode *create_assign_node(ASTNode *left, ASTNode *right){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = ASSIGN_NODE;
    node->Assign.left = left;
    node->Assign.right = right;
    return node;
}

ASTNode *create_var_node(const char *var_name){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = VAR_NODE;
    node->var_name = strdup(var_name);
    return node;
}

ASTNode *create_unary_op_node(ASTNode *expr, const char *op){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = UNARY_OP_NODE;
    node->UnaryOp.expr = expr;
    node->UnaryOp.op = strdup(op);
    return node;
}

ASTNode *create_binary_op_node(ASTNode *left, ASTNode *right, const char *op){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = BINARY_OP_NODE;
    node->BinaryOp.left = left;
    node->BinaryOp.right = right;
    node->BinaryOp.op = strdup(op);
    return node;
}

ASTNode *create_block_node(ASTNode **nodes, int count){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = BLOCK_NODE;
    node->Block.nodes = malloc(sizeof(ASTNode*) * count);
    
    for(int i = 0; i < count; i++){
        node->Block.nodes[i] = nodes[i];
    }
    node->Block.count = count;
    return node;
}

ASTNode *create_if_else_node(ASTNode *cond, ASTNode *then_block, ASTNode *else_block) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = IF_ELSE_NODE;
    node->IfElse.cond = cond;
    node->IfElse.then_block = then_block;
    node->IfElse.else_block = else_block;
    return node;
}

ASTNode *create_while_node(ASTNode *cond, ASTNode *body){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = WHILE_NODE;
    node->While.cond = cond;
    node->While.body = body;
    return node;
}

ASTNode *create_for_node(ASTNode *init, ASTNode *cond, ASTNode *incr, ASTNode *body){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = FOR_NODE;
    node->For.init = init;
    node->For.cond = cond;
    node->For.incr = incr;
    node->For.body = body;
    return node;
}

void free_ast(ASTNode *node){
    if(!node){
        return;
    }
    switch(node->type){
        case INT_NODE:
            break;
        case FLOAT_NODE:
            break;
        case BOOL_NODE:
            break;
        case STR_NODE:
            free(node->str);
            break;
        case VAR_NODE:
            free(node->var_name);
            break;
        case ASSIGN_NODE:
            free_ast(node->Assign.left);
            free_ast(node->Assign.right);
            break;
        case UNARY_OP_NODE:
            free_ast(node->UnaryOp.expr);
            free(node->UnaryOp.op);
            break;
        case BINARY_OP_NODE:
            free_ast(node->BinaryOp.left);
            free_ast(node->BinaryOp.right);
            free(node->BinaryOp.op);
            break;
        case BLOCK_NODE:
            for(int i = 0; i < node->Block.count; i++){
                free_ast(node->Block.nodes[i]);
            }
            free(node->Block.nodes);
            break;
        case IF_ELSE_NODE:
            free_ast(node->IfElse.cond);
            free_ast(node->IfElse.then_block);
            if (node->IfElse.else_block) {
                free_ast(node->IfElse.else_block);
            }
            break;
        case WHILE_NODE:
            free_ast(node->While.cond);
            free_ast(node->While.body);
            break;
        case FOR_NODE:
            free_ast(node->For.init);
            free_ast(node->For.cond);
            free_ast(node->For.incr);
            free_ast(node->For.body);
            break;
        default:
            break;
        }
    free(node);
}