#include "common.h"
#include "global.h"
#include "lexer.h"
#include "symbol.h"
#include "buffer.h"
#include "production.h"

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
int crossrightbuf = 0;
char left_buf[LEX_BUF_HALF_SIZE+1];
char right_buf[LEX_BUF_HALF_SIZE+1];
int numchar_pointer = 0;
char* lexeme_str;

void lex_init_input(char* filename)
{
    filename = "t.dl";
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

int copy_right_buf()
{
	int i = 0;
	int lexeme_temp_pointer = lexeme_beginning;
	while(lexeme_temp_pointer != LEX_BUF_ALLOCATED - 1)
	{
		right_buf[i++] = lex_buff[lexeme_temp_pointer++];
	}
	right_buf[i] = '\0';
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
    if (lexeme_beginning > LEX_BUF_HALF_SIZE) {
		crossrightbuf = copy_right_buf();
	}
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
			cout<<"noright"<<endl;
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
		} else {
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
		case 30:
		    c = nextchar(); // 取得下一个字符输入，因为前面如果多读入一个字符，回有回退指针动作，所以可放心读入下一个字符
		    if (c == '%') {
			state = 31;
		    } else {
			state = fail();
		    }
		    break;
		case 31:
		    c = nextchar();
		    if (c == '{') {
			state = 32;
		    } else if (c == '}') {
			state = 33;
		    } else if (c == '%') {
			state = 34;
		    } else if (c == 't') {
			state = 36;
		    }
		    break;
		case 32:
		    set_lexeme_begin();
		    return PDT_DECL_BEGIN;
		    break;
		case 33:
		    set_lexeme_begin();
		    return PDT_DECL_END;
		    break;
		case 34:
		    set_lexeme_begin();
		    return PDT_DELIMIT;
		    break;
		case 36:
		    c = nextchar();
		    if (c == 'o') {
			state = 37;
		    }
		    break;
		case 37:
		    c = nextchar();
		    if (c == 'k') {
			state = 38;
		    }
		    break;
		case 38:
		    c = nextchar();
		    if (c == 'e') {
			state = 39;
		    }
		    break;
		case 39:
		    c = nextchar();
		    if (c == 'n') {
			state = 40;
		    }
		    break;
		case 40:
		    set_lexeme_begin();
		    return PDT_NEWTERMINAL;
		    break;
		case 41:
		    c = nextchar();
		    if (c == '\'') {
			state = 42;
		    } else {
			state = fail();
		    }
		    break;
		case 42:
		    c = nextchar();
		    if (c != '\'') {
			state = 42;
		    } else {
			state = 43;
		    }
		    break;
		case 43:
		    lexical_token = PDT_LITERAL;
		    install_literal_tsym();   // 填充到符号表，并返回表指针
		    set_lexeme_begin();
		    return PDT_LITERAL;
		    break;
		case 44:
		    c = nextchar();
		    if (c == ':')
		    {
			state = 45;
		    } else {
			state = fail();
		    }
		    break;
		case 45:
		    set_lexeme_begin();
		    return PDT_DERIV;
		    break;
		case 46:
		    c = nextchar();
		    if (c == ';')
		    {
			state = 47;
		    } else {
			state = fail();
		    }
		    break;
		case 47:
		    lexical_token = PDT_EXPR_END;
		    set_lexeme_begin();
		    return PDT_EXPR_END;
		    break;
		case 48:
		    c = nextchar();
		    if (c == '|')
		    {
			state = 49;
		    } else {
			state = fail();
		    }
		    break;
		case 49:
		    lexical_token = PDT_ALTERNATE;
		    set_lexeme_begin();
		    return PDT_ALTERNATE;
		    break;
		case 50:
		    c = nextchar();
		    if ( c == '#')
		    {
			state = 51;
		    }
		    else 
		    {
			state = fail();
		    }
		    break;
		case 51:
		    lexical_token = PDT_EMPTY;
		    install_empty();
		    set_lexeme_begin();
		    return PDT_EMPTY;
		    break;
        }
    }
}

/**
 * 识别出ID的操作，插入到符号表，并返回索引
 */
void install_id()
{
	int index;		// 符号表索引
	char id_lexeme[100];	// 词素字符串值
    	char lex_temp[111];	// 临时存储字符串值
    	int i = 0;
	int j = 0;
    	int lexeme_begin_temp = lexeme_beginning;
	if (crossbuf)	// 覆盖了前半部分缓冲区字符，进行特殊处理
	{	
	    while (lex_temp[i++] = left_buf[j++]) {};
	    i--;
	    lexeme_begin_temp = (LEX_BUF_ALLOCATED/2);
	    //cout<<"end crossbuf"<<endl;
	    crossbuf = 0;
	    pass_left = pass_right = 0;
	}

	if (crossrightbuf)	// 覆盖了前半部分缓冲区字符，进行特殊处理
	{	
	    while (lex_temp[i++] = right_buf[j++]) {};
	    i--;
	    lexeme_begin_temp = 0;
	    // cout<<"end right crossbuf"<<endl;
	    crossrightbuf = 0;
	    pass_left = pass_right = 0;
	}

	while ((lexeme_begin_temp%LEX_BUF_ALLOCATED) != (lex_forward))
	{
	    if (lex_buff[lexeme_begin_temp] == EOF) 
	    {
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

void install_empty()
{
    char lex_temp[2];
    int index;
    lex_temp[0] = '#';
    lex_temp[1] = '\0';
    if (!(index = sym_lookup(lex_temp)))
    {
	index = sym_insert(lex_temp, PDT_EMPTY);
    }
    lexical_value = index;
    int tsym_index;
    if (!(tsym_index = pdt_terminalLookup(index))) {
	pdt_insertNewTerminal(index);
    }
}

void install_literal_tsym()
{
	int index;		// 符号表索引
	char id_lexeme[100];	// 词素字符串值
    	char lex_temp[111];	// 临时存储字符串值
    	int i = 0;
	int j = 1;
    	int lexeme_begin_temp = lexeme_beginning + 1;
	if (crossbuf)	// 覆盖了前半部分缓冲区字符，进行特殊处理
	{	
	    while (lex_temp[i++] = left_buf[j++]) {};
	    i--;
	    lexeme_begin_temp = (LEX_BUF_ALLOCATED/2);
	    //cout<<"end crossbuf"<<endl;
	    crossbuf = 0;
	    pass_left = pass_right = 0;
	}

	if (crossrightbuf)	// 覆盖了前半部分缓冲区字符，进行特殊处理
	{	
	    while (lex_temp[i++] = right_buf[j++]) {};
	    i--;
	    lexeme_begin_temp = 0;
	    // cout<<"end right crossbuf"<<endl;
	    crossrightbuf = 0;
	    pass_left = pass_right = 0;
	}

	while ((lexeme_begin_temp%LEX_BUF_ALLOCATED) != (lex_forward))
	{
	    if (lex_buff[lexeme_begin_temp] == EOF) 
	    {
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

	lex_temp[i-1]= '\0';

	strcpy(id_lexeme, lex_temp);
    	if (!(index = sym_lookup(id_lexeme)))
    	{
        	index = sym_insert(id_lexeme, lexical_token);
    	}
    	lexical_value = index;
	int tsym_index;
	if (!(tsym_index = pdt_terminalLookup(index))) {
	    pdt_insertNewTerminal(index);
	}
}

/**
 * 取得当前识别词素的字符串值
 */
void lex_get_lexeme(char* lexeme_str)
{
    char id_lexeme[100];	// 词素字符串值
    char lex_temp[111];	// 临时存储字符串值
    int i = 0;
    int j = 0;
	    int lexeme_begin_temp = lexeme_beginning;
    if (crossbuf)	// 覆盖了前半部分缓冲区字符，进行特殊处理
    {	
	while (lex_temp[i++] = left_buf[j++]) {};
	i--;
	lexeme_begin_temp = (LEX_BUF_ALLOCATED/2);
	crossbuf = 0;
    }

    
	if (crossrightbuf)	// 覆盖了前半部分缓冲区字符，进行特殊处理
	{	
	    while (lex_temp[i++] = right_buf[j++]) {};
	    i--;
	    lexeme_begin_temp = 0;
	    //cout<<"end right crossbuf"<<endl;
	    crossrightbuf = 0;
	}

    while ((lexeme_begin_temp%LEX_BUF_ALLOCATED) != (lex_forward))
    {
	if (lex_buff[lexeme_begin_temp] == EOF) 
	{
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
    printf("%s\t", lex_temp);
    strcpy(lexeme_str, lex_temp);
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
			//cout<<"end crossbuf"<<endl;
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
    crossbuf = crossrightbuf = 0;	// 识别记号成功，不用因为怕覆盖缓冲区而进行跨界缓存，故将标记清0
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
		// forward跨过缓冲区边界后失败并回退，不加载右缓冲区
		pass_left = 1;
		pass_right = 0;
	} else if (lexeme_beginning > left_cursor && lex_forward < left_cursor) {
		pass_right = 1;
		pass_left = 0;
	    /*
		if (lex_forward < lexeme_beginning && lexeme_beginning < left_cursor)
		{
			pass_right = 1;
		}
		if (lexeme_beginning > left_cursor && lex_forward < left_cursor)
		{
		    cout<<"failleft"<<endl;
			pass_left = 1;
		}
	    */
	} else if (lexeme_beginning < left_cursor && lex_forward < lexeme_beginning) {
		pass_right = 1;
		pass_left = 1;
	} else if (lexeme_beginning > left_cursor && lex_forward < lexeme_beginning) {
		pass_right = 1;
		pass_left = 1;
	} else {
		pass_right = 0;
		pass_left = 0;
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
			start = 30;	// 注意：这里要对start进行设置，而不是state
			break;
		case 30:
			start = 41;
			break;
		case 41:
			start = 44;
			break;
		case 44:
			start = 46;
			break;
		case 46:
			start = 48;
			break;
		case 48:
			start = 50;
			break;
		case 50:
			// recover();
			// copy_left_buf_dump();
			lex_buf_dump();
			pointer_dump();
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

char lex_curchar()
{
    return lex_buff[(lex_forward-1)%(LEX_BUF_ALLOCATED-1)];
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
