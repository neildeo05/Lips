#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
  char a;
} Operator;

typedef struct {
  char a;
} Number;

typedef struct {
  char a;
  char* rest;
} HeadTailPair;

typedef struct{
  HeadTailPair just;
  bool nothing;
} MaybeHTP;

typedef struct{
  Number just;
  bool nothing;
} MaybeNum;

typedef struct{
  Operator just;
  bool nothing;
} MaybeOp;
typedef struct {
  bool nothing;
} MaybeGen;
MaybeHTP JustHTP(HeadTailPair a) {
  MaybeHTP ans = {a, false};
  return ans;
}
MaybeHTP NothingHTP() {
  MaybeHTP ans = {.nothing=true};
  return ans;
}

void DisplayHTP(MaybeHTP a) {
  if(a.nothing == false) {
	HeadTailPair extract = a.just;
	printf("Just (%c, %s)\n", extract.a, extract.rest);
  }
  else {
	printf("Nothing\n");
  }
}
MaybeHTP CharParse(char target, char* str) {
  if(*str == target) {
	HeadTailPair ret = {*str, str+1};
	return JustHTP(ret);
  }
  return NothingHTP();
}
void StrParse(char* match, char* str) {
  int len = strlen(match);
  int counter = 0;
  MaybeHTP t;
  if(len > strlen(str)) exit(1);
  while (counter < len) {
	t = CharParse(*(str+counter), match+counter);
	DisplayHTP(t);
	counter++;
  }
}


int main() {
  char* tmp = "/key:value/";
  StrParse(tmp, "/key:value2/");
}


