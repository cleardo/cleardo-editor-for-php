#include "common.h"
#include "global.h"

FILE* infile;	// 输入文件
char ch;		// 当前读入的字符
int cc;			// char count，当前读入的字符计数
int ll;			// 缓冲区中保存的字符计数
char line[BUFFER_SIZE];	// 缓冲区

int sym;		// 当前读入的符号

hpointer list, sentinel;
hpointer h;
pointer p;
FirstSet FS[10];
int FirstSetLength = 1;