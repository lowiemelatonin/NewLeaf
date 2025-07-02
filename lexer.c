#include "lexer.h"
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

Token createToken(Lexer *lexer, TokenType type, TokenData data, char *lexeme){
    Token token;
    token.type = type;
    token.data = data;
    token.line = lexer->line;
    token.column = lexer->column;
    token.lexeme = strdup(lexeme);
    if(!token.lexeme){
        token.type = TOKEN_NULL;
        token.data.identifier = NULL;
        return token;
    };
    return token;
}

char peek(Lexer *lexer){
    return lexer->src[lexer->pos];
}

char peekNext(Lexer *lexer){
    if(lexer->src[lexer->pos + 1] == '\0') return '\0';
    return lexer->src[lexer->pos + 1];
}

char advance(Lexer *lexer){
    char current = lexer->src[lexer->pos];
    if(current == '\0') return '\0';

    lexer->pos++;
    if(current == '\n'){
        lexer->line++;
        lexer->column = 1;
    } else{
        lexer->column++;
    }
    return current;
}

void skipWhiteSpace(Lexer *lexer){
    while(isspace(lexer->src[lexer->pos])){
        advance(lexer);
    }
}

bool isAtEnd(Lexer *lexer){
    return lexer->src[lexer->pos] == '\0';
}

bool match(Lexer *lexer, char expected){
    if(isAtEnd(lexer)) return false;
    if(peek(lexer) != expected) return false;
    
    advance(lexer);
    return true;
}

void initLexer(Lexer *lexer, char *src){
    lexer->src = strdup(src);
    lexer->pos = 0;
    lexer->column = 1;
    lexer->line = 1;
}

Token lexNumber(Lexer *lexer){
    size_t start = lexer->pos;
    bool isFloat = false;

    while(isdigit(peek(lexer))){
        advance(lexer);
    }

    if(peek(lexer) == '.' && isdigit(peekNext(lexer))){
        isFloat = true;
        advance(lexer);

        while(isdigit(peek(lexer))){
            advance(lexer);
        }
    }
    size_t len = lexer->pos - start;
    char *numStr = malloc(len + 1);

    if(!numStr) return createToken(lexer, TOKEN_NULL, (TokenData){0}, "");
    memcpy(numStr, &lexer->src[start], len);
    numStr[len] = '\0';

    TokenData data;
    if(isFloat){
        data.literal.type = TYPE_DOUBLE;
        data.literal.value.doubleVal = atof(numStr);
    } else{
        data.literal.type = TYPE_INT;
        data.literal.value.intVal = atoi(numStr);
    }

    Token token = createToken(lexer, TOKEN_NUMBER, data, numStr);
    free(numStr);
    return token;
}

Token lexString(Lexer *lexer){
    size_t start = lexer->pos;
    while(!isAtEnd(lexer) && peek(lexer) != '"'){
        if(peek(lexer) == '\\' && peekNext(lexer) == '"') advance(lexer);
        advance(lexer);
    }

    if(isAtEnd(lexer)){
        return createToken(lexer, TOKEN_NULL, (TokenData){0}, "");
    }
    
    advance(lexer);
    size_t len = lexer->pos - start - 1;
    char *str = malloc(len + 1);
    if(!str) return createToken(lexer, TOKEN_NULL, (TokenData){0}, "");

    memcpy(str, &lexer->src[start], len);
    str[len] = '\0';

    TokenData data = {0};
    data.literal.type = TYPE_STRING;
    data.literal.value.stringVal = strdup(str);

    Token token = createToken(lexer, TOKEN_STRING_LITERAL, data, str);
    return token;
}