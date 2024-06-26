#ifndef TOKEN_H
#define TOKEN_H


typedef enum {
     T_EOF=0,
     T_WHILE,
     T_ADD,
     T_IDENT,
     T_NUMBER,
     T_ERROR,
     T_ARRAY,
     T_AUTO,
     T_BOOL,
     T_CHAR,
     T_CHARL, //10
     T_ELSE,
     T_FALSE,
     T_FUNCTION,
     T_TRUE,
     T_VOID,
     T_FOR,
     T_IF,
     T_INT,
     T_PRINT,
     T_STRING, //20
     T_STRINGL,
     T_RETURN,
     T_SUB,
     T_MULT,
     T_DIV,
     T_ASSIGN,
     T_RPAREN,
     T_LPAREN,
     T_LBRACKET,
     T_RBRACKET, //30
     T_INCREMENT,
     T_DECREMENT,
     T_NEGATE,
     T_NOT,
     T_EXP,
     T_MOD,
     T_LESS,
     T_LESSEQ,
     T_GREAT,
     T_GREATEQ, //40
     T_EQ,
     T_NEQ,
     T_AND,
     T_OR,
     T_SCOLON,
     T_COLON,
     T_QUOTE,
     T_DQUOTE,
     T_RCBRACE,
     T_LCBRACE,
     T_COMMA,
} token_t;
#endif
