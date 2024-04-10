#include <stdio.h>
#include <string.h>
/* Clunky: Declare the parse function generated from parser.bison */
extern int yyparse();
extern int yylex();
extern char* yytext;
int main()
{	
	//int t = yylex();
	//printf("id: %d, text: %s\n", t, yytext);
	if(yyparse()==0) {
		printf("Parse successful!\n");
		return 0;
	} else {
		printf("Parse failed.\n");
		printf("remaining tokens\n");
		for(int i = 0; i < 10; i++) {
			printf("text: %s\n",yytext);
			yylex();
	}
		return 1;
	}
}
