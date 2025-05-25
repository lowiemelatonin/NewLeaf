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
        default:
            break;
        }
    free(node);
}