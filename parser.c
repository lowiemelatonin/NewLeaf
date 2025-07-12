#include "parser.h"

void initParser(Parser *parser, Lexer *lexer){
    parser->lexer = lexer;
    parser->current = nextToken(lexer);
}

void advance(Parser *parser){
    parser->current = nextToken(parser->lexer);
}

ASTNode *parsePrimaryExpression(Parser *parser){
    Token token = parser->current;
    switch(token.type){
        case TOKEN_IDENTIFIER: {
            advance(parser);
            return createIdentifierNode(token.data.identifier);
        }
        case TOKEN_NUMBER:
        case TOKEN_STRING_LITERAL: {
            advance(parser);
            return(createLiteralNode(token.data.literal.type, token.data.literal.value));
        }
        case TOKEN_LPAREN: {
            advance(parser);
            ASTNode *expr = parseExpression(parser);
            if(parser->current.type != TOKEN_RPAREN){
                return NULL;
            }
            advance(parser);
            return expr;
        }
    default:
        return NULL;
    }
}