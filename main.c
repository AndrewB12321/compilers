#include <stdio.h>
#include <string.h>
#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "indent.h"
/* Clunky: Declare the parse function generated from parser.bison */
extern int yyparse();
extern int yylex();
extern char* yytext;
extern struct decl* parser_result;
int main()
{	
	//int t = yylex();
	//printf("id: %d, text: %s\n", t, yytext);
	if(yyparse()==0) {
		printf("Parse successful!\n");
		decl_print(parser_result, 0);
		decl_delete(parser_result);
		return 0;
	} else {
		printf("Parse failed.\n");
		printf("remaining tokens\n");
		for(int i = 0; i < 10; i++) {
			printf("text: %s\n",yytext);
			yylex();
		}
		decl_delete(parser_result);
		return 1;
	}
}
