#ifndef AST_H
#define AST_H

typedef enum {
    INT_NODE,
    FLOAT_NODE,
    BOOL_NODE,
    STR_NODE,
    ASSIGN_NODE,
    VAR_NODE,
    UNARY_OP_NODE,
    BINARY_OP_NODE,
    BLOCK_NODE,
    IF_ELSE_NODE,
    WHILE_NODE,
    FOR_NODE,
    DO_WHILE_NODE
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
            ASTNode *expr;
            char *op;
        } UnaryOp;

        struct {
            ASTNode *left;
            ASTNode *right;
            char *op;
        } BinaryOp;

        struct {
            ASTNode **nodes;
            int count;
        } Block;

        struct {
            ASTNode *cond;
            ASTNode *then_block;
            ASTNode *else_block;
        } IfElse;

        struct {
            ASTNode *cond;
            ASTNode *body;
        } While;

        struct {
            ASTNode *init;
            ASTNode *cond;
            ASTNode *incr;
            ASTNode *body;
        } For;

        struct {
            ASTNode *body;
            ASTNode *cond;
        } DoWhile;
    };
} ASTNode;

ASTNode *create_int_node(int value);
ASTNode *create_float_node(float value);
ASTNode *create_bool_node(int bvalue);
ASTNode *create_string_node(char *str);
ASTNode *create_assign_node(ASTNode *left, ASTNode *right);
ASTNode *create_var_node(const char *var_name);
ASTNode *create_unary_op_node(ASTNode *expr, const char *op);
ASTNode *create_binary_op_node(ASTNode *left, ASTNode *right, const char *op);
ASTNode *create_block_node(ASTNode **nodes, int count);
ASTNode *create_if_else_node(ASTNode *cond, ASTNode *then_block, ASTNode *else_block);
ASTNode *create_while_node(ASTNode *cond, ASTNode *body);
ASTNode *create_for_node(ASTNode *init, ASTNode *cond, ASTNode *incr, ASTNode *body);
ASTNode *create_do_while_node(ASTNode *body, ASTNode *cond);

void free_ast(ASTNode *node);

#endif