/**
 * 语法产生式数据结构定义与操作函数
 */
#include "production.h"

char tsym_chars[TSYMSTRMAX];	// 用于保存终结符的字符串
char* tsym_table[TSYMMAX];	// 用于保存终结符表

struct pdt_node
{
    int terminal;   // 是否终结符：1 - 终结符; 2 - 非终结符
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
    pdt_pointer entry;	// 指向语法产生式起始结点
    pdt_hpointer suc;	// 指向可选语法产生式
};