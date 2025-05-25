#ifndef AST_H
#define AST_H

typedef enum {
    INT_NODE,
    FLOAT_NODE,
    BOOL_NODE,
    STR_NODE,
    ASSIGN_NODE,
    VAR_NODE,
    BINARY_OP_NODE,
    BLOCK_NODE
} NodeType;

typedef struct ASTNode ASTNode;
typedef struct ASTNode {
    NodeType type;
    union {
        int value;
        float fvalue;
        int bvalue;
        char *str;
        char *var_name;

        struct {
            ASTNode *left;
            ASTNode *right;
        } Assign;

        struct {
            ASTNode *left;
            ASTNode *right;
            char *op;
        } BinaryOp;

        struct {
            ASTNode **nodes;
            int count;
        } Block;
        
    };
} ASTNode;

ASTNode *create_int_node(int value);
ASTNode *create_float_node(float value);
ASTNode *create_bool_node(int bvalue);
ASTNode *create_string_node(char *str);
ASTNode *create_assign_node(ASTNode *left, ASTNode *right);
ASTNode *create_var_node(const char *var_name);
ASTNode *create_binary_op_node(ASTNode *left, ASTNode *right, const char *op);
ASTNode *create_block_node(ASTNode **nodes, int count);

void free_ast(ASTNode *node);

#endif