#include <ast.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

ASTNode *createIdentifierNode(const char *name){
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;
    
    node->type = IDENTIFIER_NODE;
    node->identifier.name = strdup(name);
    return node;
};

ASTNode *createAssignmentNode(ASTNode *left, ASTNode *right){
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = ASSIGNMENT_NODE;
    node->assignment.left = left;
    node->assignment.right = right;
    return node;
}

ASTNode *createDeclarationNode(ASTNode *varType, const char *varName, ASTNode *initializer){
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;
    
    node->type = DECLARATION_NODE;
    node->declaration.varType = varType;
    node->declaration.varName = strdup(varName);
    node->declaration.initializer = initializer;
    return node;
}

ASTNode *createBoolNode(bool value){
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = BOOL_NODE;
    node->boolLiteral.value = value;
    return node;
}

ASTNode *createShortNode(short value){
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = SHORT_NODE;
    node->shortLiteral.value = value;
    return node;
}

ASTNode *createUnsignedShortNode(unsigned short value){
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = UNSIGNED_SHORT_NODE;
    node->unsignedShortLiteral.value = value;
    return node;
}