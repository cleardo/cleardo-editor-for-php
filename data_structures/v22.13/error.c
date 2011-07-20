#include "common.h"

/**
 * 程序错误
 */
void Error(char* ErrorMsg)
{
	printf("%s", ErrorMsg);
	exit(1);
} 

void FatalError(char* ErrorMsg)
{
	printf("%s", ErrorMsg);
	exit(2);
}
