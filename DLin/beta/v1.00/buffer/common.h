/**
 * 公共库文件
 */

// C包含文件
#include <stdio.h>			// EOF(=C^Z或C^D或F6),NULL
#include <ctype.h>			// 字母大小写转换等
#include <malloc.h> 		// malloc()等
#include <limits.h> 		// INT_MAX等
#include <stdlib.h>			// atoi()等
#include <io.h> 			// eof()
#include <process.h> 		// exit()
#include <string.h>

// CPP包含文件
#include <iostream>
#include <string>
using namespace std;

// 函数结果状态代码
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1

typedef int Status;			// Status是函数的类型,其值是函数结果状态代码，如OK等
typedef int Boolean; 		// Boolean是布尔类型,其值是TRUE或FALSE

// 错误处理函数
void Error(string);
void FatalError(string);

// 输出测试函数
void ArrayOutput(char*);
void ArrayDump(char*, int);
int isZeroChar(char);		// 测试字符是否'\0'
int isCRChar(char);			// 测试字符是否'\r'
int isLineChar(char);		// 测试字符是否'\n'
int isEofChar(char);		// 测试字符是否EOF
int getArrayLength(char*);	// 取得数组长度