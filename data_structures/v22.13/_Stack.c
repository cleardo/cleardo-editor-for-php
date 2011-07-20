#include "common.h"
#include "_Stack.h"

struct Node
{
	ElementType Element;
	PtrToNode Next;
};

int IsEmpty( Stack S )
{
	return S->Next == NULL;
}

Stack CreateStack( void )
{
	Stack S;

	S = malloc( sizeof ( struct Node ) );
	if( S == NULL )
		FatalError( "Out of space!!!" );
	S->Next = NULL;
	MakeEmpty( S );
	return S;
}

void MakeEmpty( Stack S )
{
	if( S == NULL )
		Error( "Must use CreateStack First" );
	else
		while( !IsEmpty( S ) )
			Pop( S );
}

void Pop( Stack S )
{
	PtrToNode TempNode;

	if( IsEmpty( S ) )
		Error( "Empty Stack" );
	else
	{
		TempNode = S->Next;
		S->Next = S->Next->Next;
		free( TempNode );
	}
}

void Push( ElementType X, Stack S )
{
	PtrToNode TempNode;

	TempNode = malloc( sizeof ( struct Node ) );
	if( TempNode == NULL )
		Error( "Out of space !!!" );
	else
	{
		TempNode->Element = X;
		TempNode->Next = S->Next;
		S->Next = TempNode;
	}
}

ElementType Top( Stack S )
{
	if( !IsEmpty( S ) )
		return S->Next->Element;
	else
		Error( "Empty Stack" );
	return 0;
}

