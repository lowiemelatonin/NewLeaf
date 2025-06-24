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

} TokenType;

#endif