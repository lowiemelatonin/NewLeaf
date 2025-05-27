#include "eval.h"
#include <stdlib.h>
#include <string.h>

Value eval_same_type_binary(Value left, Value right, const char *op){
    switch(left.type){
        case TYPE_INT:
            if(strcmp(op, "+") == 0) return (Value){.type = TYPE_INT, .data.i = left.data.i + right.data.i};
            if(strcmp(op, "-") == 0) return (Value){.type = TYPE_INT, .data.i = left.data.i - right.data.i};
            if(strcmp(op, "*") == 0) return (Value){.type = TYPE_INT, .data.i = left.data.i * right.data.i};
            if(strcmp(op, "/") == 0){
                if(right.data.i == 0){
                    exit(1);
                }
                return (Value){.type = TYPE_INT, .data.i = left.data.i / right.data.i};
            } 
            break;
        case TYPE_FLOAT:
            if(strcmp(op, "+") == 0) return (Value){.type = TYPE_FLOAT, .data.f = left.data.f + right.data.f};
            if(strcmp(op, "-") == 0) return (Value){.type = TYPE_FLOAT, .data.f = left.data.f - right.data.f};
            if(strcmp(op, "*") == 0) return (Value){.type = TYPE_FLOAT, .data.f = left.data.f * right.data.f};
            if(strcmp(op, "/") == 0){
                if(right.data.f == 0.0f){
                    exit(1);
                }
                return (Value){.type = TYPE_FLOAT, .data.f = left.data.f / right.data.f};
            } 
            break;
        case TYPE_BOOL:
            if(strcmp(op, "&&") == 0) return (Value){.type = TYPE_BOOL, .data.i = left.data.i && right.data.i};
            if(strcmp(op, "||") == 0) return (Value){.type = TYPE_BOOL, .data.i = left.data.i || right.data.i};
            break;
        case TYPE_STRING:
            if(strcmp(op, "+") == 0){
                int len = strlen(left.data.s) + strlen(right.data.s) + 1;
                char *result = malloc(len);
                strcpy(result, left.data.s);
                strcat(result, right.data.s);
                return (Value){.type = TYPE_STRING, .data.s = result};
            }
            break;
        default:
            break;
    }
    exit(1);
}

Value eval(ASTNode *node, HashMap *env){
    switch(node->type){
        case INT_NODE: {
            Value val = {.type = TYPE_INT, .data.i = node->value};
            return val;
        }
        case FLOAT_NODE: {
            Value val = {.type = TYPE_FLOAT, .data.f = node->fvalue};
            return val;
        }
        case BOOL_NODE: {
            Value val = {.type = TYPE_BOOL, .data.i = node->bvalue};
            return val;
        }
        case STR_NODE: {
            Value val = {.type = TYPE_STRING, .data.s = node->str};
            return val;
        }
        case VAR_NODE: {
            Value *val = get(env, node->var_name);
            if(val == NULL){
                exit(1);
            }
            return *val;
        }
        case ASSIGN_NODE: {
            ASTNode *left = node->Assign.left;
            ASTNode *right = node->Assign.right;
            Value rval = eval(right, env);
            if (left->type != VAR_NODE) {
                exit(1);
            }
            insert_value(env, left->var_name, rval);
            return rval;
        }
        case BINARY_OP_NODE: {
            Value left = eval(node->BinaryOp.left, env);
            Value right = eval(node->BinaryOp.right, env);
            const char *op = node->BinaryOp.op;
            if(left.type == right.type){
                return eval_same_type_binary(left, right, op);
            }
            exit(1);
        }
        default:
            break;
    }
}
