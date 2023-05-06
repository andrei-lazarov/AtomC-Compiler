#ifndef TOKEN_H
#define TOKEN_H

typedef struct _Token {
    int code; // code (name)
    union{
        char        *text; // used for ID, CT_STRING (dynamically allocated)
        long int    i; // used for CT_INT, CT_CHAR
        double      r; // used for CT_REAL
    };
    int line; // the input file line
    struct _Token *next; // link to the next token
}Token;

#endif
