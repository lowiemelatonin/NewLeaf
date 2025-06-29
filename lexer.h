#ifndef LEXER_H
#define LEXER_H

#include "ast.h"
#include <stddef.h>

typedef enum {
    TOKEN_NULL,                 // null
    TOKEN_VOID,                 // void
    TOKEN_BYTE,                 // byte
    TOKEN_BOOL,                 // bool
    TOKEN_SHORT,                // short
    TOKEN_USHORT,               // ushort
    TOKEN_INT,                  // int
    TOKEN_UINT,                 // uint
    TOKEN_LONG,                 // long
    TOKEN_ULONG,                // ulong
    TOKEN_LONG_LONG,            // long long
    TOKEN_ULONG_LONG,           // ulong long
    TOKEN_FLOAT,                // float
    TOKEN_DOUBLE,               // double
    TOKEN_LONG_DOUBLE,          // long double
    TOKEN_SIGNED_CHAR,          // schar
    TOKEN_CHAR,                 // char
    TOKEN_UNSIGNED_CHAR,        // uchar
    TOKEN_STRING,               // string

    TOKEN_ARCH,                 // arch
    TOKEN_UNSIGNED_ARCH,        // uarch

    TOKEN_STRUCT,               // struct
    TOKEN_IMPL,                 // impl
    TOKEN_UNION,                // union
    TOKEN_ENUM,                 // enum
    TOKEN_TYPEDEF,              // typedef

    TOKEN_CONST,                // const
    TOKEN_STATIC,               // static
    TOKEN_EXTERN,               // extern
    TOKEN_VOLATILE,             // volatile
    TOKEN_ATOMIC,               // atomic

    TOKEN_IDENTIFIER,           // variable names, functions, types, params...
    TOKEN_NUMBER,               // for numeric types
    TOKEN_STRING_LITERAL,       // strings
    
    TOKEN_LPAREN,               // (
    TOKEN_RPAREN,               // )
    TOKEN_LBRACE,               // {
    TOKEN_RBRACE,               // }
    TOKEN_LBRACKET,             // [
    TOKEN_RBRACKET,             // ]
    TOKEN_COMMA,                // ,
    TOKEN_SEMICOLON,            // ;
    TOKEN_COLON,                // :
    TOKEN_DOT,                  // .
    TOKEN_ARROW,                // ->

    TOKEN_PLUS,                 // +
    TOKEN_MINUS,                // -
    TOKEN_STAR,                 // *
    TOKEN_SLASH,                // /
    TOKEN_PERCENT,              // %

    TOKEN_AND,                  // &&
    TOKEN_OR,                   // ||
    TOKEN_NOT,                  // !

    TOKEN_BITWISE_AND,          // &
    TOKEN_BITWISE_OR,           // |
    TOKEN_BITWISE_XOR,          // ^
    TOKEN_BITWISE_NOT,          // ~
    TOKEN_SHIFT_LEFT,           // <<
    TOKEN_SHIFT_RIGHT,          // >>

    TOKEN_INCREMENT,            // ++
    TOKEN_DECREMENT,            // --

    TOKEN_ASSIGNMENT,           // =
    TOKEN_ADD_ASSIGNMENT,       // +=
    TOKEN_SUB_ASSIGNMENT,       // -=
    TOKEN_MUL_ASSIGNMENT,       // *=
    TOKEN_DIV_ASSIGNMENT,       // /=
    TOKEN_MOD_ASSIGNMENT,       // %=
    TOKEN_BITWISE_AND_ASSIGN,   // &=
    TOKEN_BITWISE_OR_ASSIGN,    // |=
    TOKEN_BITWISE_XOR_ASSIGN,   // ^=
    TOKEN_SHIFT_LEFT_ASSIGN,    // <<=
    TOKEN_SHIFT_RIGHT_ASSIGN,   // >>=

    TOKEN_EQUAL,                // ==
    TOKEN_NOT_EQUAL,            // !=
    TOKEN_LESS_THAN,            // <
    TOKEN_LESS_EQUAL_THAN,      // <=
    TOKEN_GREATER_THAN,         // >
    TOKEN_GREATER_EQUAL_THAN,   // >=

    TOKEN_QUESTION,             // ?
    TOKEN_BACKSLASH,            // \ 

    TOKEN_IF,                   // if
    TOKEN_ELSE,                 // else
    TOKEN_WHILE,                // while
    TOKEN_DO,                   // do
    TOKEN_FOR,                  // for
    TOKEN_SWITCH,               // switch
    TOKEN_CASE,                 // case
    TOKEN_DEFAULT,              // default
    TOKEN_CONTINUE,             // continue
    TOKEN_BREAK,                // break

    TOKEN_LABEL,                // label
    TOKEN_JUMP,                 // jump

    TOKEN_TRY,                  // try
    TOKEN_CATCH,                // catch
    TOKEN_THROW,                // throw

    TOKEN_FUNCTION,             // fun (alternative to void)
    TOKEN_LAMBDA,               // lambda
    TOKEN_RETURN,               // return

    TOKEN_TYPEOF,               // typeof
    TOKEN_SIZEOF,               // sizeof

    TOKEN_MALLOC,               // malloc
    TOKEN_CALLOC,               // calloc
    TOKEN_REALLOC,              // realloc
    TOKEN_FREE,                 // free

    TOKEN_MEMCPY,               // memcpy
    TOKEN_MEMSET,               // memset
    TOKEN_MEMMOV                // memmov
} TokenType;

typedef struct {
    const char *src;
    size_t pos;
    int line;
    int column;
} Lexer;

typedef struct {
    TokenType type;

    union {
        PrimitiveType value;
        char *identifier;
    };
    
    char *lexeme;
    int line;
    int column;
} Token;

void initLexer(Lexer *lexer, char *src);
#endif