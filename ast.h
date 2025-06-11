#ifndef AST_H
#define AST_H

#include <stdbool.h>

typedef enum {
    POSITIVE_UNOP,
    NEGATIVE_UNOP,
    NOT_UNOP,
    BIT_NOT_UNOP,

    PRE_INCREMENT_UNOP,
    POST_INCREMENT_UNOP,
    PRE_DECREMENT_UNOP,
    POST_DECREMENT_UNOP,

    DEFERENCE_UNOP,
    ADDRESS_OF_UNOP,
    SIZE_OF_UNOP
} UnaryOpType;

typedef enum {
    ADD_BINOP,
    SUB_BINOP,
    MUL_BINOP,
    DIV_BINOP,
    MOD_BINOP,

    AND_BINOP,
    OR_BINOP,

    BIT_AND_BINOP,
    BIT_OR_BINOP,
    BIT_XOR_BINOP,

    SHIFT_LEFT_BINOP,
    SHIFT_RIGHT_BINOP,

    EQU_BINOP,
    NOT_EQU_BINOP,
    LESS_BINOP,
    LESS_EQU_BINOP,
    GREATER_BINOP,
    GREATER_EQU_BINOP,

    COMMA_BINOP
} BinaryOpType;

typedef enum {
    IDENTIFIER_NODE,
    ASSIGNMENT_NODE,
    DECLARATION_NODE,

    BOOL_NODE,
    SHORT_NODE,
    UNSIGNED_SHORT_NODE,
    INT_NODE,
    UNSIGNED_INT_NODE,
    LONG_NODE,
    UNSIGNED_LONG_NODE,
    LONG_LONG_NODE,
    UNSIGNED_LONG_LONG_NODE,
    FLOAT_NODE,
    DOUBLE_NODE,
    LONG_DOUBLE_NODE,
    SIGNED_CHAR_NODE,
    CHAR_NODE,
    UNSIGNED_CHAR_NODE,
    STRING_NODE,
    POINTER_NODE,
    VOID_NODE,
    NULL_NODE,

    ARRAY_NODE,
    STRUCT_NODE,
    UNION_NODE,
    ENUM_NODE,
    TYPEDEF_NODE,

    FUNCTION_NODE,
    RETURN_NODE,
    FUNCTION_CALL_NODE,
    IMPL_NODE,

    LABEL_NODE,
    JUMP_NODE,

    MALLOC_NODE,
    CALLOC_NODE,
    REALLOC_NODE,
    FREE_NODE,

    UNARY_OPERATION_NODE,
    BINARY_OPERATION_NODE,
    TERNARY_OPERATION_NODE,

    BLOCK_NODE,
    COMPOUND_EXPR_NODE,
    CAST_EXPR_NODE,

    IF_NODE,
    SWITCH_NODE,
    CASE_NODE,
    DEFAULT_NODE,

    WHILE_NODE,
    DO_WHILE_NODE,
    FOR_NODE,

    BREAK_NODE,
    CONTINUE_NODE,

    TRY_NODE,
    CATCH_NODE,
    THROW_NODE

} NodeType;

typedef struct ASTNode ASTNode;

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
            ASTNode *varType;
            char *varName;
            ASTNode *initializer;
        } declaration;

        struct {
            bool value;
        } boolLiteral;

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
            long double value; 
        } longDoubleLiteral;

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
            ASTNode *typeOfElement;
            ASTNode *size;
        } array;
                
        struct {
            char *name;
            ASTNode **fields;
            int fieldsCount;
        } structDef;
                
        struct {
            char *name;
            ASTNode **fields;
            int fieldsCount;
        } unionDef;
                
        struct {
            char *name;
            char **values;
            int *intValues;
            int valuesCount;
        } enumDef;
                
        struct {
            char *alias;
            ASTNode *original;
        } typedefDef;

        struct {
            char *structName;
            ASTNode **methods;
            int methodsCount;
        } implDef;
        
        struct {
            ASTNode *array;
            ASTNode *index;
        } arrayAccess;

        struct {
            ASTNode *object;
            char *fieldName;
            bool isPointerAccess;
        } fieldAccess;
        
        struct {
            char *name;
            ASTNode *returnType;
            ASTNode **params;
            int paramCount;
            ASTNode **body;
            int bodyCount;
        } functionDef;
        
        struct {
            ASTNode *value;
        } returnStmt;

        struct {
            ASTNode *function;
            ASTNode **args;
            int argsCount;
        } functionCall;

        struct {
            char *labelName;
        } labelStmt;

        struct {
            char *labelName;
        } jumpStmt;

        struct {
            ASTNode *sizeExpr;
        } mallocExpr;

        struct {
            ASTNode *num;
            ASTNode *size;
        } callocExpr;

        struct {
            ASTNode *ptr;
            ASTNode *sizeExpr;
        } reallocExpr;

        struct {
            ASTNode *ptr;
        } freeExpr;

        struct {
            ASTNode *expr;
            UnaryOpType op;
        } unaryOp;
        
        struct {
            ASTNode *left;
            ASTNode *right;
            BinaryOpType op;
        } binaryOp;

        struct {
            ASTNode *condition;
            ASTNode *trueExpr;
            ASTNode *falseExpr;
        } ternaryOp;

        struct {
            ASTNode **statements;
            int stmtCount;
        } block;

        struct {
            ASTNode **statements;
            int stmtCount;
        } compoundExpr;

        struct {
            ASTNode *targetType;
            ASTNode *value;
        } castExpr;

        struct {
            ASTNode *condition;
            ASTNode *thenBranch;
            ASTNode *elseBranch;
        } ifStmt;

        struct {
            ASTNode *expr;
            ASTNode **cases;
            int caseCount;
        } switchStmt; 

        struct {
            ASTNode *value;
            ASTNode **body;
            int bodyCount;
        } caseStmt;
        
        struct {
            ASTNode **body;
            int bodyCount;
        } defaultStmt;
        
        struct {
            ASTNode *condition;
            ASTNode **body;
            int bodyCount;
        } whileStmt;
        
        struct {
            ASTNode **body;
            int bodyCount;
            ASTNode *condition;
        } doWhileStmt;

        struct {
            ASTNode *initializer;
            ASTNode *condition;
            ASTNode *increment;
            ASTNode **body;
            int bodyCount;
        } forStmt;

        struct {
            ASTNode **tryBlock;
            int tryBlockCount;
            ASTNode **catchBlocks;
            int catchCount;
        } tryStmt;

        struct {
            char *exceptionVarName;
            ASTNode **body;
            int bodyCount;
        } catchStmt;

        struct {
            ASTNode *exceptionExpr;
        } throwStmt;
        
    };

} ASTNode;

ASTNode *createIdentifierNode(const char *name);
ASTNode *createAssignmentNode(ASTNode *left, ASTNode *right);
ASTNode *createDeclarationNode(ASTNode *varType, const char *varName, ASTNode *initializer);
ASTNode *createBoolNode(bool value);
ASTNode *createShortNode(short value);
ASTNode *createUnsignedShortNode(unsigned short value);

#endif