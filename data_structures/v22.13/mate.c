#include <stdio.h>
typedef int ElementType;
#include "_Stack.h"

#define SMALL_LEFT ( 40 )
#define SMALL_RIGHT ( 41 )
#define MEDIUM_LEFT ( 91 )
#define MEDIUM_RIGHT ( 93 )
#define BIG_LEFT ( 123 )
#define BIG_RIGHT ( 125 )

void PrintError( int line, int column, int type );
void PrintError( int line, int column, int type )
{
	if( type == 1 )
		printf( "Unexpected start on line %d, char %d", line, column );
	if( type == 2 )
		printf( "Unexpected end on line %d, char %d", line, column );
}

int main()
{
    Stack S;
	FILE *fp;
	int line = 1, column = 0;
	ElementType c, top;

	if( ( fp = fopen( "peidui.txt", "r" ) ) == NULL )
	{
		printf( "Cannot open this file.\n" );
		return 0;
	}

	S = CreateStack();

	while( !feof ( fp ) )
	{
		if( c == '\n')
		{
			++line;
			column = 0;
		}
		else
			++column;	
		c = fgetc( fp );
		if( c == '\n' )
			++line;
		if( c == SMALL_LEFT || c == MEDIUM_LEFT || c == BIG_LEFT )
		{
			Push( c, S );
		}
		top = Top(S);
		printf("%c\n", top);

		if( c == SMALL_RIGHT )
		{
			if( IsEmpty( S ) )
				PrintError( line, column, 1 );
			else
			{
				top = Top( S );
				if( top != SMALL_LEFT )
				{
					PrintError( line, column, 2 );
					return 0;
				}
			}
		}

		if( c == MEDIUM_RIGHT )
		{
			if( IsEmpty( S ) )
				PrintError( line, column, 1 );
			else
			{
				top = Top( S );
				if( top != MEDIUM_LEFT )
				{
					PrintError( line, column, 2 );
					return 0;
				}
			}
		}

		if( c == BIG_RIGHT )
		{
			if( IsEmpty( S ) )
				PrintError( line, column, 1 );
			else
			{
				top = Top( S );
				if( top != BIG_LEFT )
				{
					PrintError( line, column, 2 );
					return 0;
				}
			}
		}
	}

	if( !IsEmpty( S ) )
		PrintError( line, column, 2 );
	return 0;

}
