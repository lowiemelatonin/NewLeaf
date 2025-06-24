#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_BYTE,             // byte
    TOKEN_BOOL,             // bool
    TOKEN_SHORT,            // short
    TOKEN_USHORT,           // ushort
    TOKEN_INT,              // int
    TOKEN_UINT,             // uint
    TOKEN_LONG,             // long
    TOKEN_ULONG,            // ulong
    TOKEN_LONG_LONG,        // long long
    TOKEN_ULONG_LONG,       // ulong long
    TOKEN_FLOAT,            // float
    TOKEN_DOUBLE,           // double
    TOKEN_LONG_DOUBLE,      // long double
    TOKEN_SIGNED_CHAR,      // schar
    TOKEN_CHAR,             // char
    TOKEN_UNSIGNED_CHAR,    // uchar
    TOKEN_STRING,           // string

    TOKEN_ARCH,             // arch
    TOKEN_UNSIGNED_ARCH,    // uarch

    TOKEN_CONST,            // const
    TOKEN_STATIC,           // static
    TOKEN_EXTERN,           // extern
    TOKEN_VOLATILE,         // volatile
    TOKEN_ATOMIC,           // atomic

    TOKEN_IDENTIFIER,       // variable names, functions, types, params...
    TOKEN_NUMBER,           // for numeric types
    TOKEN_STRING_LITERAL,   // strings
    
    TOKEN_LPAREN,           // (
    TOKEN_RPAREN,           // )
    TOKEN_LBRACE,           // {
    TOKEN_RBRACE,           // }
    TOKEN_LBRACKET,         // [
    TOKEN_RBRACKET,         // ]
    TOKEN_COMMA,            // ,
    TOKEN_SEMICOLON,        // ;
    TOKEN_COLON,            // :
    TOKEN_DOT,              // .
    TOKEN_ARROW,            // ->

    TOKEN_PLUS,             // +
    TOKEN_MINUS,            // -
    TOKEN_STAR,             // *
    TOKEN_SLASH,            // /
    TOKEN_PERCENT,          // %

    TOKEN_AND,              // &&
    TOKEN_OR,               // ||
    TOKEN_NOT               // !
} TokenType;

#endif