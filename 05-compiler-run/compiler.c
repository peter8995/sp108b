#include <assert.h>
#include "compiler.h"

int  E();
void STMT();
void IF();
void While();
void For();
void BLOCK();
void GOTO();

int tempIdx = 1, labelIdx = 1;

#define nextTemp() (tempIdx++)
#define nextLabel() (labelIdx++)
#define NEXT(var, tags)  char var[LEN];strcpy(var, token);next(tags)

int isNext(char *set) {
  char eset[SMAX], etoken[SMAX];
  sprintf(eset, " %s ", set);
  sprintf(etoken, " %s ", tokens[tokenIdx]);
  return (tokenIdx < tokenTop && strstr(eset, etoken) != NULL);
}

int isNextType(TokenType type) {
  return (types[tokenIdx] == type);
}

int isEnd() {
  return tokenIdx >= tokenTop;
}

char *next() {
  // printf("%02d:token = %-10s type = %-10s\n", tokenIdx, tokens[tokenIdx], typeName[types[tokenIdx]]);
  return tokens[tokenIdx++];
}

char *skip(char *set) {
  if (isNext(set)) {
    return next();
  } else {
    error("skip(%s) got %s fail!\n", set, next());
  }
}

char *skipType(TokenType type) {
  if (isNextType(type)) {
    return next();
  } else {
    error("skipType(%s) got %s fail!\n", typeName[type], typeName[types[tokenIdx]]);
  }
}

// CALL(id) = (E*)
int CALL(char *id) {
  assert(isNext("("));
  skip("(");
  int e[100], ei = 0;
  while (!isNext(")")) {
    e[ei++] = E();
    if (!isNext(")")) skip(",");
  }
  for (int i=0; i<ei; i++) {
    irEmitArg(e[i]);
  }
  skip(")");
  irEmitCall(id, ei);
}

// F = (E) | Number | Id | CALL
int F() {
  int f;
  if (isNext("(")) { // '(' E ')'
    next(); // (
    f = E();
    next(); // )
  } else { // Number | Id | CALL
    f = nextTemp();
    char *item = next();
    irEmitAssignTs(f, item);
    // emit("t%d = %s\n", f, item);
  }
  return f;
}

// E = F (op E)*
int E() {
  int i1 = F();
  while (isNext("+ - * / & | < > = <= >= == != && ||")) {
    char *op = next();
    int i2 = E();
    int i = nextTemp();
    irEmitOp2(i, i1, op, i2);
    // emit("t%d = t%d %s t%d\n", i, i1, op, i2);
    i1 = i;
  }
  return i1;
}

int EXP() {
  tempIdx = 1; // 讓 temp 重新開始，才不會 temp 太多！
  return E();
}

// ASSIGN = id '=' E
void ASSIGN(char *id) {
  // char *id = next();
  skip("=");
  int e = EXP();
  irEmitAssignSt(id, e);
  // emit("%s = t%d\n", id, e);
}

// while (E) STMT
void WHILE() {
  int whileBegin = nextLabel();
  int whileEnd = nextLabel();
  irEmitLabel(whileBegin);
  // emit("(L%d)\n", whileBegin);
  skip("while");
  skip("(");
  int e = E();
  irEmitIfNotGoto(e, whileEnd);
  // emit("goif T%d L%d\n", whileEnd, e);
  skip(")");
  STMT();
  irEmitGoto(whileBegin);
  // emit("goto L%d\n", whileBegin);
  irEmitLabel(whileEnd);
  // emit("(L%d)\n", whileEnd);
}

//IF = if (E) STMT (else STMT)?
void IF() {
  int ifBegin = nextLabel();
  int ifEnd = nextLabel();
  skip("if");
  skip("(");
  int e = E();
  irEmitIfNotGoto(e, ifBegin);
  skip(")");
  STMT();
  irEmitGoto(ifEnd);
  if (isNext("else")) {
    skip("else");
    STMT();
  }
  irEmitLabel(ifEnd);
}

// FOR=for (E;E;E) STMT
void FOR() {
  int forStart = nextLabel();
  int forEnd = nextLabel();
  skip("for");
  skip("(");
  STMT();
  irEmitLabel(forStart);
  //若接下來不是for(E;E;E)內的分號
  if(!(isNext(";"))){
    int e = E();
    irEmitIfNotGoto(e, forEnd);
  }
  skip(";");
  char *id = next();
  ASSIGN(id);
  skip(")");
  STMT();
  irEmitGoto(forStart);
  irEmitLabel(forEnd);
}

void GOTO(){
  int goLabel = nextLabel();
  skip("goto");
  //觀察到冒號後
  while(1){
    next();
    if(isNext(":")) break;
  }
  skip(":");
  irEmitGoto(goLabel);
  {
    irEmitLabel(goLabel);
    STMT();
  }
}

void STMT() {
  if (isNext("while"))
    WHILE();
  else if (isNext("for"))
    FOR();
  else if (isNext("if"))
    IF();
  else if (isNext("{"))
    BLOCK();
  else if (isNext("goto"))
    GOTO();
  else {
    char *id = next();
    if (isNext("(")) {
      CALL(id);
      skip(";");
    } 
    else {
      ASSIGN(id);
      skip(";");
    }
  }
}

void STMTS() {
  while (!isEnd() && !isNext("}")) {
    STMT();
  }
}

// { STMT* }
void BLOCK() {
  skip("{");
  STMTS();
  skip("}");
}

void PROG() {
  STMTS();
}

void parse() {
  // printf("============ parse =============\n");
  tokenIdx = 0;
  PROG();
}
