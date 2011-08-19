#include "common.h"
#include "global.h"
#include "symbol.h"

struct entry {
	char *lexptr;	// 标识符指针，指向标识符在词素字符数组中的起始索引位置
	int token;		// 标识符类型，关键字，普通标识符等
};

char lexemes[STRMAX];
int lastchar = -1;  // 词素字符数组使用的最后一个索引
struct entry symtable[SYMMAX];
int lastentry = 0;  // 使用的符号表项的最后一个索引

int sym_lookup(char s[])
{
	int p;
	for (p = lastentry; p > 0; p--)
	{
		// cout<<lastentry<<endl;
		if (strcmp(symtable[p].lexptr, s) == 0)
			return p;
	}
	return 0;
}
	
int sym_insert(char s[], int tok)
{
  int len;
  len = strlen(s);  // 计算标识符的长度
  if (lastentry + 1 >= SYMMAX)
    Error("符号表已满，标识符个数超出！");
  if (lastchar + len + 1 >= STRMAX)
    Error("标识符长度总和超出！");
  lastentry++;	// 位置0不使用，作为哨兵
  symtable[lastentry].token = tok;
  symtable[lastentry].lexptr = &lexemes[lastchar + 1];
  lastchar = lastchar + len + 1;
  strcpy(symtable[lastentry].lexptr, s);
  return lastentry;
}

void symtable_dump()
{
	int i;
	for (i = 1; i <= lastentry; i++)
	{
		cout<<symtable[i].lexptr<<'\t'<<symtable[i].token<<endl;
	}
	return;
}

void lexemes_dump()
{
	ArrayDump(lexemes, 6);
}

