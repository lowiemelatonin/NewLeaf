#include "eval.h"
#include <stdlib.h>
#include <string.h>

Value eval_unary_op(Value expr, const char *op){
    if(expr.type == TYPE_INT){
        if(strcmp(op, "!") == 0) return (Value){ .type = TYPE_INT, .data.i = !expr.data.i };
        if(strcmp(op, "+") == 0) return (Value){ .type = TYPE_INT, .data.i = +expr.data.i };
        if(strcmp(op, "-") == 0) return (Value){ .type = TYPE_INT, .data.i = -expr.data.i };
    }
    if(expr.type == TYPE_FLOAT){
        if(strcmp(op, "!") == 0) return (Value){ .type = TYPE_FLOAT, .data.f = !expr.data.f };
        if(strcmp(op, "+") == 0) return (Value){ .type = TYPE_FLOAT, .data.f = +expr.data.f };
        if(strcmp(op, "-") == 0) return (Value){ .type = TYPE_FLOAT, .data.f = -expr.data.f };
    }
    if(expr.type == TYPE_BOOL){
        if(strcmp(op, "!") == 0) return (Value){ .type = TYPE_BOOL, .data.i = !expr.data.i };
    }
    exit(1);
}

Value eval_binary_op(Value left, Value right, const char *op){
    if(left.type == TYPE_FLOAT && right.type == TYPE_INT){
        right.data.f = (float)right.data.i;
        right.type = TYPE_FLOAT;
    } else if(left.type == TYPE_INT && right.type == TYPE_FLOAT){
        left.data.f = (float)left.data.i;
        left.type = TYPE_FLOAT;
    }
    if((left.type == TYPE_STRING && right.type != TYPE_STRING) || (right.type == TYPE_STRING && left.type != TYPE_STRING)){
        exit(1);
    }

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
            if(strcmp(op, "==") == 0) return (Value){.type = TYPE_BOOL, .data.i = left.data.i == right.data.i};
            if(strcmp(op, "!=") == 0) return (Value){.type = TYPE_BOOL, .data.i = left.data.i != right.data.i};
            if(strcmp(op, ">") == 0) return (Value){.type = TYPE_BOOL, .data.i = left.data.i > right.data.i};
            if(strcmp(op, "<") == 0) return (Value){.type = TYPE_BOOL, .data.i = left.data.i < right.data.i};
            if(strcmp(op, ">=") == 0) return (Value){.type = TYPE_BOOL, .data.i = left.data.i >= right.data.i};
            if(strcmp(op, "<=") == 0) return (Value){.type = TYPE_BOOL, .data.i = left.data.i <= right.data.i};
            break;
        case TYPE_STRING:
            if(strcmp(op, "+") == 0){
                int len = strlen(left.data.s) + strlen(right.data.s) + 1;
                char *result = malloc(len);
                strcpy(result, left.data.s);
                strcat(result, right.data.s);
                return (Value){.type = TYPE_STRING, .data.s = result};
            }
            if(strcmp(op, "==") == 0) return (Value){.type = TYPE_BOOL, .data.i = strcmp(left.data.s, right.data.s) == 0};
            if(strcmp(op, "!=") == 0) return (Value){.type = TYPE_BOOL, .data.i = strcmp(left.data.s, right.data.s) != 0};
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
            Value val = {.type = TYPE_STRING, .data.s = strdup(node->str)};
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
            Value rval;
            if (left->type != VAR_NODE) {
                exit(1);
            }

            if(right->type == BINARY_OP_NODE && right->BinaryOp.left->type == VAR_NODE && strcmp(right->BinaryOp.left->var_name, left->var_name) == 0){
                Value lval = eval(left, env);
                Value rval_right = eval(right->BinaryOp.right, env);
                rval = eval_binary_op(lval, rval_right, right->BinaryOp.op);
            } else {
                rval = eval(right, env);
            }
            insert_value(env, left->var_name, rval);
            return rval;
        }
        case UNARY_OP_NODE: {
            Value expr = eval(node->UnaryOp.expr, env);
            const char *op = node->UnaryOp.op;
            return eval_unary_op(expr, op);
        }
        case BINARY_OP_NODE: {
            Value left = eval(node->BinaryOp.left, env);
            Value right = eval(node->BinaryOp.right, env);
            const char *op = node->BinaryOp.op;
            return eval_binary_op(left, right, op);
        }
        case BLOCK_NODE: {
            Value result;
            for(int i = 0; i < node->Block.count; i++){
                result = eval(node->Block.nodes[i], env);
            }
            return result;
        }
        default:
            break;
    }
}
