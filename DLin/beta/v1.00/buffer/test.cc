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
	cout<<endl;
}

void ArrayDump(char* str, int index)
{
	int i = 0;
	for(i = 0; i < index; i++)
	{
		if (isZeroChar(str[i]))
			cout<<"zero char"<<endl;
		else if(isCRChar(str[i]))
			cout<<"cr char"<<endl;
		else if(isLineChar(str[i]))
			cout<<"new line"<<endl;
		else if(isEofChar(str[i]))
			cout<<"end of file"<<endl;
		else 
			printf("char: %c\n", str[i]);
	}
}

int isZeroChar(char c)
{
	return (c == '\0');
}

int isCRChar(char c)
{
	return (c == '\r');

}

int isLineChar(char c)
{
	return (c == '\n');

}

int isEofChar(char c)
{
	return (c == EOF);
}

int getArrayLength(char* str)
{
	int length = 0;
	while (str[length] != '\0')
		length++;
	return length;
}
