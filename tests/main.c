#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>
#include <string.h>
#include "token.c"
int main(int argc, char** argv) {
  fprintf(stdout, "LIPS v. 0.1 BETA\n");
  fprintf(stdout, "Ctrl-C to exit\n\n");
  while (1) {
    char* input = readline("Lips> ");
    add_history(input);
    NextLine(input);
    free(input);
  }
  return 0;
}
