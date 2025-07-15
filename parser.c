#include "parser.h"

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

ASTNode *parseExpression(Parser *parser){
    return parseBinaryExpression(parser, 0);
}