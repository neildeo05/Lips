#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
enum TokenTypes {
				 Operator,
				 Number,
};

//Chops until predicate, and returns that predicate
char *chopwhile(int(*fn) (int), char* val) {
  int counter = 0;
  while(fn(val[counter]) == 1) {
	counter++;
  }
  char* tmp = (char*) malloc(counter);
  for(int i = 0; i < counter; i++) {
	tmp[i] = *val;
	val++;
  }
  return tmp;
}
char *removewhile(int (*fn) (int), char *val) {
  while(fn(*val) == 1) {
	val++;
  }
  return val;
}
int isoperator(int a) {
  if(a == '+' || a == '-' || a == '*' || a == '/') {
	return 1;
  }
  return 0;
}
int main() {
  char* tmp = "+ 10 10";
  char* answ = chopwhile(isoperator, tmp);
  tmp = removewhile(isoperator, tmp);
  tmp = removewhile(isspace, tmp);
  answ = chopwhile(isalnum, tmp);
  printf("%s\n %s\n", answ, tmp);
  free(answ);
  return 0;
}
