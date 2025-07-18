#include "parser.h"
#include <stdlib.h>
#include <stdbool.h>

int getPrecedence(TokenType type){
    switch(type){
        case TOKEN_OR:
            return 1;
        case TOKEN_AND:
            return 2;
        case TOKEN_EQUAL:
        case TOKEN_NOT_EQUAL: 
            return 3;
        case TOKEN_LESS_THAN:
        case TOKEN_LESS_EQUAL_THAN:
        case TOKEN_GREATER_THAN:
        case TOKEN_GREATER_EQUAL_THAN:
            return 4;
        case TOKEN_PLUS:
        case TOKEN_MINUS:
            return 5;
        case TOKEN_STAR:
        case TOKEN_SLASH:
        case TOKEN_PERCENT:
            return 6;

        default:
            return -1;
    }
}

void initParser(Parser *parser, Lexer *lexer){
    parser->lexer = lexer;
    parser->current = nextToken(lexer);
}

void advance(Parser *parser){
    parser->current = nextToken(parser->lexer);
}

ASTNode *parsePrimaryExpression(Parser *parser){
    Token token = parser->current;
    ASTNode *expr = NULL;
    switch(token.type){
        case TOKEN_IDENTIFIER: {
            advance(parser);
            expr = createIdentifierNode(token.data.identifier); 
            break;
        }
        case TOKEN_NUMBER:
        case TOKEN_STRING_LITERAL: {
            advance(parser);
            expr = createLiteralNode(token.data.literal.type, token.data.literal.value);
            break;
        }
        case TOKEN_LPAREN: {
            advance(parser);
            expr = parseExpression(parser);
            if(parser->current.type != TOKEN_RPAREN){
                return NULL;
            }
            advance(parser);
            break;
        }
    default:
        return NULL;
    }

    if(parser->current.type == TOKEN_INCREMENT || parser->current.type == TOKEN_DECREMENT){
        TokenType opToken = parser->current.type;
        advance(parser);

        UnaryOpType op = tokenToUnaryOp(opToken, false);
        expr = createUnaryOpNode(expr, op);
    }
    return expr;
}

UnaryOpType tokenToUnaryOp(TokenType type, bool isPrefix){
    switch(type){
        case TOKEN_PLUS: return POSITIVE_UNOP;
        case TOKEN_MINUS: return NEGATIVE_UNOP;
        case TOKEN_NOT: return NOT_UNOP;
        case TOKEN_BITWISE_NOT: return BIT_NOT_UNOP;
        case TOKEN_INCREMENT: return isPrefix ? PRE_INCREMENT_UNOP : POST_INCREMENT_UNOP;
        case TOKEN_DECREMENT: return isPrefix ? PRE_DECREMENT_UNOP : POST_DECREMENT_UNOP;
        case TOKEN_STAR: return DEFERENCE_UNOP;
        case TOKEN_BITWISE_AND: return ADDRESS_OF_UNOP;
        case TOKEN_SIZEOF: return SIZE_OF_UNOP;    
    default:
        return POSITIVE_UNOP;
    }
}

BinaryOpType tokenToBinaryOp(TokenType type){
    switch(type){
        case TOKEN_PLUS: return ADD_BINOP;
        case TOKEN_MINUS: return SUB_BINOP;
        case TOKEN_STAR: return MUL_BINOP;
        case TOKEN_SLASH: return DIV_BINOP;
        case TOKEN_PERCENT: return MOD_BINOP;
        case TOKEN_AND: return AND_BINOP;
        case TOKEN_OR: return OR_BINOP;
        case TOKEN_EQUAL: return EQU_BINOP;
        case TOKEN_NOT_EQUAL: return NOT_EQU_BINOP;
        case TOKEN_LESS_THAN: return LESS_BINOP;
        case TOKEN_LESS_EQUAL_THAN: return LESS_EQU_BINOP;
        case TOKEN_GREATER_THAN: return GREATER_BINOP;
        case TOKEN_GREATER_EQUAL_THAN: return GREATER_EQU_BINOP;
    default:
        return COMMA_BINOP; 
    }
}

ASTNode *parseUnaryExpression(Parser *parser){
    Token token = parser->current;
    switch(token.type){
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_NOT:
        case TOKEN_BITWISE_NOT:
        case TOKEN_INCREMENT:
        case TOKEN_DECREMENT:
        case TOKEN_STAR:
        case TOKEN_BITWISE_AND:
        case TOKEN_SIZEOF:
            advance(parser);
            UnaryOpType op = tokenToUnaryOp(token.type, true);
            ASTNode *expr = parseUnaryExpression(parser);
            if(!expr) return NULL;
            return createUnaryOpNode(expr, op);
    default:
        return parsePrimaryExpression(parser);
    }
}

ASTNode *parseBinaryExpression(Parser *parser, int minPrecedence){
    ASTNode *left = parseUnaryExpression(parser);
    if(!left) return NULL;

    while(1){
        int prec = getPrecedence(parser->current.type);
        if(prec < minPrecedence) break;

        TokenType opType = parser->current.type;
        BinaryOpType op = tokenToBinaryOp(opType);

        advance(parser);

        ASTNode *right = parseBinaryExpression(parser, prec + 1);
        if(!right) return NULL;

        left = createBinaryOpNode(left, right, op);
    }
    return left;
}

ASTNode *parseTernaryExpression(Parser *parser){
    ASTNode *condition = parseBinaryExpression(parser, 0);
    if(!condition) return NULL;

    if(parser->current.type == TOKEN_QUESTION){
        advance(parser);

        ASTNode *trueExpr = parseExpression(parser);
        if(!trueExpr) return NULL;

        if(parser->current.type != TOKEN_COLON) return NULL;
        advance(parser);

        ASTNode *falseExpr = parseExpression(parser);
        if(!falseExpr) return NULL;

        return createTernaryOpNode(condition, trueExpr, falseExpr);
    }
    return condition;
}

ASTNode *parsePostfixExpression(Parser *parser){
    ASTNode *expr = parseUnaryExpression(parser);
    if(!expr) return NULL;

    while(1){
        Token token = parser->current;
        switch(token.type){
            case TOKEN_LPAREN: {
                advance(parser);

                ASTNode **args = NULL;
                int argsCount = 0;

                if(parser->current.type != TOKEN_RPAREN){
                    while(1){
                        ASTNode *arg = parseExpression(parser);
                        if(!arg) return NULL;

                        args = realloc(args, sizeof(ASTNode *) * (argsCount + 1));
                        args[argsCount++] = arg;

                        if(parser->current.type == TOKEN_COMMA){
                            advance(parser);
                        } else{
                            break;
                        }
                    }
                }
                if(parser->current.type != TOKEN_RPAREN) return NULL;
                advance(parser);
                expr = createFunctionCallNode(expr, args, argsCount);
                break;
            }

            case TOKEN_LBRACKET: {
                advance(parser);
                ASTNode *index = parseExpression(parser);
                if(!index) return NULL;
                if(parser->current.type != TOKEN_RBRACKET) return NULL;
                advance(parser);

                expr = createArrayAccessNode(expr, index);
                break;
            }

            case TOKEN_DOT: {
                advance(parser);
                if(parser->current.type != TOKEN_IDENTIFIER) return NULL;
                char *field = parser->current.data.identifier;
                advance(parser);

                expr = createFieldAccessNode(expr, field, false);
                break;
            }

            case TOKEN_ARROW: {
                advance(parser);
                if(parser->current.type != TOKEN_IDENTIFIER) return NULL;
                char *field = parser->current.data.identifier;
                advance(parser);

                expr = createFieldAccessNode(expr, field, true);
                break;
            }
            default: 
                return expr;
        }
    }
}

ASTNode *parseExpressionStatement(Parser *parser){
    ASTNode *expr = parseExpression(parser);
    if(!expr) return NULL;

    if(parser->current.type != TOKEN_SEMICOLON) return NULL;

    advance(parser);
    return expr;
}

ASTNode *parseBlockStatement(Parser *parser){
    if(parser->current.type != TOKEN_LBRACE) return NULL;
    advance(parser);

    ASTNode **statements = NULL;
    int count = 0;

    while(parser->current.type != TOKEN_RBRACE && parser->current.type != TOKEN_EOF){
        ASTNode *stmt = parseStatement(parser);
        if(!stmt) return NULL;

        statements = realloc(statements, sizeof(ASTNode *) * (count + 1));
        statements[count++] = stmt;
    }

    if(parser->current.type != TOKEN_RBRACE) return NULL;
    advance(parser);
    return createBlockNode(statements, count);
}

ASTNode *parseReturnStatement(Parser *parser){
    if(parser->current.type != TOKEN_RETURN) return NULL;
    advance(parser);

    ASTNode *expr = NULL;
    if(parser->current.type != TOKEN_SEMICOLON){
        expr = parseExpression(parser);
        if(!expr) return NULL;
    }

    if(parser->current.type != TOKEN_SEMICOLON) return NULL;
    advance(parser);

    return createReturnNode(expr);
}

ASTNode *parseIfStatement(Parser *parser){
    if(parser->current.type != TOKEN_IF) return NULL;
    advance(parser);

    if(parser->current.type != TOKEN_LPAREN) return NULL;
    advance(parser);

    ASTNode *condition = parseExpression(parser);
    if(!condition) return NULL;

    if(parser->current.type != TOKEN_RPAREN) return NULL;
    advance(parser);

    ASTNode *thenBranch = parseStatement(parser);
    if(!thenBranch) return NULL;

    ASTNode *elseBranch = NULL;
    if(parser->current.type == TOKEN_ELSE){
        advance(parser);
        elseBranch = parseStatement(parser);
        if(!elseBranch) return NULL;
    }

    return createIfStmtNode(condition, thenBranch, elseBranch);
}

ASTNode *parseWhileStatement(Parser *parser){
    if(parser->current.type != TOKEN_WHILE) return NULL;
    advance(parser);

    if(parser->current.type != TOKEN_LPAREN) return NULL;
    advance(parser);

    ASTNode *condition = parseExpression(parser);
    if(!condition) return NULL;

    if(parser->current.type != TOKEN_RPAREN) return NULL;
    advance(parser);

    ASTNode *body = parseStatement(parser);
    if(!body) return NULL;

    ASTNode **bodyArr = malloc(sizeof(ASTNode*));
    if(!bodyArr) return NULL;
    bodyArr[0] = body;

    return createWhileStmtNode(condition, bodyArr, 1);
}

ASTNode *parseStatement(Parser *parser){
    switch(parser->current.type){
        case TOKEN_LBRACE:
            return parseBlockStatement(parser);
        case TOKEN_RETURN:
            return parseReturnStatement(parser);
        case TOKEN_IF:
            return parseIfStatement(parser);
        case TOKEN_WHILE:
            return parseWhileStatement(parser);
        default:
            return parseExpressionStatement(parser);
    }
}

ASTNode *parseExpression(Parser *parser){
    return parseTernaryExpression(parser);
}