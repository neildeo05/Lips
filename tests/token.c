#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenTypes {
  Operator,
  Number,
  Paren,
};

struct Token {
  enum TokenTypes type;
  char val;
};

struct Token *arr;

void NextToken(char tok) {
  switch(tok) {
  case '+':
    break;
  case '-':
    break;
  case '*':
    break;
  case '/':
    break;
  }
  //  printf("%c\n", tok);
  struct Token tmp = {Operator, tok};
  struct Token *iterator = arr;
  iterator = &tmp;
  iterator++;
  // arr++;p
}

void NextLine(char* line) {
  for(int i = 0; i < strlen(line); i++) {
      if(isspace(line[i]) != 1) {
	NextToken(line[i]);
      }
  }
  printf("%c\n", arr->val);
}

