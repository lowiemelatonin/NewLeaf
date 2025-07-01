#include "lexer.h"
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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