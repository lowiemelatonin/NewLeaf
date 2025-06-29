#include "lexer.h"
#include <string>

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

void initLexer(Lexer *lexer, char *src){
    lexer->src = src;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;
}