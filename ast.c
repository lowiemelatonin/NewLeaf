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
}

ASTNode *createLiteralNode(LiteralType type, LiteralValue value){
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = LITERAL_NODE;
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