#include <stdio.h>
#include <string.h>
#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "symbol.h"
#include "codegen.h"
#include "hash_table.h"
#include "stack.h"

struct stack* global_stack;
struct scratch_register** global_register_Table;
FILE* global_output_file;
int global_label_count;



extern int yyparse();
extern int yylex();
extern char* yytext;
extern struct decl* parser_result;
int main()
{	
	//int t = yylex();
	//printf("id: %d, text: %s\n", t, yytext);

	global_stack = stack_create();
	global_register_Table = create_global_table();
	global_output_file = fopen("./tmp.asm", "w");
	global_label_count = 0;
	scope_enter();

	if(yyparse()==0) {
		printf("Parse successful!\n");
		decl_print(parser_result, 0);
		decl_resolve(parser_result);
		decl_typecheck(parser_result);
		scope_exit();
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
