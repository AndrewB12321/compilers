#include <stdio.h>

/* Clunky: Declare the parse function generated from parser.bison */
extern int yyparse();

int main()
{
        printf("CSE 40243 Expression Validator\n");
        printf("Enter an infix expression using the operators +-*/() ending with ;\n\n");

	if(yyparse()==0) {
		printf("Parse successful!\n");
		return 0;
	} else {
		printf("Parse failed.\n");
		return 1;
	}
}