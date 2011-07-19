#include "common.h"
#include "global.h"

void ArrayOutput(char* str)
{
	int index = 0;
	int temp;
	while (temp = str[index])
	{
		printf("%c\t", temp);
		index++;
	}
	// cout<<"total count: "<<index<<endl;
	// cout<<"first char: "<<str[0]<<endl;
	// cout<<"isZeroChar: "<<isEofChar(str[0])<<endl;
}

int isZeroChar(char c)
{
	return (c == '\0');
}

int isLineChar(char c)
{
	return (c == '\n');

}

int isEofChar(char c)
{
	return (c == EOF);
}
