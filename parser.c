#include "parser.h"

void initParser(Parser *parser, Lexer *lexer){
    parser->lexer = lexer;
    parser->current = nextToken(lexer);
}

void advance(Parser *parser){
    parser->current = nextToken(parser->lexer);
}