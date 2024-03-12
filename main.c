#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#define YYLMAX 256
extern FILE *yyin;
extern int yylex();
extern char *yytext;
extern int yylineno;

int main(int argc, char* argv[])
{
	if(argc != 2) {
		printf("need file as argument\n");
		exit(1);
	}
	yyin = fopen(argv[1],"r");
	if(!yyin) {
		printf("could not open program.c!\n");
		return 1;
	}

	while(1) {
		token_t t = yylex();
		if(t==T_EOF) break;
		if(t != T_ERROR)
			printf("token: %d  text: %s, line: %d\n",t,yytext, yylineno);
		else 
			printf("Unidentified token: %s, line: %d\n",yytext, yylineno);
	}
}
