#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static ASTNode *allocNode(NodeType type){
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;
    
    node->type = type;
    return node;
};

ASTNode *createIdentifierNode(const char *name){
    ASTNode *node = allocNode(IDENTIFIER_NODE);
    if(!node) return NULL;

    node->identifier.name = strdup(name);
    if(!node->identifier.name){
        free(node);
        return NULL;
    }
    return node;
}

ASTNode *createLiteralNode(PrimitiveType type, PrimitiveValue value){
    ASTNode *node = allocNode(LITERAL_NODE);
    if(!node) return NULL;

    node->literal.type = type;    
    if(type == TYPE_STRING){
        node->literal.value.stringVal = strdup(value.stringVal);
        if(!node->literal.value.stringVal){
            free(node);
            return NULL;
        }
    } else{
        node->literal.value = value;
    }
    return node;
}

ASTNode *createAssignmentNode(ASTNode *left, ASTNode *right, AssignmentOpType op){
    ASTNode *node = allocNode(ASSIGNMENT_NODE);
    if(!node) return NULL;

    node->assignment.left = left;
    node->assignment.right = right;
    node->assignment.op = op;
    return node;
}

ASTNode *createDeclarationNode(ASTNode *varType, const char *varName, ASTNode *initializer){
    ASTNode *node = allocNode(DECLARATION_NODE);
    if(!node) return NULL;

    node->declaration.varType = varType;
    node->declaration.varName = strdup(varName);
    if(!node->declaration.varName){
        free(node);
        return NULL;
    }
    node->declaration.initializer = initializer;
    return node;
}

ASTNode *createPointerNode(ASTNode *ptrTo){
    ASTNode *node = allocNode(POINTER_NODE);
    if(!node) return NULL;

    node->pointer.ptr = ptrTo;
    return node;
}

ASTNode *createNullNode(ASTNode *typeOf){
    ASTNode *node = allocNode(NULL_NODE);
    if(!node) return NULL;

    node->null.typeOf = typeOf;
    return node;
};

ASTNode *createVoidNode(void){
    ASTNode *node = allocNode(VOID_NODE);
    if(!node) return NULL;

    return node;
}

ASTNode *createArrayNode(ASTNode *typeOfElement, ASTNode *size, ASTNode **elements, int elementsCount){
    ASTNode *node = allocNode(ARRAY_NODE);
    if(!node) return NULL;

    node->array.typeOfElement = typeOfElement;
    node->array.size = size;
    if(elementsCount > 0){
        node->array.elements = malloc(elementsCount * sizeof(ASTNode *));
        if(!node->array.elements){
            free(node);
            return NULL;
        }
        for(int i = 0; i < elementsCount; i++){
            node->array.elements[i] = elements[i];
        }
    }else node->array.elements = NULL;

    node->array.elementsCount = elementsCount;
    return node;
}

ASTNode *createStructNode(char *name, ASTNode **fields, int fieldsCount){
    ASTNode *node = allocNode(STRUCT_NODE);
    if(!node) return NULL;

    node->structDef.name = strdup(name);
    if(!node->structDef.name){
        free(node);
        return NULL;
    }
    node->structDef.fields = fields;
    node->structDef.fieldsCount = fieldsCount;
    return node;
}

ASTNode *createUnionNode(char *name, ASTNode **fields, int fieldsCount){
    ASTNode *node = allocNode(UNION_NODE);
    if(!node) return NULL;

    node->unionDef.name = strdup(name);
    if(!node->unionDef.name){
        free(node);
        return NULL;
    }
    node->unionDef.fields = fields;
    node->unionDef.fieldsCount = fieldsCount;
    return node;
}

ASTNode *createEnumNode(char *name, char **values, int *intValues, int valuesCount){
    ASTNode *node = allocNode(ENUM_NODE);
    if(!node) return NULL;

    node->enumDef.name = strdup(name);
    if(!node->enumDef.name){
        free(node);
        return NULL;
    }

    node->enumDef.values = malloc(sizeof(char*) * valuesCount);
    if(!node->enumDef.values){
        free(node->enumDef.name);
        free(node);
        return NULL;
    }

    for(int i = 0; i < valuesCount; i++){
        node->enumDef.values[i] = strdup(values[i]);
        if(!node->enumDef.values[i]){
            for(int j = 0; j < i; j++){
                free(node->enumDef.values[j]);
            }
            free(node->enumDef.values);
            free(node->enumDef.name);
            free(node);
            return NULL;
        }
    }

    node->enumDef.intValues = malloc(sizeof(int) * valuesCount);
    if(!node->enumDef.intValues){
        for(int i = 0; i < valuesCount; i++){
            free(node->enumDef.values[i]);
        }
        free(node->enumDef.values);
        free(node->enumDef.name);
        free(node);
        return NULL;
    }
    memcpy(node->enumDef.intValues, intValues, sizeof(int) * valuesCount);

    node->enumDef.valuesCount = valuesCount;
    return node;
}

ASTNode *createTypedefNode(char *alias, ASTNode *original){
    ASTNode *node = allocNode(TYPEDEF_NODE);
    if(!node) return NULL;

    node->typedefDef.alias = strdup(alias);
    if(!node->typedefDef.alias){
        free(node);
        return NULL;
    }
    node->typedefDef.original = original;
    return node;
}

ASTNode *createImplNode(char *structName, ASTNode **methods, int methodsCount){
    ASTNode *node = allocNode(IMPL_NODE);
    if(!node) return NULL;

    node->implDef.structName = strdup(structName);
    if(!node->implDef.structName){
        free(node);
        return NULL;
    }

    node->implDef.methods = malloc(methodsCount * sizeof(ASTNode *));
    if(!node->implDef.methods){
        free(node->implDef.structName);
        free(node);
        return NULL;
    }

    for(int i = 0; i < methodsCount; i++){
        node->implDef.methods[i] = methods[i];
    }

    node->implDef.methodsCount = methodsCount;
    return node;
}

ASTNode *createArrayAccessNode(ASTNode *array, ASTNode *index){
    ASTNode *node = allocNode(ARRAY_ACCESS_NODE);
    if(!node) return NULL;

    node->arrayAccess.array = array;
    node->arrayAccess.index = index;
    return node;
}

ASTNode *createFieldAccessNode(ASTNode *object, char *fieldName, bool isPointerAccess){
    ASTNode *node = allocNode(FIELD_ACCESS_NODE);
    if(!node) return NULL;

    node->fieldAccess.object = object;
    node->fieldAccess.fieldName = strdup(fieldName);
    if(!node->fieldAccess.fieldName){
        free(node);
        return NULL;
    }

    node->fieldAccess.isPointerAccess = isPointerAccess;
    return node;
}

ASTNode *createFunctionNode(char *name, ASTNode *returnType, ASTNode **params, int paramCount, ASTNode **body, int bodyCount){
    ASTNode *node = allocNode(FUNCTION_NODE);
    if(!node) return NULL;

    node->functionDef.name = strdup(name);
    if(!node->functionDef.name){
        free(node);
        return NULL;
    }

    node->functionDef.returnType = returnType;
    
    node->functionDef.params = malloc(paramCount * sizeof(ASTNode *));
    if(!node->functionDef.params){
        free(node->functionDef.name);
        free(node);
        return NULL;
    }
    for(int i = 0; i < paramCount; i++){
        node->functionDef.params[i] = params[i];
    }
    node->functionDef.paramCount = paramCount;

    node->functionDef.body = malloc(bodyCount * sizeof(ASTNode *));
    if(!node->functionDef.body){
        free(node->functionDef.params);
        free(node->functionDef.name);
        free(node);
        return NULL;
    }
    for(int i = 0; i < bodyCount; i++){
        node->functionDef.body[i] = body[i];
    }
    node->functionDef.bodyCount = bodyCount;
    return node;
}

ASTNode *createReturnNode(ASTNode *value){
    ASTNode *node = allocNode(RETURN_NODE);
    if(!node) return NULL;

    node->returnStmt.value = value;
    return node;
}

ASTNode *createFunctionCallNode(ASTNode *function, ASTNode **args, int argsCount){
    ASTNode *node = allocNode(FUNCTION_CALL_NODE);
    if(!node) return NULL;

    node->functionCall.function = function;

    node->functionCall.args = malloc(argsCount * sizeof(ASTNode *));
    if(!node->functionCall.args){
        free(node);
        return NULL;
    }
    for(int i = 0; i < argsCount; i++){
        node->functionCall.args[i] = args[i];
    }
    node->functionCall.argsCount = argsCount;
    return node;
}

ASTNode *createLabelNode(char *labelName){
    ASTNode *node = allocNode(LABEL_NODE);
    if(!node) return NULL;

    node->labelStmt.labelName = strdup(labelName);
    if(!node->labelStmt.labelName){
        free(node);
        return NULL;
    }
    return node;
}

ASTNode *createJumpNode(char *labelName){
    ASTNode *node = allocNode(JUMP_NODE);
    if(!node) return NULL;

    node->jumpStmt.labelName = strdup(labelName);
    if(!node->jumpStmt.labelName){
        free(node);
        return NULL;
    }
    return node;
}

ASTNode *createMallocExprNode(ASTNode *size){
    ASTNode *node = allocNode(MALLOC_NODE);
    if(!node) return NULL;

    node->mallocExpr.size = size;
    return node;
}

ASTNode *createCallocExprNode(ASTNode *num, ASTNode *size){
    ASTNode *node = allocNode(CALLOC_NODE);
    if(!node) return NULL;

    node->callocExpr.num = num;
    node->callocExpr.size = size;
    return node;
}

ASTNode *createReallocExprNode(ASTNode *ptr, ASTNode *size){
    ASTNode *node = allocNode(REALLOC_NODE);
    if(!node) return NULL;

    node->reallocExpr.ptr = ptr;
    node->reallocExpr.size = size;
    return node;
}

ASTNode *createFreeExprNode(ASTNode *ptr){
    ASTNode *node = allocNode(FREE_NODE);
    if(!node) return NULL;

    node->freeExpr.ptr = ptr;
    return node;
}

ASTNode *createMemcpyExprNode(ASTNode *dest, ASTNode *src, ASTNode *size){
    ASTNode *node = allocNode(MEMCPY_NODE);
    if(!node) return NULL;

    node->memcpyExpr.dest = dest;
    node->memcpyExpr.src = src;
    node->memcpyExpr.size = size;
    return node;
}

ASTNode *createMemsetExprNode(ASTNode *dest, ASTNode *value, ASTNode *size){
    ASTNode *node = allocNode(MEMSET_NODE);
    if(!node) return NULL;

    node->memsetExpr.dest = dest;
    node->memsetExpr.value = value;
    node->memsetExpr.size = size;
    return node;
}

ASTNode *createMemmoveExprNode(ASTNode *dest, ASTNode *src, ASTNode *size){
    ASTNode *node = allocNode(MEMMOVE_NODE);
    if(!node) return NULL;

    node->memmoveExpr.dest = dest;
    node->memmoveExpr.src = src;
    node->memmoveExpr.size = size;
    return node;
}

ASTNode *createUnaryOpNode(ASTNode *expr, UnaryOpType op){
    ASTNode *node = allocNode(UNARY_OPERATION_NODE);
    if(!node) return NULL;

    node->unaryOp.expr = expr;
    node->unaryOp.op = op;
    return node;
}

ASTNode *createBinaryOpNode(ASTNode *left, ASTNode *right, BinaryOpType op){
    ASTNode *node = allocNode(BINARY_OPERATION_NODE);
    if(!node) return NULL;

    node->binaryOp.left = left;
    node->binaryOp.right = right;
    node->binaryOp.op = op;
    return node;
}

ASTNode *createTernaryOpNode(ASTNode *condition, ASTNode *trueExpr, ASTNode *falseExpr){
    ASTNode *node = allocNode(TERNARY_OPERATION_NODE);
    if(!node) return NULL;

    node->ternaryOp.condition = condition;
    node->ternaryOp.trueExpr = trueExpr;
    node->ternaryOp.falseExpr = falseExpr;
    return node;
}

ASTNode *createBlockNode(ASTNode **statements, int stmtCount){
    ASTNode *node = allocNode(BLOCK_NODE);
    if(!node) return NULL;

    node->block.statements = malloc(stmtCount * sizeof(ASTNode *));
    if(!node->block.statements){
        free(node);
        return NULL;
    }
    for(int i = 0; i < stmtCount; i++){
        node->block.statements[i] = statements[i];
    }

    node->block.stmtCount = stmtCount;
    return node;
}

ASTNode *createCompoundExprNode(ASTNode **statements, int stmtCount){
    ASTNode *node = allocNode(COMPOUND_EXPR_NODE);
    if(!node) return NULL;

    node->compoundExpr.statements = malloc(stmtCount * sizeof(ASTNode *));
    if(!node->compoundExpr.statements){
        free(node);
        return NULL;
    }
    for(int i = 0; i < stmtCount; i++){
        node->compoundExpr.statements[i] = statements[i];
    }

    node->compoundExpr.stmtCount = stmtCount;
    return node;
}

ASTNode *createCastExprNode(ASTNode *targetType, ASTNode *value){
    ASTNode *node = allocNode(CAST_EXPR_NODE);
    if(!node) return NULL;

    node->castExpr.targetType = targetType;
    node->castExpr.value = value;
    return node;
}

ASTNode *createIfStmtNode(ASTNode *condition, ASTNode *thenBranch, ASTNode *elseBranch){
    ASTNode *node = allocNode(IF_NODE);
    if(!node) return NULL;

    node->ifStmt.condition = condition;
    node->ifStmt.thenBranch = thenBranch;
    node->ifStmt.elseBranch = elseBranch;
    return node;
}

ASTNode *createSwitchStmtNode(ASTNode *expr, ASTNode **cases, int caseCount){
    ASTNode *node = allocNode(SWITCH_NODE);
    if(!node) return NULL;

    node->switchStmt.expr = expr;
    node->switchStmt.cases = malloc(caseCount * sizeof(ASTNode *));
    if(!node->switchStmt.cases){
        free(node);
        return NULL;
    }
    for(int i = 0; i < caseCount; i++){
        node->switchStmt.cases[i] = cases[i];
    }
    
    node->switchStmt.caseCount = caseCount;
    return node;
}

ASTNode *createCaseStmtNode(ASTNode *value, ASTNode **body, int bodyCount){
    ASTNode *node = allocNode(CASE_NODE);
    if(!node) return NULL;

    node->caseStmt.value = value;
    node->caseStmt.body = malloc(bodyCount * sizeof(ASTNode *));
    if(!node->caseStmt.body){
        free(node);
        return NULL;
    }
    for(int i = 0; i < bodyCount; i++){
        node->caseStmt.body[i] = body[i];
    }

    node->caseStmt.bodyCount = bodyCount;
    return node;
}

ASTNode *createDefaultStmtNode(ASTNode **body, int bodyCount){
    ASTNode *node = allocNode(DEFAULT_NODE);
    if(!node) return NULL;

    node->defaultStmt.body = malloc(bodyCount * sizeof(ASTNode *));
    if(!node->defaultStmt.body){
        free(node);
        return NULL;
    }
    for(int i = 0; i < bodyCount; i++){
        node->defaultStmt.body[i] = body[i];
    }
    
    node->defaultStmt.bodyCount = bodyCount;
    return node;
}

ASTNode *createWhileStmtNode(ASTNode *condition, ASTNode **body, int bodyCount){
    ASTNode *node = allocNode(WHILE_NODE);
    if(!node) return NULL;

    node->whileStmt.condition = condition;
    node->whileStmt.body = malloc(bodyCount * sizeof(ASTNode *));
    if(!node->whileStmt.body){
        free(node);
        return NULL;
    }
    for(int i = 0; i < bodyCount; i++){
        node->whileStmt.body[i] = body[i];
    }

    node->whileStmt.bodyCount = bodyCount;
    return node;
}

ASTNode *createDoWhileStmtNode(ASTNode **body, int bodyCount, ASTNode *condition){
    ASTNode *node = allocNode(DO_WHILE_NODE);
    if(!node) return NULL;

    node->doWhileStmt.body = malloc(bodyCount * sizeof(ASTNode *));
    if(!node->doWhileStmt.body){
        free(node);
        return NULL;
    }
    for(int i = 0; i < bodyCount; i++){
        node->doWhileStmt.body[i] = body[i];
    }

    node->doWhileStmt.bodyCount = bodyCount;
    node->doWhileStmt.condition = condition;
    return node;
}

ASTNode *createForStmtNode(ASTNode *initializer, ASTNode *condition, ASTNode *increment, ASTNode **body, int bodyCount){
    ASTNode *node = allocNode(FOR_NODE);
    if(!node) return NULL;

    node->forStmt.initializer = initializer;
    node->forStmt.condition = condition;
    node->forStmt.increment = increment;
    node->forStmt.body = malloc(bodyCount * sizeof(ASTNode *));
    if(!node->forStmt.body){
        free(node);
        return NULL;
    }
    for(int i = 0; i < bodyCount; i++){
        node->forStmt.body[i] = body[i];
    }

    node->forStmt.bodyCount = bodyCount;
    return node;
}

ASTNode *createContinueStmtNode(void){
    ASTNode *node = allocNode(CONTINUE_NODE);
    if(!node) return NULL;

    return node;
}

ASTNode *createBreakStmtNode(void){
    ASTNode *node = allocNode(BREAK_NODE);
    if(!node) return NULL;

    return node;
}

ASTNode *createTryStmtNode(ASTNode **tryBlock, int tryBlockCount, ASTNode **catchBlock, int catchCount){
    ASTNode *node = allocNode(TRY_NODE);
    if(!node) return NULL;

    node->tryStmt.tryBlock = malloc(tryBlockCount * sizeof(ASTNode *));
    if(!node->tryStmt.tryBlock){
        free(node);
        return NULL;
    }
    for(int i = 0; i < tryBlockCount; i++){
        node->tryStmt.tryBlock[i] = tryBlock[i];
    }

    node->tryStmt.tryBlockCount = tryBlockCount;
    node->tryStmt.catchBlock = malloc(catchCount * sizeof(ASTNode *));
    if(!node->tryStmt.catchBlock){
        free(node->tryStmt.tryBlock);
        free(node);
        return NULL;
    }
    for(int i = 0; i < catchCount; i++){
        node->tryStmt.catchBlock[i] = catchBlock[i];
    }

    node->tryStmt.catchCount = catchCount;
    return node;
}

ASTNode *createCatchStmtNode(ASTNode *exceptionVar, ASTNode **body, int bodyCount){
    ASTNode *node = allocNode(CATCH_NODE);
    if(!node) return NULL;

    node->catchStmt.exceptionVar = exceptionVar;
    node->catchStmt.body = malloc(bodyCount * sizeof(ASTNode *));
    if(!node->catchStmt.body){
        free(node);
        return NULL;
    }
    for(int i = 0; i < bodyCount; i++){
        node->catchStmt.body[i] = body[i];
    }

    node->catchStmt.bodyCount = bodyCount;
    return node;
}

ASTNode *createThrowStmtNode(ASTNode *exceptionExpr){
    ASTNode *node = allocNode(THROW_NODE);
    if(!node) return NULL;

    node->throwStmt.exceptionExpr = exceptionExpr;
    return node;
}

ASTNode *createTypeOfExprNode(ASTNode *expr){
    ASTNode *node = allocNode(TYPEOF_NODE);
    if(!node) return NULL;

    node->typeOfExpr.expr = expr;
    return node;
}

ASTNode *createSizeOfExprNode(ASTNode *expr){
    ASTNode *node = allocNode(SIZEOF_NODE);
    if(!node) return NULL;

    node->sizeOfExpr.expr = expr;
    return node;
}

void freeAST(ASTNode *node){
    if(!node) return NULL;

    switch(node->type){
        case IDENTIFIER_NODE:
            free(node->identifier.name);
            break;
        case LITERAL_NODE:
            if(node->literal.type == TYPE_STRING){
                free(node->literal.value.stringVal);
            }
            break;
        case ASSIGNMENT_NODE:
            freeAST(node->assignment.left);
            freeAST(node->assignment.right);
            break;
        case DECLARATION_NODE:
            free(node->declaration.varName);
            freeAST(node->declaration.varType);
            freeAST(node->declaration.initializer);
            break;
        case POINTER_NODE:
            freeAST(node->pointer.ptr);
            break;
        case VOID_NODE:
            break;
        case NULL_NODE:
            freeAST(node->null.typeOf);
            break;
        case ARRAY_NODE:
            freeAST(node->array.size);
            freeAST(node->array.typeOfElement);
            for(int i = 0; i < node->array.elementsCount; i++){
                if(node->array.elements[i]){
                    freeAST(node->array.elements[i]);
                }
            }
            free(node->array.elements);
            break;
        case STRUCT_NODE:
            free(node->structDef.name);
            for(int i = 0; i < node->structDef.fieldsCount; i++){
                freeAST(node->structDef.fields[i]);
            }
            free(node->structDef.fields);
            break;
        case UNION_NODE:
            free(node->unionDef.name);
            for(int i = 0; i < node->unionDef.fieldsCount; i++){
                freeAST(node->unionDef.fields[i]);
            }
            free(node->unionDef.fields);
            break;
        case ENUM_NODE:
            free(node->enumDef.name);
            for(int i = 0; i < node->enumDef.valuesCount; i++){
                free(node->enumDef.values[i]);
            }
            free(node->enumDef.intValues);
            free(node->enumDef.values);
            break;
        case TYPEDEF_NODE:
            free(node->typedefDef.alias);
            freeAST(node->typedefDef.original);
            break;
        case IMPL_NODE:
            free(node->implDef.structName);
            for(int i = 0; i < node->implDef.methodsCount; i++){
                freeAST(node->implDef.methods[i]);
            }
            free(node->implDef.methods);
            break;
        case ARRAY_ACCESS_NODE:
            freeAST(node->arrayAccess.array);
            freeAST(node->arrayAccess.index);
            break;
        case FIELD_ACCESS_NODE:
            freeAST(node->fieldAccess.object);
            free(node->fieldAccess.fieldName);
            break;
        case FUNCTION_NODE:
            free(node->functionDef.name);
            freeAST(node->functionDef.returnType);
            for(int i = 0; i < node->functionDef.paramCount; i++){
                freeAST(node->functionDef.params[i]);
            }
            free(node->functionDef.params);
            for(int i = 0; i < node->functionDef.bodyCount; i++){
                freeAST(node->functionDef.body[i]);
            }
            free(node->functionDef.body);
            break;
        case RETURN_NODE:
            freeAST(node->returnStmt.value);
            break;
        case FUNCTION_CALL_NODE:
            freeAST(node->functionCall.function);
            for(int i = 0; i < node->functionCall.argsCount; i++){
                freeAST(node->functionCall.args[i]);
            }
            free(node->functionCall.args);
            break;
        case LABEL_NODE:
            free(node->labelStmt.labelName);
            break;
        case JUMP_NODE:
            free(node->jumpStmt.labelName);
            break;
        case MALLOC_NODE:
            freeAST(node->mallocExpr.size);
            break;
        case CALLOC_NODE:
            freeAST(node->callocExpr.num);
            freeAST(node->callocExpr.size);
            break;
        case REALLOC_NODE:
            freeAST(node->reallocExpr.ptr);
            freeAST(node->reallocExpr.size);
            break;
        case FREE_NODE:
            freeAST(node->freeExpr.ptr);
            break;
        case MEMCPY_NODE:
            freeAST(node->memcpyExpr.dest);
            freeAST(node->memcpyExpr.src);
            freeAST(node->memcpyExpr.size);
            break;
        case MEMSET_NODE:
            freeAST(node->memsetExpr.dest);
            freeAST(node->memsetExpr.value);
            freeAST(node->memsetExpr.size);
            break;
        case MEMMOVE_NODE:
            freeAST(node->memmoveExpr.dest);
            freeAST(node->memmoveExpr.src);
            freeAST(node->memmoveExpr.size);
            break;
        case UNARY_OPERATION_NODE:
            freeAST(node->unaryOp.expr);
            break;
        case BINARY_OPERATION_NODE:
            freeAST(node->binaryOp.left);
            freeAST(node->binaryOp.right);
            break;
        case TERNARY_OPERATION_NODE:
            freeAST(node->ternaryOp.condition);
            freeAST(node->ternaryOp.trueExpr);
            freeAST(node->ternaryOp.falseExpr);
            break;
        default:
            break;
    }
    free(node);
}