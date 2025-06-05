#ifndef AST_H
#define AST_H

typedef enum {
    UNARY_OPERATION_NODE,
    BINARY_OPERATION_NODE
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        struct {
            ASTNode *expr;
            char *op;
        } unaryOp;
        
        struct {
            ASTNode *left;
            ASTNode *right;
            char *op;
        } binaryOp;

    };

} ASTNode;

#endif