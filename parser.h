#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct {
    Lexer *lexer;
    Token current;
} Parser;

void initParser(Parser *parser, Lexer *lexer);
void advance(Parser *parser);
ASTNode *parseExpression(Parser *parser);
ASTNode *parseUnaryExpression(Parser *parser);
ASTNode *parsePrimaryExpression(Parser *parser);
ASTNode *parseBinaryExpression(Parser *parser, int minPrecedence);
ASTNode *parsePostfixExpression(Parser *parser);
#endif