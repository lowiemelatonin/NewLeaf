#ifndef EVAL_H
#define EVAL_H

#include "ast.h"
#include "hashmap.h"

Value eval_same_type_binary(Value left, Value right, const char *op);
Value eval(ASTNode *node, HashMap *env);

#endif