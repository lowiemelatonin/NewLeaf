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

ASTNode *createLiteralNode(LiteralType type, LiteralValue value){
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

ASTNode *createAssignmentNode(ASTNode *left, ASTNode *right){
    ASTNode *node = allocNode(ASSIGNMENT_NODE);
    if(!node) return NULL;

    node->assignment.left = left;
    node->assignment.right = right;
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