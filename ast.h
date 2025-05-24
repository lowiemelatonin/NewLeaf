#ifndef AST_H
#define AST_H

typedef enum {
    INT_NODE,
    FLOAT_NODE,
    BOOL_NODE,
    ASSIGN_NODE,
    VAR_NODE,
    BINARY_OP_NODE
}NodeType;

typedef struct ASTNode ASTNode;
typedef struct ASTNode {
    NodeType type;
    union {
        int value;
        float fvalue;
        int bvalue;
        char *var_name;

        struct {
            ASTNode *left;
            ASTNode *right;
        }Assign;

        struct {
            ASTNode *left;
            ASTNode *right;
            char *op;
        }BinaryOp;
    };
}ASTNode;

ASTNode *create_int_node(int value);
ASTNode *create_float_node(float value);
ASTNode *create_bool_node(int bvalue);
ASTNode *create_assign_node(ASTNode *left, ASTNode *right);
ASTNode *create_var_node(const char *var_name);
ASTNode *create_binary_op_node(ASTNode *left, ASTNode *right, const char *op);

void free_ast(ASTNode *node);

#endif