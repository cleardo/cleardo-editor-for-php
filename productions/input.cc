#include "common.h"
#include "global.h"

void input()
{
	char filename[20] = "p";
	if ( (infile = fopen(filename, "r")) == NULL)
	{
		FatalError("对不起，输入文件无法打开!");
		exit(1);
	}
	cc = ll = 0;
}
