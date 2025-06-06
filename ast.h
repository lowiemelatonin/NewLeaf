#ifndef AST_H
#define AST_H

typedef enum {
    IDENTIFIER_NODE,
    ASSIGNMENT_NODE,

    SHORT_NODE,
    UNSIGNED_SHORT_NODE,
    INT_NODE,
    UNSIGNED_INT_NODE,
    LONG_NODE,
    UNSIGNED_LONG_NODE,
    LONG_LONG_NODE,
    FLOAT_NODE,
    DOUBLE_NODE,
    SIGNED_CHAR_NODE,
    CHAR_NODE,
    UNSIGNED_CHAR_NODE,
    STRING_NODE,
    POINTER_NODE,
    VOID_NODE,

    UNARY_OPERATION_NODE,
    BINARY_OPERATION_NODE,
    TERNARY_OPERATION_NODE

} NodeType;

struct ASTNode;

typedef struct ASTNode {
    NodeType type;
    union {
        struct {
            char *name;
        } identifier;

        struct {
            ASTNode *left;
            ASTNode *right;
        } assignment;

        struct {
            short value;
        } shortLiteral;

        struct {
            unsigned short value;
        } unsignedShortLiteral;

        struct {
            int value;
        } intLiteral;

        struct {
            unsigned int value;
        } unsignedIntLiteral;

        struct {
            long value;
        } longLiteral;

        struct {
            unsigned long value;
        } unsignedLongLiteral;

        struct {
            long long value;
        } longLongLiteral;

        struct {
            unsigned long long value;
        } unsignedLongLongLiteral;

        struct {
            float value;
        } floatLiteral;

        struct {
            double value; 
        } doubleLiteral;

        struct {
            signed char value;
        } signedCharLiteral;

        struct {
            char value;
        } charLiteral;

        struct {
            unsigned char value;
        } unsignedCharLiteral;

        struct {
            char *value;
        } stringLiteral;
        
        struct {
            ASTNode *ptr;
        } pointer;
        
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

    };

} ASTNode;

#endif