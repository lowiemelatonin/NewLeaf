#include "lexer.h"
#include <string.h>

static char peek(Lexer *lexer){
    return lexer->src[lexer->pos];
}

static char peekNext(Lexer *lexer){
    if(lexer->src[lexer->pos] == "\0") return '\0';
    return lexer->src[lexer->pos+1];
}

static char advance(Lexer *lexer){
    char current = lexer->src[lexer->pos++];
    if(current == '\n'){
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return current;
}

void ignoreWhiteSpace(Lexer *lexer){
    if(isspace(peek(lexer))){
        advance(lexer);
    }
}

Token createToken(Lexer *lexer, TokenType type, char *lexeme, PrimitiveType pType, PrimitiveValue pValue, char *identifier){
    Token token;
    token.type = type;

    token.lexeme = strdup(lexeme);
    token.line = lexer->line;
    token.column = lexer->column;
    
    if(type == TOKEN_IDENTIFIER){
        token.identifier = strdup(identifier);
    } else if(type == TOKEN_NUMBER || type == TOKEN_STRING_LITERAL){
        token.literal.pType = pType;
        if(pType == TYPE_STRING && pValue.stringVal != NULL){
            token.literal.pValue.stringVal = strdup(pValue.stringVal);
        } else{
            token.literal.pValue = pValue;
        }
    }
    return token;
}

void initLexer(Lexer *lexer, char *src){
    lexer->src = src;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;
}