#include "common.h"
#include "global.h"
#include "symbol.h"
#include "lexer.h"

int main(int argc, char *argv[])
{
    char* prog_file_name;
    prog_file_name = *++argv;   // 取得程序文件名
    lex_init_input(prog_file_name);
    int token;
    token = nexttoken();
	cout<<"记号："<<token<<endl;
	cout<<"行号"<<lineno<<endl;
    cout<<lexical_value<<endl;
	cout<<"符号表输出："<<endl;
	symtable_dump();
	copy_left_buf_dump();
	token = nexttoken();
	cout<<"记号："<<token<<endl;
	cout<<"行号"<<lineno<<endl;
    cout<<lexical_value<<endl;
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
