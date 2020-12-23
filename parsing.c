#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>
#include <string.h>
#include "mpc.h"
/* Evaluation

operator|char:1:1 '*'
expr|number|regex:1:3 '10'
expr|>
   char:1:6 '('
   operator|char:1:7 '+'
   expr|number|regex:1:9 '1'
   expr|number|regex:1:11 '51'
   char:1:13 ')'


Number is always a number -> has no children. Can just convert it to children (This should be the base case)
Expr that is not number -> second character is an operator
Accumulator - use long for the value
atoi - (char* -> int)
strcmp - (char* -> char* -> integer): lexicographically compare, if equals, return 0
strstr - (char* -> char* -> char*): returns a pointer to the location of the second string in the first
or 0
strcmp to check which operator to use, strstr to check if a tag contains some substring
*/

int number_of_nodes(mpc_ast_t* t) {
  if(t->children_num == 0) {
	return 1;
  }
  int total = 1;
  for(int i = 0; i < t->children_num; i++) {
	total += number_of_nodes(t->children[i]);
  }
  return total;
}
/* TODO */
long eval_op(long x, char* op, long y) {
  /* Evaluate the operand by comparing char* op and "+", "-", "*", and "/" */
  if(strcmp(op, "+") == 0) return x + y;
  if(strcmp(op, "-") == 0) return x - y;
  if(strcmp(op, "*") == 0) return x * y;
  if(strcmp(op, "/") == 0) return x / y;
  /* If none are true, return 0; This should never occur because of syntax checking*/
  return 0;
}

/*
CASE 1: {+ 10 10}
-----------------
 >
  regex
  operator|char:1:1 '+'
  expr|number|regex:1:3 '10'
  expr|number|regex:1:6 '10'
  regex
-----------------
CASE 2: {+ (/10 10) 10}
CASE 3: {/ 10 20 + 10}
*/

long eval(mpc_ast_t* t) {
  if (strstr(t->tag, "number")) {
	return atoi(t->contents);
  } //Base Case -> if the current tag is a number (expr|number|regex) (return the current number as a int)
  //Operator has to be the second thing in the expression (after regex)
  char *op = t->children[1]->contents;
  mpc_ast_print(t);
  long x = eval(t->children[2]);
  int i = 3;
  while(strstr(t->children[i]->tag, "expr")) {
	x = eval_op(x, op, eval(t->children[i]));
	i++;
  }
  return x;
}

int main(int argc, char** argv) {
  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr = mpc_new("expr");
  mpc_parser_t* Lips = mpc_new("lips");
  mpca_lang(MPCA_LANG_DEFAULT,
	  "number   : /-?[0-9]+/ ;                          \
	  operator : '+' | '-' | '*' | '/' ;                \
	  expr     : <number> | '(' <operator><expr>+ ')';  \
	  lips     : /^/ <operator> <expr>+ /$/ ;           \
	  ",
	  Number, Operator, Expr, Lips);

  fprintf(stdout, "LIPS v. 0.1 BETA\n");
  fprintf(stdout, "Ctrl-C to exit\n\n");
  while (1) {
   char* input = readline("Lips> ");
   add_history(input);
   mpc_result_t r;
   if(mpc_parse("<stdin>", input, Lips, &r)) {
	 long result = eval(r.output);
	 printf("%ld\n", result);
	 mpc_ast_delete(r.output);
   }
   else {
	 mpc_err_print(r.error);
	 mpc_err_delete(r.error);
   }
   free(input);
  }
  mpc_cleanup(4, Number, Operator, Expr, Lips);
  return 0;
}
