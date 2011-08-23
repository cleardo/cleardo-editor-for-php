#include "common.h"
#include "lexer.h"
#include <string>

/**
 * 程序错误
 */
void Error(string ErrorMsg)
{
	cout<<ErrorMsg<<endl;
	exit(1);
} 

void FatalError(string ErrorMsg)
{
	cout<<ErrorMsg<<endl;
	exit(2);
}

void error()
{
	cout<<endl;
	cout<<"解析失败: \t";
	char ch = lex_curchar();
	printf("%c\n", ch);
	exit(1);
}