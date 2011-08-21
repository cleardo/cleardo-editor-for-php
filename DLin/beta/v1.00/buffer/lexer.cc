#include "common.h"
#include "global.h"
#include "lexer.h"
#include "symbol.h"
#include "buffer.h"

typedef int token;

FILE* prog_fp;  // 程序输入文件
char char_read; // 当前读入字符
char lex_temp_buf[LEX_BUF_ALLOCATED/2] = {1};
char lex_buff[LEX_BUF_ALLOCATED];
int lexeme_beginning = 0;   // 词素开始指针，为数组下标
int lex_forward = 0;        
int state = 0;  // 当前状态
int start = 0;  // 当前转移图起始状态
int lexical_value = 0;  // 词素属性值
int lexical_token = 0;	// 词素类型
int lineno = 1;
int pass_left = 0;
int pass_right = 0;	// 标记是否越过缓冲区边界
int crossbuf = 0;
char left_buf[LEX_BUF_HALF_SIZE+1];
int numchar_pointer = 0;

void lex_init_input(char* filename)
{
    if ((prog_fp = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "无法打开程序文件：%s", filename);
		exit(1);
    }
    else
    {
        lex_buff[LEX_BUF_HALF_SIZE] = lex_buff[LEX_BUF_ALLOCATED-1] = EOF;
        // 加载一定字符到缓冲区左半部分
        lex_load_left();
    }
}

void lex_load_temp_buf()
{
	int read_count = 0;
	if (!(read_count = lex_getbuff(lex_temp_buf))) {
    	read_count = fread(lex_temp_buf, sizeof(char), LEX_BUF_HALF_SIZE, prog_fp);
	}
    	if (read_count < LEX_BUF_HALF_SIZE)
    	{
        	lex_temp_buf[read_count] = EOF;
        	read_count++;
    	}
    	lex_temp_buf[read_count] = '\0';
}

int copy_left_buf()
{
	int i = 0;
	int lexeme_temp_pointer = lexeme_beginning;
	while(lexeme_temp_pointer != LEX_BUF_HALF_SIZE)
	{
		left_buf[i++] = lex_buff[lexeme_temp_pointer++];
	}
	left_buf[i] = '\0';
	return lexeme_beginning;
}

/**
 * 加载左缓冲区
 */
void lex_load_left()
{
    int i = 0;
    lex_load_temp_buf();
	if (lexeme_beginning < LEX_BUF_HALF_SIZE) {
		crossbuf = copy_left_buf();
	}
    while(lex_temp_buf[i])
    {
        lex_buff[i] = lex_temp_buf[i];
		i++;
    }
}

void lex_load_right()
{
    int i = 0;
    int j = LEX_BUF_ALLOCATED/2;
    lex_load_temp_buf();
    while(lex_temp_buf[i])
    {
        lex_buff[j++] = lex_temp_buf[i++];
    }
}

char nextchar()
{
	/*

	char c;
			if (crossbuf && left_buf[numchar_pointer]) {
				c = left_buf[numchar_pointer++];
				
				lex_forward = LEX_BUF_ALLOCATED/2;
				printf("ssfs---%c\n", c);
				return c;
			}
	*/
    if (lex_buff[lex_forward] == EOF)
    {
        if (lex_forward == LEX_BUF_HALF_SIZE)
        {
		if (!pass_left) {
            		lex_load_right();
			
		} else {
			pass_left = 0;
		}
            lex_forward++;
        }
        else if (lex_forward == LEX_BUF_ALLOCATED-1)
        {
		if (!pass_right) {
            		lex_load_left();
	    		
		}else {
			pass_right = 0;
		}
            lex_forward = 0;
        }
        else
        {
	    lex_forward++;
            return EOF;
        }
    }
    // 先取出当前指针指向字符，再将指针前移
    return lex_buff[lex_forward++];
}

token nexttoken()
{
    int c;
    state = 0;	// 初始化状态为0
    start = 0;
    while(1)
    {
        switch(state)
        {
        	case 0:
                c = nextchar();
                if (c == blank || c == tab) {
                    state = 0;
                    set_lexeme_begin();	// 设置缓冲区指针到下一个token起始处
                }
                else if (c == cr) {
                    state = 1;	// 回车符
                }
                else if (c == newline) {
                    state = 2;	// 换行符
                }
                else if (c == '<') 
                    state = 3;
                else if (c == '=')
                    state = 7;
                else if (c == '>')
                    state = 8;
		else if (c == EOF)	// 读入文件结束
			return DONE;
                else {
			state = fail();
		}
                break;
           case 1:
                c = nextchar();
                if (c == newline)
                {
                    state = 2;
                }
                break;
           case 2:
                lineno++;
                set_lexeme_begin();
                state = 0;
                break;
           case 3:
                c = nextchar();
                if (c == '=')
                {
                    state = 4;
                }
                else if (c == '>')
                {
                    state = 5;
                }
                else
                {
                    state = 6;
                }
                break;
            	case 4:
			lexical_value = LE;	// 记号值，即token的属性
			set_lexeme_begin();	// 跳到下一个记号起始位置
			return RELOP;		// 识别出关系运算符
                break;
            case 5:
                lexical_value = NE;
                set_lexeme_begin();
                return RELOP;
                break;
            case 6:
                lexical_value = LT;
                retract(1);
                set_lexeme_begin();
                return RELOP;
                break;
            case 7:
                lexical_value = EQ;
				set_lexeme_begin();
                return RELOP;
                break;
            case 8:
                c = nextchar();
                if (c == '=')
                    state = 9;
                else 
                    state = 10;
                break;
            case 9:
                lexical_value = GE;
                set_lexeme_begin();
                return RELOP;
                break;
            case 10:
                lexical_value = GT;
		retract(1);
		set_lexeme_begin();
                return RELOP;
                break;
            case 11:
                c = nextchar();
				if (isalpha(c)) {
                    state = 12;
				}
				else {
				state = fail();
				}
                break;
            case 12:
                c = nextchar();
                if (isalnum(c))
                    state = 12;
                else
                    state = 13;
                break;
            case 13:		// 识别出标识符
                retract(1);
		lexical_token = ID;
                install_id();   // 填充到符号表，并返回表指针
				set_lexeme_begin();
                return ID;
                break;
            case 14:
                c = nextchar();
				
                if (isdigit(c))
                    state = 15;
		else  {
                    state = fail();	// 下一个转移图
		}
                break;
            case 15:
                c = nextchar();
				
                if (isdigit(c))
                    state = 15;
                else if (c == '.')
                    state = 16;
                else if (c == 'E')
					state = 18;
                else {
					if (crossbuf) {		// 测试输入"  2.354336>="
						lex_restore();	// 恢复左缓冲区为"  2."
						pass_left = 1;	// 解析用科学表示法表示的实数失败，等下不再加载左缓冲区
						//cout<<"buffer: "<<endl;
						//lex_buffer_dump();
					}
					start = 22;
					//printf("xdxd%c\n", c);
                    state = fail();
					//printf("state%d\n", state);
				}
                break;
            case 16:
                c = nextchar();
				
                if (isdigit(c))
                    state = 17;
                break;
            case 17:
                c = nextchar();
				
                if (isdigit(c))
                    state = 17;
                else if (c == 'E')
                    state = 18;
                else {
		    if (crossbuf) {		// 测试输入"  2.354336>="
			lex_restore();	// 恢复左缓冲区为"  2."
			pass_left = 1;	// 解析用科学表示法表示的实数失败，等下不再加载左缓冲区
			//cout<<"buffer: "<<endl;
			//lex_buffer_dump();
		    }
                    state = fail();
		}
                break;
            case 18:
                c = nextchar();
                if (c == '+' || c == '-')
                    state = 19;
                else if (isdigit(c))
                    state = 20;
                break;
            case 19:
                c = nextchar();
                if (isdigit(c))
                    state = 20;
                break;
            case 20:
                c = nextchar();
                if (isdigit(c))
                    state = 20;
                else {
                    state = 21;
		}
                break;
            case 21:
                retract(1);
                install_num();
		set_lexeme_begin();
                return NUM;
                break;
		case 22:	// 普通实数
			c = nextchar();
			
			if (isdigit(c))
				state = 23;
			else  {
                    state = fail();	// 下一个转移图

			}
			break;
		case 23:	
			c = nextchar();
			
			if (isdigit(c))
				state = 23;
			else if (c == '.')
				state = 24;
			else {
				if (crossbuf) {		// 测试输入"  123456>="
						lex_restore();	// 恢复左缓冲区为"  2."
						pass_left = 1;	// 解析用科学表示法表示的实数失败，等下不再加载左缓冲区
						//lex_buffer_dump();
					}

                    state = fail();
			}
			break;
		case 24:
			
			c = nextchar();
			
			if (isdigit(c))
				state = 25;
			break;
		case 25:
			c = nextchar();
			if (isdigit(c))
				state = 25;
			else
				state = 26;
			break;
		case 26:
			retract(1);
			install_num();
			set_lexeme_begin();
			return NUM;
			break;
		case 27:	// 整数
			c = nextchar();
			
			if (isdigit(c))
				state = 28;
			else 
				state = fail();
			break;
		case 28:
			c = nextchar();
			if (isdigit(c))
				state = 28;
			else
				state = 29;
			break;
		case 29:
			retract(1);
			set_lexeme_begin();
			install_num();
			return NUM;
			break;
        }
    }
}

void install_id()
{
	int index;
	char id_lexeme[100];
    	char lex_temp[111];
    	int i = 0;
		int j = 0;
    	int lexeme_begin_temp = lexeme_beginning;
		if (crossbuf) 
		{
			while (lex_temp[i++] = left_buf[j++]) {};
			i--;
			lexeme_begin_temp = (LEX_BUF_ALLOCATED/2);
			crossbuf = 0;
		}

    while ((lexeme_begin_temp%LEX_BUF_ALLOCATED) != (lex_forward))
    {
 	if (lex_buff[lexeme_begin_temp] == EOF) {
		 if (lexeme_begin_temp == LEX_BUF_HALF_SIZE)
	        {
	            lexeme_begin_temp++;
	        }
	        else if (lexeme_begin_temp == LEX_BUF_ALLOCATED-1)
	        {
	            lexeme_begin_temp = 0;
	        }
		continue;
	}
        lex_temp[i++] = lex_buff[lexeme_begin_temp++];
    }

	lex_temp[i]= '\0';

	strcpy(id_lexeme, lex_temp);
    	if (!(index = sym_lookup(id_lexeme)))
    	{
        	index = sym_insert(id_lexeme, lexical_token);
    	}
    	lexical_value = index;
}

void install_num()
{
    char lex_temp[111];
    int i = 0;
	int j = 0;
    int lexeme_begin_temp = lexeme_beginning;
	
	if (crossbuf) 
		{
			while (lex_temp[i++] = left_buf[j++]) {};
			i--;
			lexeme_begin_temp = (LEX_BUF_ALLOCATED/2);
			cout<<"end crossbuf"<<endl;
			crossbuf = 0;
			pass_left = pass_right = 0;
		}


    while ((lexeme_begin_temp%LEX_BUF_ALLOCATED) != (lex_forward))
    {
 	if (lex_buff[lexeme_begin_temp] == EOF) {
		 if (lexeme_begin_temp == LEX_BUF_HALF_SIZE)
	        {
	            lexeme_begin_temp++;
	        }
	        else if (lexeme_begin_temp == LEX_BUF_ALLOCATED-1)
	        {
	            lexeme_begin_temp = 0;
	        }
		else {
			break;
		}
		continue;
	}
        lex_temp[i++] = lex_buff[lexeme_begin_temp++];
    }
	lex_temp[i]= '\0';
	printf("num: %s\n", lex_temp);
	lexical_value = 3;

	i = 0;
    lex_load_temp_buf();
    while(lex_temp_buf[i])
    {
        lex_buff[i] = lex_temp_buf[i];
		i++;
    }
	//lex_load_right();
	pass_left = pass_right = 0;
}

/**
 * 让词素指针跳到下一个记号起始位置
 */
void set_lexeme_begin()
{
    lexeme_beginning = lex_forward;
}

void retract(int i)
{
    while (i--) {
        lex_forward--;
        if (lex_forward < 0)
            lex_forward = LEX_BUF_SIZE;
        else if (lex_forward == LEX_BUF_HALF_SIZE)
            lex_forward--;
    }
}
 
int fail()
{
	int left_cursor = LEX_BUF_HALF_SIZE;
	if (lexeme_beginning < left_cursor && lex_forward > left_cursor)
	{
		pass_left = 1;
	} else {
		if (lex_forward < lexeme_beginning)
		{
			pass_right = 1;
		}
		if (lexeme_beginning > left_cursor)
		{
			pass_left = 1;
		}
	}
	if (lexeme_beginning == LEX_BUF_HALF_SIZE) {
		lexeme_beginning++;
	} else if (lexeme_beginning == LEX_BUF_ALLOCATED-1) {
		lexeme_beginning = 0;
	}
    lex_forward = lexeme_beginning;
	//reload = 1;
    switch(start)
    {
        case 0: 
            start = 11;
            break;
        case 11:
            start = 14;
            break;
		case 14:
			start = 22;
			break;
		case 22:
			start = 27;
			break;
		case 27:
			// recover();
			// copy_left_buf_dump();
			FatalError("词法分析出错：未知词法元素！");
			break;
		default:	// 编译错误
			break;
    }
    return start;
}

void lex_restore()
{
	int i =0;
	int lexeme_temp = lexeme_beginning;
	lex_setbuff(lex_buff);	// 把左部分缓冲区保存到输入临时存放区
	while(left_buf[i]) {
		lex_buff[lexeme_temp++] = left_buf[i];
		i++;
	}
	
	//lex_buf_dump();

}

/**
 * 测试函数定义
 */

void lex_buf_dump()
{
    ArrayDump(lex_buff, LEX_BUF_ALLOCATED);
}

void nextchar_dump(int index)
{
	int i = 0;
    char nextch;
	for(i = 0; i < index; i++)
	{
		nextch = nextchar();
		if (isZeroChar(nextch))
			cout<<"zero char"<<endl;
		else if(isCRChar(nextch))
			cout<<"cr char"<<endl;
		else if(isLineChar(nextch))
			cout<<"new line"<<endl;
		else if(isEofChar(nextch))
			cout<<"end of file"<<endl;
		else 
			printf("char: %c\n", nextch);
	}
}

void lex_eof_test()
{
    if (lex_buff[3] == EOF)
        cout<<"缓冲区的左半部分分界符为End of file"<<endl;
    if (lex_buff[7] == EOF)
        cout<<"缓冲区的右半部分分界符为End of file"<<endl;
}

void copy_left_buf_dump()
{
	ArrayDump(left_buf, LEX_BUF_HALF_SIZE);
}

void pointer_dump()
{
	cout<<"forward: "<<lex_forward<<endl;
	cout<<"begin: "<<lexeme_beginning<<endl;
}
