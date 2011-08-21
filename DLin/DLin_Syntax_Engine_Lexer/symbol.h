#include "global.h"

#define STRMAX 999  // 词素字符数组的大小，最多存储的标识符总长度为999个字符
#define SYMMAX 100  // 最多支持100个标识符

struct entry;

int sym_lookup(char s[]);
int sym_insert(char s[], int tok);

/**
  符号表测试函数
  */
void symtable_dump();
void lexemes_dump();