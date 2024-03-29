#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>
#include <string.h>
#include "mpc.h"
// NOTES //////////
/* operator|char:1:1 '*'
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
/* int number_of_nodes(mpc_ast_t* t) { */
/*   if(t->children_num == 0) { */
/*	return 1; */
/*   } */
/*   int total = 1; */
/*   for(int i = 0; i < t->children_num; i++) { */
/*	total += number_of_nodes(t->children[i]); */
/* } */
/*   return total; */
/* } */

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


// CODE //////////

//lval strcuture
typedef struct {
  int type;
  long num;

  char* err;
  char* sym;

  int count;
  struct lval** cell;

} lval;

//lval types
enum {
				 LVAL_NUM,
				 LVAL_ERR,
				 LVAL_SYM,
				 LVAL_SEXPR,
};
//lval error types
enum {
				 LERR_DIV_ZERO,
				 LERR_BAD_OP,
				 LERR_BAD_NUM,
};
//wrapper for creating an lval_num
lval* lval_num(long x) {
  //  lval v;
  //  v.type = LVAL_NUM;
  //  v.num = x;
  //  return v;
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_NUM;
  v->num = x;
  return v
}
lval* lval_err(char* m) {
  //  lval v;
  //  v.type = LVAL_ERR;
  //  v.err = x;
  //  return v;
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_ERR;
  v->err = malloc(strlen(m) + 1);
  strcpy(v->err, m);
  return v;
}
lval* lval_sym(char* s) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SYM;
  v->sym = malloc(strlen(s) + 1);
  return v;
}

lval* lval_sexpr(void) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SEXPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

void lval_del(lval *v) {
  switch (v->type) {
  case LVAL_NUM: break;
  case LVAL_ERR:
	free(v->err);
	break;
  case LVAL_SYM:
	free(v->sym);
	break;
  case LVAL_SEXPR:
	for (int i = 0; i < v->count; i++) {
	  lval_del(v->cell[i]);
	}
	free(v->cell);
	break;
  }
  free(v);
}
lval* lval_read_num(mpc_ast_t* t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE ? lval_num(x) : lval_err("invalid number");
}
lval* lval_add(lval* v, lval* x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  v->cell[v->count-1] = x;
  return v;
}

lval* lval_read(mpc_ast_t* t) {
  if(strstr(t->tag, "number")) {
	return lval_read_num(t);
  }
  if (strstr(t->tag, "symbol")) {
	return lval_sym(t->contents);
  }
  lval *x = NULL;
  if (strcmp(t->tag, ">") == 0 ){
	x = lval_sexpr()
  }
  if (strstr(t->tag, "sexpr")){
	x = lval_sexpr()
  }
  for(int i = 0; i < t->children_num; i++) {
	if(strcmp(t->children[i]->contents, "(") == 0) {
	  continue;
	}
	if(strcmp(t->children[i]->contents, ")") == 0) {
	  continue;
	}
	if(strcmp(t->children[i]->tag, "regex") == 0) {
	  continue;
	}
	x = lval_add(x, lval_read(t->childrn[i]));
  }
  return x;
}
//IMPLEMENT lval_expr_print

void lval_print(lval v) {
  switch (v.type) {
  case LVAL_NUM: printf("%ld", v.num); break;
  case LVAL_ERR:
	if(v.err == LERR_DIV_ZERO) {
	  printf("Error: Division by Zero... broken broken broken broken broke.");
	}
	if (v.err == LERR_BAD_OP) {
	  printf("Error: Invalid operand");
	}
	if (v.err == LERR_BAD_NUM) {
	  printf("Error: Invalid Number :) (:");
	}
	break;
  }
}
void lval_println(lval v) {
  lval_print(v);
  printf("\n");
}

lval eval_op(lval x, char* op, lval y) {
  /* Evaluate the operand by comparing char* op and "+", "-", "*", and "/" */
  if (x.type == LVAL_ERR) return x;
  if (y.type == LVAL_ERR) return y;

  if(strcmp(op, "+") == 0) return lval_num(x.num + y.num);
  if(strcmp(op, "-") == 0) return lval_num(x.num - y.num);
  if(strcmp(op, "*") == 0) return lval_num(x.num * y.num);
  if(strcmp(op, "/") == 0) {
	return y.num == 0
	  ? lval_err(LERR_DIV_ZERO)
	  : lval_num(x.num/y.num);
  }
  if(strcmp(op, "%") == 0) {
	return y.num == 0
	  ? lval_err(LERR_DIV_ZERO)
	  : lval_num(x.num % y.num);
	  };
  /* If none are true, return 0; This should never occur because of syntax checking*/
  return lval_err(LERR_BAD_OP);
}
/*
 >
  regex
  operator|char:1:1 '+'
  expr|number|regex:1:3 '10'
  expr|number|regex:1:6 '10'
  regex
*/
lval eval(mpc_ast_t* t) {
  //Base Case -> if the current tag is a number (expr|number|regex) (return the current number as a int)
  if (strstr(t->tag, "number")) {
	errno = 0;
	long x = strtol(t->contents, NULL, 10);
	return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
  }
  //Operator has to be the second thing in the expression (after regex)
  char *op = t->children[1]->contents; //first is plus
  lval x = eval(t->children[2]); //x is 10

  int i = 3; //ignore the operator and the first number
  while(strstr(t->children[i]->tag, "expr")) { //should not be an operator or a parentheses
	x = eval_op(x, op, eval(t->children[i])); //evaluate the first number, the current number and the op
	i++; //increment the counter to go past the currentt value in the expression
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
	  symbol : '+' | '-' | '*' | '/' | '%' ;          \
	  sexpr : '(' <expr>* ')' ;                         \
	  expr     : <number> | <symbol> | <sexpr>;  \
	  lips     : /^/ <operator> <expr>+ /$/ ;           \
	  ",
			Number, Symbol, Sexpr, Expr, Lips);
  fprintf(stdout, "LIPS v. 0.1 BETA\n");
  fprintf(stdout, "Ctrl-C to exit\n\n");
  while (1) {
   char* input = readline("Lips> ");
   add_history(input);
   mpc_result_t r;
   if(mpc_parse("<stdin>", input, Lips, &r)) {
	 lval result = eval(r.output);
	 lval_println(result);
	 mpc_ast_delete(r.output);
   }
   else {
	 mpc_err_print(r.error);
	 mpc_err_delete(r.error);
   }
   free(input);
  }
  mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Lips);
  return 0;
}
