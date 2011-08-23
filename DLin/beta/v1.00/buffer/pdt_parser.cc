/**
 * 语法产生式解析器函数实现
 */
#include "common.h"
#include "global.h"
#include "symbol.h"
#include "lexer.h"
#include "production.h"
#include "pdt_parser.h"
/*
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
    char sym[200];
    pdt_pointer entry;	    // 指向语法产生式起始结点
    pdt_hpointer suc;	    // 指向可选语法产生式
};
*/
void pdt_factor(pdt_pointer &p, pdt_pointer &q)
{
    /*
    if (pdt_terminalLookup(lexical_token))
    {
	 // 如果是单个记号，终结符或非终结符
	 nexttoken();    // terminal: ';'
    }
    */
    pdt_pointer a, b;
    pdt_hpointer h;

    int tsym_index;
    if (lexical_token == ID || lexical_token == PDT_LITERAL)
    {
	a = pdt_new_node();
	//a = (pdt_pointer)malloc(sizeof(struct pdt_node));
	if ((tsym_index = pdt_terminalLookup(lexical_value)))
	{
	    // 终结符
	    // a->terminal = 1;
	    // a->sym.tsym = tsym_index;	// 在终结符号表中的索引
	    // a->alt = a->suc = NULL;
	    pdt_set_tsym_value(a, tsym_index, NULL, NULL);
	    //cout<<"tsym: "<<tsym_index<<endl;
	}
	else 
	{
	    pdt_headerFind(sym_getSymbolEntry(lexical_value), h);
	    //a->terminal = 0;
	    //a->sym.nsym = h;
	    //a->alt = a->suc = NULL;
	    pdt_set_nsym_value(a, h, NULL, NULL);
	}
	p = a;
	q = a;
	nexttoken();
    } 

    /*
    else
    {
	// 解析"[term]"这种形式的产生式结构，由于可用(production | #)代替，故暂不支持这种形式的产生式
	if (token == LEFT_SQBRACKET)
	{
	    nexttoken();
	    pdt_term();
	    if (token == RIGHT_SQBRACKET)
	    {
		nexttoken();
	    }
	}
    }
    */
}

void pdt_term(pdt_pointer &p, pdt_pointer &q, pdt_pointer &r)
{
    pdt_pointer a, b, c;
    pdt_factor(p, a);
    q = a;
    while (lexical_token == ID || lexical_token == PDT_LITERAL)
    {
	pdt_link_factor(a, b);
    }
    r = a;
    //pdt_header_output(p->sym.nsym);
    //pdt_pointer_output(pdt_pointer p);
}

void pdt_expression(pdt_pointer &p, pdt_pointer &q)
{
    pdt_pointer a, b, c;
    pdt_term(p, a, c);
    while (lexical_token == PDT_ALTERNATE)
    {
	nexttoken();
	pdt_link_term(p, a, b, c);
    }
    q = a;
}

