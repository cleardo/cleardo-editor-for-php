#include "common.h"
#include <string>

/**
 * 程序错误
 */
void Error(string ErrorMsg)
{
	cout<<ErrorMsg<<endl;
	exit(1);
} 

void FatalError(string ErrorMsg)
{
	cout<<ErrorMsg<<endl;
	exit(2);
}
