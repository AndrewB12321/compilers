%{
#include "parser.h"
static void comment(void);
extern void yyerror(const char *);  /* prints grammar violation message */
%}
%option yylineno
DIGIT  [0-9]
LETTER [a-zA-Z]
%%
(" "|\t|\n)  /* skip whitespace */
\/\/(.+)?\n     /* skip c++ style comments */
"/*"       { comment(); }
"array"    { return T_ARRAY;     }
"auto"     { return T_AUTO;      }
"boolean"  { return T_BOOL;      }
"char"     { return T_CHAR;      }
"else"     { return T_ELSE;      }
"false"    { return T_FALSE;     }
"for"      { return T_FOR;       }
"function" { return T_FUNCTION;  }
"if"       { return T_IF;        }
"integer"  { return T_INT;       }
"print"    { return T_PRINT;     }
"return"   { return T_RETURN;    }
"string"   { return T_STRING;    }
"true"     { return T_TRUE;      }
"void"     { return T_VOID;      }
"while"    { return T_WHILE;     }
 \+        { return T_PLUS;      }
 "("       { return T_LPAREN;    }
 ")"       { return T_RPAREN;    }
 "["       { return T_LBRACKET;  }
 "]"       { return T_RBRACKET;  }
 "++"      { return T_INCREMENT; }
 "--"      { return T_DECREMENT; }
 "-"       { return T_MINUS;     }     
 "*"       { return T_MULT;      }
 "/"       { return T_DIV;       }
 "%"       { return T_MOD;       }
 "<"       { return T_LESS;      }
 ">"       { return T_GREAT;     }
 "<="      { return T_LESSEQ;    }
 ">="      { return T_GREATEQ;   }
 "=="      { return T_EQ;        }
 "!="      { return T_NEQ;       }
 "!"       { return T_NOT;       }
 "&&"      { return T_AND;       }
 "||"      { return T_OR;        }
 "="       { return T_ASSIGN;    }
 ";"       { return T_SCOLON;    }
 ":"       { return T_COLON;     }
 \"        { return T_DQUOTE;    }
 '         { return T_QUOTE;     }
 "{"       { return T_LCBRACE;   }
 "}"       { return T_RCBRACE;   }
 ","       { return T_COMMA;     }
 "^"       { return T_EXP;       }
 \".*\"    { return T_STRINGL;   }
 '{LETTER}*' {return T_CHARL;    }
({LETTER}|_)({LETTER}|_|{DIGIT})*  { return T_IDENT;     }
{DIGIT}+   { return T_NUMBER;    }
.          { return T_ERROR;     }
%%

int yywrap() { return 1; }

static void comment(void)
{
    int c;

    while ((c = input()) != 0)
        if (c == '*')
        {
            while ((c = input()) == '*')
                ;

            if (c == '/')
                return;

            if (c == 0)
                break;
        }
    yyerror("unterminated comment");
}
