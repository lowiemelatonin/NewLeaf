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

Token lexIdentifier(Lexer *lexer){
    size_t start = lexer->pos;
    while(isalnum(peek(lexer)) || peek(lexer) == '_'){
        advance(lexer);
    }

    size_t len = lexer->pos - start;
    char *text = malloc(len + 1);
    if(!text) return createToken(lexer, TOKEN_NULL, (TokenData){0}, "");
    memcpy(text, &lexer->src[start], len);
    text[len] = '\0';

    TokenType type = TOKEN_IDENTIFIER;
    #define KeyWord(str, tok) if(strcmp(text, str) == 0) type = tok;

    KeyWord("null", TOKEN_NULL);
    KeyWord("void", TOKEN_VOID);
    KeyWord("byte", TOKEN_BYTE);
    KeyWord("bool", TOKEN_BOOL);
    KeyWord("short", TOKEN_SHORT);
    KeyWord("ushort", TOKEN_USHORT);
    KeyWord("int", TOKEN_INT);
    KeyWord("uint", TOKEN_UINT);
    KeyWord("long", TOKEN_LONG);
    KeyWord("ulong", TOKEN_ULONG);
    KeyWord("longlong", TOKEN_LONG_LONG);
    KeyWord("ulonglong", TOKEN_ULONG_LONG);
    KeyWord("float", TOKEN_FLOAT);
    KeyWord("double", TOKEN_DOUBLE);
    KeyWord("longdouble", TOKEN_LONG_DOUBLE);
    KeyWord("schar", TOKEN_SIGNED_CHAR);
    KeyWord("char", TOKEN_CHAR);
    KeyWord("uchar", TOKEN_UNSIGNED_CHAR);
    KeyWord("string", TOKEN_STRING);

    KeyWord("arch", TOKEN_ARCH);
    KeyWord("uarch", TOKEN_UNSIGNED_ARCH);

    KeyWord("struct", TOKEN_STRUCT);
    KeyWord("impl", TOKEN_IMPL);
    KeyWord("union", TOKEN_UNION);
    KeyWord("enum", TOKEN_ENUM);
    KeyWord("typedef", TOKEN_TYPEDEF);

    KeyWord("const", TOKEN_CONST);
    KeyWord("static", TOKEN_STATIC);
    KeyWord("extern", TOKEN_EXTERN);
    KeyWord("volatile", TOKEN_VOLATILE);
    KeyWord("atomic", TOKEN_ATOMIC);

    KeyWord("if", TOKEN_IF);
    KeyWord("else", TOKEN_ELSE);
    KeyWord("while", TOKEN_WHILE);
    KeyWord("do", TOKEN_DO);
    KeyWord("for", TOKEN_FOR);
    KeyWord("switch", TOKEN_SWITCH);
    KeyWord("case", TOKEN_CASE);
    KeyWord("default", TOKEN_DEFAULT);
    KeyWord("continue", TOKEN_CONTINUE);
    KeyWord("break", TOKEN_BREAK);

    KeyWord("label", TOKEN_LABEL);
    KeyWord("jump", TOKEN_JUMP);

    KeyWord("try", TOKEN_TRY);
    KeyWord("catch", TOKEN_CATCH);
    KeyWord("throw", TOKEN_THROW);

    KeyWord("fun", TOKEN_FUNCTION);
    KeyWord("lambda", TOKEN_LAMBDA);
    KeyWord("return", TOKEN_RETURN);

    KeyWord("typeof", TOKEN_TYPEOF);
    KeyWord("sizeof", TOKEN_SIZEOF);

    KeyWord("malloc", TOKEN_MALLOC);
    KeyWord("calloc", TOKEN_CALLOC);
    KeyWord("realloc", TOKEN_REALLOC);
    KeyWord("free", TOKEN_FREE);

    KeyWord("memcpy", TOKEN_MEMCPY);
    KeyWord("memset", TOKEN_MEMSET);
    KeyWord("memmove", TOKEN_MEMMOVE);
    #undef KeyWord;

    TokenData data = {0};
    if(type == TOKEN_IDENTIFIER){
        data.identifier = strdup(text);
    }

    Token token = createToken(lexer, type, data, text);
    free(text);
    return token;
}

Token nextToken(Lexer *lexer){
    skipWhiteSpace(lexer);

    if(isAtEnd(lexer)){
        return createToken(lexer, TOKEN_EOF, (TokenData){0}, "");
    }

    char current = advance(lexer);

    if(isalpha(current) || current == '_'){
        lexer->pos--;
        lexer->column--;
        return lexIdentifier(lexer);
    }

    if(isdigit(current)){
        lexer->pos--;
        lexer->column--;
        return lexNumber(lexer);
    }

    if(current == '"' || current =="'"){
        lexString(lexer);
    }

    switch(current){
        case '+':
            if(match(lexer, '+')) return createToken(lexer, TOKEN_INCREMENT, (TokenData){0}, "++");
            if(match(lexer, '=')) return createToken(lexer, TOKEN_ADD_ASSIGNMENT, (TokenData){0}, "+=");
            return createToken(lexer, TOKEN_PLUS, (TokenData){0}, "+");
        case '-':
            if(match(lexer, '-')) return createToken(lexer, TOKEN_DECREMENT, (TokenData){0}, "--");
            if(match(lexer, '=')) return createToken(lexer, TOKEN_SUB_ASSIGNMENT, (TokenData){0}, "-=");
            if(match(lexer, ">")) return createToken(lexer, TOKEN_ARROW, (TokenData){0}, "->");
            return createToken(lexer, TOKEN_MINUS, (TokenData){0}, "-");
        case '*':
            if(match(lexer, '=')) return createToken(lexer, TOKEN_MUL_ASSIGNMENT, (TokenData){0}, "*=");
            return createToken(lexer, TOKEN_STAR, (TokenData){0}, "*");
        case '/':
            if(match(lexer, '=')) return createToken(lexer, TOKEN_DIV_ASSIGNMENT, (TokenData){0}, "/=");
            return createToken(lexer, TOKEN_SLASH, (TokenData){0}, "/");
        case '%':
            if(match(lexer, '=')) return createToken(lexer, TOKEN_MOD_ASSIGNMENT, (TokenData){0}, "%=");
            return createToken(lexer, TOKEN_PERCENT, (TokenData){0}, "%");
        case '=':
            if(match(lexer, '=')) return createToken(lexer, TOKEN_EQUAL, (TokenData){0}, "==");
            return createToken(lexer, TOKEN_ASSIGNMENT, (TokenData){0}, "=");
        case '!':
            if(match(lexer, '=')) return createToken(lexer, TOKEN_NOT_EQUAL, (TokenData){0}, "!=");
            return createToken(lexer, TOKEN_NOT, (TokenData){0}, "!");
        case '<':
            if(match(lexer, '=')) return createToken(lexer, TOKEN_LESS_EQUAL_THAN, (TokenData){0}, "<=");
            if(match(lexer, '<')){
                if(match(lexer, '=')) return createToken(lexer, TOKEN_SHIFT_LEFT_ASSIGN, (TokenData){0}, "<<=");
                return createToken(lexer, TOKEN_SHIFT_LEFT, (TokenData){0}, "<<");
            }
            return createToken(lexer, TOKEN_LESS_THAN, (TokenData){0}, "<");
        case '>':
            if(match(lexer, '=')) return createToken(lexer, TOKEN_GREATER_EQUAL_THAN, (TokenData){0}, ">=");
            if(match(lexer, '>')){
                if(match(lexer, '=')) return createToken(lexer, TOKEN_SHIFT_RIGHT_ASSIGN, (TokenData){0}, ">>=");
                return createToken(lexer, TOKEN_SHIFT_RIGHT, (TokenData){0}, ">>");
            }
            return createToken(lexer, TOKEN_GREATER_THAN, (TokenData){0}, ">");
        case '&':
            if(match(lexer, '&')) return createToken(lexer, TOKEN_AND, (TokenData){0}, "&&");
            if(match(lexer, '=')) return createToken(lexer, TOKEN_BITWISE_AND_ASSIGN, (TokenData){0}, "&=");
            return createToken(lexer, TOKEN_BITWISE_AND, (TokenData){0}, "&");
        case '|':
            if(match(lexer, '|')) return createToken(lexer, TOKEN_OR, (TokenData){0}, "||");
            return createToken(lexer, TOKEN_BITWISE_OR, (TokenData){0}, "|");
        case '^':
            return createToken(lexer, TOKEN_BITWISE_XOR, (TokenData){0}, "^");
        case '~':
            return createToken(lexer, TOKEN_BITWISE_NOT, (TokenData){0}, "~");
        case '?':
            return createToken(lexer, TOKEN_QUESTION, (TokenData){0}, "?");
        case ':':
            return createToken(lexer, TOKEN_COLON, (TokenData){0}, ":");
        // add more in the future...
    default:
        break;
    }
}