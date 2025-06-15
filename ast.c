#include <ast.h>
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