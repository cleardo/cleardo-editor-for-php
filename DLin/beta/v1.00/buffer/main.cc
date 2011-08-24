#include "common.h"
#include "global.h"
#include "symbol.h"
#include "lexer.h"
#include "buffer.h"
#include "production.h"
#include "pdt_parser.h"
#include "first.h"
#include "error.h"


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


int main(int argc, char *argv[])
{
    char* prog_file_name;
    int dlin_token;
    prog_file_name = *++argv;		// 取得程序文件名
    lex_init_input(prog_file_name);	// 初始化输入缓冲区，填充左半部分缓冲区
    cout<<"开始解析语法产生式..."<<endl;
    pdt_pointer pdt_tail;  // 指向语法产生式数据结构的指针
    pdt_initPdtSentinel();  // 初始化语法产生式头结点链表哨兵元素

    if ((dlin_token = pdt_declaration()) == PDT_DELIMIT)
    {	
	// 处理翻译规则部分
	while((dlin_token = nexttoken()) != PDT_DELIMIT)
	{
	    pdt_headerFind(sym_getSymbolEntry(lexical_value), pdt_headFound);
	    
	    if (dlin_token == ID)
	    {
		// 取得下一个记号':'
		dlin_token = nexttoken();
	    }
	    else
	    {
		error();
	    }
	    if (dlin_token == PDT_DERIV)
	    {
		// 推导记号
		dlin_token = nexttoken();   // 对于输入"line : expr;"，则这里词法分析结果为 ID: expr
	    }
	    else
	    {
		error();
	    }
	    expr_parse(pdt_headFound, pdt_tail);
	    if (lexical_token != PDT_EXPR_END)
	    {
		error();
	    }

	    cout<<endl;
	    //break;
	}
	// cout<<pdt_terminalLookup(5)<<pdt_terminalLookup(2);
	//slr_initPdtArr();
	//slr_ipointer slr_ilist;
	//slr_ilist = slr_initI0();
	
	//slr_added_test();
	//slr_item_closure(slr_ilist);
	
	//slr_item_op();
/*
	 FSList fsl, fsl2;
    CreateFSL(fsl);
    fsl = FIRST(list);
    PrintFSL(fsl);
    // fsl = FIRST_Str(list->suc->entry);
    fsl = FOLLOW(list);
    // FOLLOW2(list->suc->suc->suc->entry, fsl, 'A');
    PrintFSL(fsl);
*/
/*
	slr_ipointer slr_ilist0 = slr_goto(slr_ilist, 1);
	cout<<"goto1:"<<endl;
	SlrPrint(slr_ilist0);

	slr_ipointer slr_ilist1 = slr_goto(slr_ilist, 2);
	cout<<"goto2:"<<endl;
	SlrPrint(slr_ilist1);
	
	slr_ipointer slr_ilist3 = slr_goto(slr_ilist, 3);
	cout<<"goto3:"<<endl;
	SlrPrint(slr_ilist3);
	slr_ipointer slr_ilist4 = slr_goto(slr_ilist, 4);
	cout<<"goto4:"<<endl;
	SlrPrint(slr_ilist4);
	slr_ipointer slr_ilist5 = slr_goto(slr_ilist, 5);
	cout<<"goto5:"<<endl;
	SlrPrint(slr_ilist5);
	slr_ipointer slr_ilist6 = slr_goto(slr_ilist, 6);
	cout<<"goto6:"<<endl;
	SlrPrint(slr_ilist6);
	slr_ipointer slr_ilist7 = slr_goto(slr_ilist, 7);
	cout<<"goto7:"<<endl;
	SlrPrint(slr_ilist7);
	slr_ipointer slr_ilist8 = slr_goto(slr_ilist, 8);
	cout<<"goto8:"<<endl;
	SlrPrint(slr_ilist8);
	slr_ipointer slr_ilist9 = slr_goto(slr_ilist, 9);
	cout<<"goto9:"<<endl;
	SlrPrint(slr_ilist9);
	slr_ipointer slr_ilist10 = slr_goto(slr_ilist, 10);
	cout<<"goto10:"<<endl;
	SlrPrint(slr_ilist10);
	*/
	//slr_pdt_dump();   // 输出产生式数组 slr_productions
	//slr_pdtNoTest();
	//slr_ipointer test_ilist_1 = slr_goto(slr_ilist, 1);
	//slr_itemlist_dump(test_ilist_1);
	// 进行php例程定义部分
	cout<<"开始解析php自定义例程..."<<endl;
	/*

	*/
	cout<<"php自定义例程部分解析完毕！"<<endl<<endl;

	cout<<"语法产生式生成的符号表内容如下："<<endl;
	symtable_dump();

	cout<<endl<<"终结符号表内容如下："<<endl;
	pdt_tsymtable_dump();

	cout<<endl<<"语法产生式头结点链表内容如下："<<endl;

	pdt_list_dump();

	pdt_tree_dump();
/*
	pdt_header_output(pdt_list->entry->sym.nsym);			    // line 的第一个结点: expr
	pdt_pointer_output(pdt_list->entry->suc);			    // line 的第二个结点: 2
	pdt_header_output(pdt_list->suc->entry->sym.nsym);		    // expr 的第一个结点: expr
	pdt_pointer_output(pdt_list->suc->entry->suc);			    // expr 的第二个结点: 3
	pdt_header_output(pdt_list->suc->entry->suc->suc->sym.nsym);	    // expr 的第三个结点: term
	pdt_header_output(pdt_list->suc->entry->alt->sym.nsym);		    // expr 的第一个可选结点: term
	pdt_header_output(pdt_list->suc->suc->entry->sym.nsym);		    // term 的第一个结点: term
	pdt_pointer_output(pdt_list->suc->suc->entry->suc);		    // term 的第二个结点: 4
	pdt_header_output(pdt_list->suc->suc->entry->suc->suc->sym.nsym);   // term 的第三个结点: factor
	pdt_header_output(pdt_list->suc->suc->entry->alt->sym.nsym);	    // term 的第一个可选结点: factor
	pdt_pointer_output(pdt_list->suc->suc->suc->entry);		    // factor 的第一个结点: 5
	pdt_header_output(pdt_list->suc->suc->suc->entry->suc->sym.nsym);   // factor 的第二个结点: expr
	pdt_pointer_output(pdt_list->suc->suc->suc->entry->suc->suc);	    // factor 的第三个结点：6
	pdt_pointer_output(pdt_list->suc->suc->suc->entry->alt);	    // factor 的可选第一个结点：1
	pdt_pointer_output(pdt_list->suc->suc->suc->entry->alt->alt);	    // factor 的可选第一个结点：2
*/
    }
    //pdt_tsymtable_dump();
    
//pointer_dump();
//lex_buf_dump();
//nextchar_dump(8);
	//char c = nextchar();
	//copy_left_buf_dump();
	//printf("xdxd%c\n", c);

	// ArrayDump(lex_temp_buf, 5);
	/*
	token = nexttoken();
	cout<<"记号："<<token<<endl;
	cout<<"行号"<<lineno<<endl;
    cout<<lexical_value<<endl;
		token = nexttoken();
	cout<<"记号："<<token<<endl;
	cout<<"行号"<<lineno<<endl;
    cout<<lexical_value<<endl;
	
	symtable_dump();
	*/
	//lexemes_dump();
    return 0;
}
