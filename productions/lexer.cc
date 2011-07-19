#include "common.h"
#include "global.h"

/**
 * 读入一个字符到ch全局变量
 */
void getch(void)
{
	char ch_read;
	if (cc == ll)
	{
		ll = cc = 0;
		while ( (ll < BUFFER_SIZE - 1) && ((ch_read = fgetc(infile)) != EOF) )  
		{
			line[ll++] = ch_read;
			// cout<<ll<<endl;
		}
		if (ch_read == EOF) {
			line[ll++] = EOF;
		}
		line[ll] = '\0';
		// ArrayOutput(line);
	}
	ch = line[cc++];
}

/**
 * 词法分析器
 */
int getsym()
{
	do {
		getch();
		sym = ch;
		if (sym == EOF) {
			cout<<"源文件读入完毕!"<<endl;
			return 256;
		}
		// putchar(sym);
	} while (sym == ' ' || sym == '\n');
}
