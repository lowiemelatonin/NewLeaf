#ifndef AST_H
#define AST_H

#include <stdbool.h>

typedef enum {
    TYPE_BOOL,
    TYPE_SHORT,
    TYPE_USHORT,
    TYPE_INT,
    TYPE_UINT,
    TYPE_LONG,
    TYPE_ULONG,
    TYPE_LONG_LONG,
    TYPE_ULONG_LONG,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_LONG_DOUBLE,
    TYPE_SIGNED_CHAR,
    TYPE_CHAR_NODE,
    TYPE_UNSIGNED_CHAR,
    TYPE_STRING,
} PrimitiveType;

typedef union {
    bool boolVal;
    short shortVal;
    unsigned short uShortVal;
    int intVal;
    unsigned int uIntVal;
    long longVal;
    unsigned long uLongVal;
    long long longLongVal;
    unsigned long long uLongLongVal;
    float floatVal;
    double doubleVal;
    long double longDoubleVal;
    signed char signedCharVal;
    char charVal;
    unsigned char uCharVal;
    char *stringVal;
} PrimitiveValue;

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
    SIMPLE_ASSIGN,
    ADD_AND_ASSIGN,
    SUB_AND_ASSIGN,
    MUL_AND_ASSIGN,
    DIV_AND_ASSIGN,
    MOD_AND_ASSIGN,
    AND_AND_ASSIGN,
    OR_AND_ASSIGN,
    XOR_AND_ASSIGN,
    SHIFT_LEFT_AND_ASSIGN,
    SHIFT_RIGHT_AND_ASSIGN
} AssignmentOpType;

typedef enum {
    IDENTIFIER_NODE,
    LITERAL_NODE,
    ASSIGNMENT_NODE,
    DECLARATION_NODE,

    POINTER_NODE,
    VOID_NODE,
    NULL_NODE,

    ARRAY_NODE,
    STRUCT_NODE,
    UNION_NODE,
    ENUM_NODE,
    TYPEDEF_NODE,
    IMPL_NODE,

    ARRAY_ACCESS_NODE,
    FIELD_ACCESS_NODE,

    FUNCTION_NODE,
    RETURN_NODE,
    FUNCTION_CALL_NODE,

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
            PrimitiveType type;
            PrimitiveValue value;
        } literal;
        
        struct {
            ASTNode *left;
            ASTNode *right;
            AssignmentOpType op;
        } assignment;

        struct {
            ASTNode *varType;
            char *varName;
            ASTNode *initializer;
        } declaration;

        struct {
            ASTNode *ptr;
        } pointer;
        
        struct {
            ASTNode *typeOf;
        } null;
        
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
ASTNode *createLiteralNode(PrimitiveType type, PrimitiveValue value);
ASTNode *createAssignmentNode(ASTNode *left, ASTNode *right, AssignmentOpType op);
ASTNode *createDeclarationNode(ASTNode *varType, const char *varName, ASTNode *initializer);
ASTNode *createPointerNode(ASTNode *ptrTo);
ASTNode *createNullNode(ASTNode *typeOf);
ASTNode *createVoidNode(void);
ASTNode *createArrayNode(ASTNode *typeOfElement, ASTNode *size);
ASTNode *createStructNode(char *name, ASTNode **fields, int fieldsCount);
ASTNode *createUnionNode(char *name, ASTNode **fields, int fieldsCount);
ASTNode *createEnumNode(char *name, char **values, int *intValues, int valuesCount);
ASTNode *createTypedefNode(char *alias, ASTNode *original);
ASTNode *createImplNode(char *structName, ASTNode **methods, int methodsCount);
ASTNode *createArrayAccessNode(ASTNode *array, ASTNode *index);
ASTNode *createFieldAccessNode(ASTNode *object, char *fieldName, bool isPointerAccess);

#endif