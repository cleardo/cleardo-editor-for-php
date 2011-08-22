/**
 * 语法产生式数据结构定义与操作函数
 */
#include "common.h"
#include "global.h"
#include "production.h"
#include "lexer.h"

char tsym_chars[TSYMSTRMAX];	// 用于保存终结符的字符串
int pdt_lastchar = -1;		// 终结符字符串使用的最后一个索引
char* tsym_table[TSYMMAX];	// 用于保存终结符表
int pdt_lastentry = 0;		// 终结符表项索引

struct pdt_node
{
    int terminal;	    // 是否终结符：1 - 终结符; 2 - 非终结符
    union {
	int tsym;	    // 终结符，用终结符表索引表示
	pdt_hpointer nsym;  // 非终结符，用非终结符头指针表示
    }sym;
    pdt_pointer suc;	    // 指向下一个语法产生式结点
    pdt_pointer alt;	    // 指向可选语法产生式结点
};

struct pdt_header
{
    char* sym;
    pdt_pointer entry;	    // 指向语法产生式起始结点
    pdt_hpointer suc;	    // 指向可选语法产生式
};

void pdt_declaration()
{
    char* lexeme_str;
    while ((lexical_token = nexttoken()) == PDT_NEWTERMINAL)
    {
	lexical_token = nexttoken();
	cout<<lexical_token<<endl;
	//lex_get_lexeme(lexeme_str);
	//printf("%s\t", lexeme_str);
	//pdt_insertNewTerminal(lexeme_str);
    }
    cout<<lexical_token<<endl;
}

void pdt_insertNewTerminal(char s[])
{
    
}

void pdt_terminalLookup()
{
}