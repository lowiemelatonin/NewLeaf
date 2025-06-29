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

void initLexer(Lexer *lexer, char *src){
    lexer->src = strdup(src);
    lexer->pos = 0;
    lexer->column = 1;
    lexer->line = 1;
}