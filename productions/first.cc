#include "common.h"
#include "global.h"

void FIRST2(pointer p, FSList fsl)
{
    if (p->terminal == 1)
    {
        if (!LocateFSL(fsl, p->sym.tsym))
        {
            InsertFSL(fsl, p->sym.tsym);
        }
    }
    else if (p->terminal == 0)
    {
        FSList fsl2;    // 声明一个新的First集指针
        if (p->sym.nsym->sym == 'A')	// 是否已经求过First集
        {
            CreateFSL(fsl2);
            InsertFSL(fsl2, empty);
            p->sym.nsym->fset = fsl2;
        }
        else
        {
			fsl2 = FIRST(p->sym.nsym);
        }
        MergeFSL(fsl, fsl2);
        if (hasNullFirst(p->sym.nsym) && p->suc)
        {
            p = p->suc;
            FIRST2(p, fsl);
        }
        if (hasNullFirst(p->sym.nsym) && !p->suc)
        {
            if (!LocateFSL(fsl, empty))
            {
                InsertFSL(fsl, empty);
            }
        }
    }
    return;
}

FSList FIRST(hpointer h)
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

int hasNullFirst(hpointer h)
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

int hasNullFirst_Str(FSList FSNode)
{
    PtrToFSnode Node;
    Node = FSNode->Next;
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

FSList FIRST_Str(pointer p)
{
    FSList fsl;
    CreateFSL(fsl);
    if (p->terminal == 1)
    {
        InsertFSL(fsl, p->sym.tsym);
    }
    else if (p->terminal == 0)
    {
        FSList fsl2;    // 声明一个新的First集指针
        fsl2 = FIRST(p->sym.nsym);
        MergeFSL(fsl, fsl2);
        if (hasNullFirst(p->sym.nsym) && p->suc)
        {
            p = p->suc;
            FIRST2(p, fsl);
        }
        if (hasNullFirst(p->sym.nsym) && !p->suc)
        {
            if (!LocateFSL(fsl, empty))
            {
                InsertFSL(fsl, empty);
            }
        }
    }
    return fsl;
}

void PrintFSL(FSList fsl)
{
    PtrToFSnode Node;
    Node = fsl->Next;
    cout<<"The First List: ";
    while(Node)
    {
        printf("%c ", Node->sym);
	Node = Node->Next;
    }
    cout<<endl;
}

void CreateFSL(FSList &fsl)
{
    PtrToFSnode Node;
    Node = (PtrToFSnode)malloc(sizeof(FSNode));
    Node->sym = 'A';
    Node->Next = NULL;
    fsl = Node;
}

void InsertFSL(FSList fsl, char ch)
{
    PtrToFSnode Node;
    Node = (PtrToFSnode)malloc(sizeof(FSNode));
    Node->sym = ch;
    Node->Next = fsl->Next;
    fsl->Next = Node;
}

int LocateFSL(FSList fsl, char ch)
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
