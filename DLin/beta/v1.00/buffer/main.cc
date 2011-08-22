#include "common.h"
#include "global.h"
#include "symbol.h"
#include "lexer.h"
#include "buffer.h"
#include "production.h"

int main(int argc, char *argv[])
{
    char* prog_file_name;
    prog_file_name = *++argv;		// 取得程序文件名
    lex_init_input(prog_file_name);	// 初始化输入缓冲区，填充左半部分缓冲区
    cout<<"处理语法产生式..."<<endl;
    //pdt_declaration();
    int token;
    token = nexttoken();
	cout<<"记号："<<token<<endl;
	cout<<"行号"<<lineno<<endl;
    cout<<lexical_value<<endl;

		token = nexttoken();
	cout<<"记号："<<token<<endl;
	cout<<"行号"<<lineno<<endl;
    cout<<lexical_value<<endl;
lex_buf_dump();
    		token = nexttoken();
	cout<<"记号："<<token<<endl;
	cout<<"行号"<<lineno<<endl;
    cout<<lexical_value<<endl;
    		token = nexttoken();
	cout<<"记号："<<token<<endl;
	cout<<"行号"<<lineno<<endl;
    cout<<lexical_value<<endl;
    symtable_dump();
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
