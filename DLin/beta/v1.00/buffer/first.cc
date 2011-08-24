#include "common.h"
#include "global.h"
#include "production.h"
#include "first.h"

struct FSNode {
    int sym;
    Position Next;
};

/**
 * 打印First符号集元素
 */
void PrintFSL(FSList fsl)
{
    PtrToFSnode Node;	// 声明结点
    Node = fsl->Next;
    cout<<"The First List: ";
    while(Node)
    {
        printf("%d ", Node->sym);
	Node = Node->Next;
    }
    cout<<endl;
}

void CreateFSL(FSList &fsl)
{
    PtrToFSnode Node;
    Node = (PtrToFSnode)malloc(sizeof(FSNode));
    Node->sym = 0;
    Node->Next = NULL;
    fsl = Node;
}

void InsertFSL(FSList fsl, int ch)
{
    PtrToFSnode Node;
    Node = (PtrToFSnode)malloc(sizeof(FSNode));
    Node->sym = ch;
    Node->Next = fsl->Next;
    fsl->Next = Node;
}

int LocateFSL(FSList fsl, int ch)
{
    PtrToFSnode Node;
    Node = fsl->Next;
    while (Node && Node->sym != ch)
    {
        Node = Node->Next;
    }
    if (Node)
    {
        return 1;
    }
    else
    {   
        return 0;
    }
}

void MergeFSL(FSList fsl1, FSList fsl2)
{
    PtrToFSnode Node;
    Node = fsl2->Next;
    while (Node)
    {   
        if (!LocateFSL(fsl1, Node->sym) && Node->sym != empty)
        {
            InsertFSL(fsl1, Node->sym);
        }
        Node = Node->Next;
    }
}
/*
FSList FIRST(pdt_hpointer h)
{
    FSList fsl;     // 创建First集链表
    CreateFSL(fsl); // 初始化，并分配头结点空间
    pointer p;
    p = h->entry;
    while (p != NULL)
    {
        FIRST2(p, fsl);
        p = p->alt;
    }
    h->fset = fsl;
    return fsl;
}
*/
void FIRST2(pdt_pointer p, FSList fsl)
{
}
/*
int hasNullFirst(pdt_hpointer h)
{
    PtrToFSnode Node;
    Node = h->fset->Next;
    while(Node && Node->sym != empty)
    {
        Node = Node->Next;
    }       
    if (Node)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
*/