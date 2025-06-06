#ifndef AST_H
#define AST_H

typedef enum {
    IDENTIFIER_NODE,

    INT_NODE,
    DOUBLE_NODE,
    FLOAT_NODE,
    CHAR_NODE,
    STRING_NODE,

    UNARY_OPERATION_NODE,
    BINARY_OPERATION_NODE,
    TERNARY_OPERATION_NODE,

    ASSIGNMENT_NODE
} NodeType;

struct ASTNode;

typedef struct ASTNode {
    NodeType type;
    union {
        struct {
            char *name;
        } identifier;

        struct {
            int value;
        } intLiteral;

        struct {
            double value; 
        } doubleLiteral;

        struct {
            float value;
        } floatLiteral;

        struct {
            char value;
        } charLiteral;

        struct {
            char *value;
        } stringLiteral;
        
        struct {
            ASTNode *expr;
            char *op;
        } unaryOp;
        
        struct {
            ASTNode *left;
            ASTNode *right;
            char *op;
        } binaryOp;

        struct {
            ASTNode *condition;
            ASTNode *true_expr;
            ASTNode *false_expr;
        } ternaryOp;
        
        struct {
            ASTNode *left;
            ASTNode *right;
        } assignment;
    };

} ASTNode;

#endif