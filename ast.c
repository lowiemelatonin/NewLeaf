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

ASTNode *createArrayNode(ASTNode *typeOfElement, ASTNode *size){
    ASTNode *node = allocNode(ARRAY_NODE);
    if(!node) return NULL;

    node->array.typeOfElement = typeOfElement;
    node->array.size = size;
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