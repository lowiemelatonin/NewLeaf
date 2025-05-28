#ifndef EVAL_H
#define EVAL_H

#include "ast.h"
#include "hashmap.h"

Value eval_unary_op(Value expr, const char *op);
Value eval_binary_op(Value left, Value right, const char *op);
Value eval(ASTNode *node, HashMap *env);

#endif