#ifndef PARSER_H
#define PARSE_H

#include "lexer.h"

typedef struct {
    Lexer *lexer;
    Token current;
} Parser;

void initParser(Parser *parser, Lexer *lexer);

#endif