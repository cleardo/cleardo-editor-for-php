#define LEX_BUF_ALLOCATED	10					// 分配的缓冲区大小
#define LEX_BUF_SIZE        LEX_BUF_ALLOCATED-2 // 输入缓冲区的大小
#define LEX_BUF_HALF_SIZE   (LEX_BUF_SIZE)/2		// 输入缓冲区的一半
#define blank 	' '
#define tab 	'\t'
#define cr		'\r'
#define newline	'\n'
	
typedef int token;

void lex_init_input(char*);  // 初始化输入文件指针
void lex_load_temp_buf();
void lex_load_left();   // 加载缓冲区左半部分
void lex_load_right();  // 加载缓冲区右半部分
int copy_left_buf();
char nextchar();
token nexttoken();
void ungetchar();
void install_id();
void install_num();
void set_lexeme_begin();
void retract(int);		// 撤回指针
int fail();
void recover();			// 词法分析错误恢复

/**
 * 测试函数
 */
void nextchar_dump(int);
void lex_buf_dump();
void lex_eof_test();
void copy_left_buf_dump();