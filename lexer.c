#include "lexer.h"
#include <string.h>

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

bool isAtEnd(Lexer *lexer){
    return lexer->src[lexer->pos] == '\0';
}

void initLexer(Lexer *lexer, char *src){
    lexer->src = strdup(src);
    lexer->pos = 0;
    lexer->column = 1;
    lexer->line = 1;
}