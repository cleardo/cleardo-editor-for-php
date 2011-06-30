#include "common.h"
#include "global.h"
#include "lexer.h"

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
int lineno = 1;

void lex_init_input(char* filename)
{
    if ((prog_fp = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "无法打开程序文件：%s", filename);
    }
    else
    {
        /*
        while(!feof(prog_fp))
        {
            char_read = fgetc(prog_fp);
            if (char_read == '\n') {
                cout<<"new line\t";
            } else if (char_read == EOF) {
                cout<<"END OF FILE\t";
            } else {
                cout<<char_read<<'\t';    
            }
        }   
        */
        lex_buff[LEX_BUF_HALF_SIZE] = lex_buff[LEX_BUF_ALLOCATED-1] = EOF;
        // 加载一定字符到缓冲区左半部分
        lex_load_left();
    }
}

void lex_load_temp_buf()
{
	int read_count = 0;
    read_count = fread(lex_temp_buf, sizeof(char), LEX_BUF_HALF_SIZE, prog_fp);
    // cout<<"read_count: "<<read_count<<endl;
    if (read_count < LEX_BUF_HALF_SIZE)
    {
        lex_temp_buf[read_count] = EOF;
        read_count++;
    }
    lex_temp_buf[read_count] = '\0';
}

void lex_load_left()
{
    int i = 0;
    lex_load_temp_buf();
    while(lex_temp_buf[i])
    {
        lex_buff[i] = lex_temp_buf[i];
		i++;
    }
    // cout<<"is eof?"<<lex_temp_buf[0]<<'\t'<<isEofChar(lex_temp_buf[0])<<endl;
    // ArrayDump(lex_temp_buf, LEX_BUF_ALLOCATED/2);
    /*
    cout<<getArrayLength(lex_temp_buf);
	if ('0' != '\0')
    		cout<<"length: "<<i<<endl;
    */
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
    if (lex_buff[lex_forward] == EOF)
    {
        if (lex_forward == LEX_BUF_HALF_SIZE)
        {
            lex_load_right();
            lex_forward++;
        }
        else if (lex_forward == LEX_BUF_ALLOCATED-1)
        {
            lex_load_left();
            lex_forward = 0;
        }
        else
        {
	    lex_forward++;
            return EOF;
        }
    }
    return lex_buff[lex_forward++];
}

token nexttoken()
{
    int c;
    state = 0;
    while(1)
    {
        switch(state)
        {
            case 0:
                c = nextchar();
                if (c == blank || c == tab) {
                    state = 0;
                    set_lexeme_begin();
                }
                else if (c == cr) {
                    state = 1;
                }
                else if (c == newline) {
                    state = 2;
                }
                else if (c == '<') 
                    state = 3;
                else if (c == '=')
                    state = 7;
                else if (c == '>')
                    state = 8;
		else if (c == EOF)
			return DONE;
                else state = fail();
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
                lexical_value = LE;
                printf("1%c\n", lex_buff[lexeme_beginning]);
                set_lexeme_begin();
                printf("2%c\n", lex_buff[lexeme_beginning]);
                return RELOP;
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
                if (isalpha(c))
                    state = 12;
                else 
                    state = fail();
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
                install_id();   // 填充到符号表，并返回表指针
                return ID;
                break;
            case 14:
                c = nextchar();
                if (isdigit(c))
                    state = 15;
		else 
                    state = fail();	// 下一个转移图
                break;
            case 15:
                c = nextchar();
                if (isdigit(c))
                    state = 15;
                else if (c == '.')
                    state = 16;
                else if (c == 'E')
		            state = 18;
                else 
                    state = 23;
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
                else 
                    state = 22;
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
			printf("bbbb%d", lex_forward);
                    state = 21;
		}
                break;
            case 21:
printf("bbbb%d", lex_forward);
cout<<"begin21: "<<lexeme_beginning<<"end: "<<lex_forward<<endl;
                retract(1);
cout<<"begin21: "<<lexeme_beginning<<"end: "<<lex_forward<<endl;
                install_num();
	printf("kkkkk%c", nextchar());
                return NUM;
                break;
	case 22:
		retract(1);
                install_num();
                return NUM;
		break;
	case 23:
	cout<<"========"<<endl;
	ArrayDump(lex_buff, 8);
cout<<"========"<<endl;
	cout<<"begin21: "<<lexeme_beginning<<"end: "<<lex_forward<<endl;
                retract(1);
cout<<"begin21: "<<lexeme_beginning<<"end: "<<lex_forward<<endl;
                install_num();
                return NUM;
		break;
        }
    }
}

void install_id()
{
/*
    if (!index = sym_lookup(id_lexeme))
    {
        index = sym_insert(id_lexeme);
    }
    lexical_value = index;
*/
    	cout<<"begin: "<<lexeme_beginning<<"end: "<<lex_forward<<endl;
    char lex_temp[111];
    int i = 0;
    int lexeme_begin_temp = lexeme_beginning;
    while ((lexeme_begin_temp%8) != (lex_forward+1))
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
	printf("i%d\n",i);
    }
	lex_temp[i]= '\0';
    	printf("aaa%s\n", lex_temp);
	lexical_value = 2;	// ID符号表索引
}

void install_num()
{
	cout<<"begin: "<<lexeme_beginning<<"end: "<<lex_forward<<endl;
    char lex_temp[111];
    int i = 0;
    int lexeme_begin_temp = lexeme_beginning;
    while ((lexeme_begin_temp%8) != (lex_forward+1))
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
	printf("i%d\n",i);
    }
	lex_temp[i]= '\0';
    	printf("aaa%s\n", lex_temp);
	lexical_value = 3;
}

void set_lexeme_begin()
{
    cout<<"setting: "<<lex_forward<<endl;
    lexeme_beginning = lex_forward;
    /*
    if (lexeme_beginning == LEX_BUF_HALF_SIZE)
    {
        lexeme_beginning++;
    }
    else if (lexeme_beginning == LEX_BUF_ALLOCATED - 1)
    {
        lex
    */
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
    lex_forward = lexeme_beginning;
    switch(start)
    {
        case 0: 
            start = 11;
            break;
        case 11:
            start = 14;
            break;
	case 14:
    }
    return start;
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
