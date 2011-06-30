#include "common.h"
#include "global.h"

int main(int argc, char *argv[])
{
    char* prog_file_name;
    prog_file_name = *++argv;   // 取得程序文件名
    // printf(prog_file_name);
    lex_init_input(prog_file_name);
    // cout<<LEX_BUF_SIZE<<'\t'<<LEX_BUF_ALLOCATED<<'\t'<<LEX_BUF_HALF_SIZE<<endl;
    // nextchar_dump(10);
    // lex_load_right();
    // lex_buf_dump();
    // lex_eof_test();
    int token;
    token = nexttoken();
 cout<<"记号："<<token<<endl;
token = nexttoken();
    cout<<"记号："<<token<<endl;
	//cout<<"行号"<<lineno<<endl;
     cout<<lexical_value<<endl;
    return 0;
}
