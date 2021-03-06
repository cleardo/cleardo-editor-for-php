#include "common.h"
#include "global.h"
#include "production.h"
#include "first.h"

int empty_id;

struct pdt_node
{
    int terminal;	    // 是否终结符：1 - 终结符; 2 - 非终结符
    union {
	int tsym;	    // 终结符，用终结符表索引表示
	pdt_hpointer nsym;  // 非终结符，用非终结符头指针表示
    }sym;
    pdt_pointer suc;	    // 指向下一个语法产生式结点
    pdt_pointer alt;	    // 指向可选语法产生式结点
};

struct pdt_header
{
    char sym[200];
    pdt_pointer entry;	    // 指向语法产生式起始结点
    pdt_hpointer suc;	    // 指向可选语法产生式
};

struct FSNode {
    int sym;
    Position Next;
};

struct FSHeader {
	char nsym[200];
	pdt_hpointer h;
	int hasFS;
	int hasFO;
	FSList fs;
	FSList fo;
};

struct FSHeader fs_table[200];
int fs_lastentry = 0;

void fs_initFsTable()
{
	char empty_str[10] = "#";
	empty_id = pdt_isTerminal(empty_str);
	cout<<"empty_id: "<<empty_id<<endl;
	pdt_hpointer h = pdt_list;
	while (h != pdt_sentinel)
	{
		fs_lastentry++;
		strcpy(fs_table[fs_lastentry].nsym, h->sym);
		fs_table[fs_lastentry].h = h;
		CreateFSL(fs_table[fs_lastentry].fs);
		h = h->suc;
	}
}



int fs_lookup(char ch[])
{
	int p;
	for (p = fs_lastentry; p > 0; p--)
	{
		if (strcmp(fs_table[p].nsym, ch) == 0)
		{
			return p;
		}
	}
	return 0;
}

void fs_tableDump()
{
	fs_initFsTable();
	int p;
	for (p = fs_lastentry; p > 0; p--)
	{
		cout<<fs_table[p].nsym<<endl;
	}
}

void fs_listDump(FSList l)
{
	PtrToFSnode p;
	p = l->Next;
	while (p)
	{
		char* str;
		str = pdt_getTerminalSym(p->sym);
		cout<<str<<'\t';
		p = p->Next;
	}
}

void fs_tableDump2()
{
	int p;
	cout<<"fs_table and fs_list:"<<endl;
	for (p = fs_lastentry; p > 0; p--)
	{
		cout<<fs_table[p].nsym<<'\t';
		fs_listDump(fs_table[p].fs);
		cout<<endl;
	}
}


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
	    char* str;
		str = pdt_getTerminalSym(Node->sym);
		cout<<str<<'\t';
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
        if (!LocateFSL(fsl1, Node->sym) && !pdt_isEmpty(Node->sym))
        {
            InsertFSL(fsl1, Node->sym);
        }
        Node = Node->Next;
    }
}

FSList FIRST(pdt_hpointer h)
{
	int head;
	head = fs_lookup(h->sym);
    fs_table[head].hasFS = 1;
	
    FSList fsl;     // 创建First集链表
    CreateFSL(fsl); // 初始化，并分配头结点空间
    pdt_pointer p;
    p = h->entry;
    // cout<<"header: "<<h->sym<<endl;
    while (p != NULL)
    {
        FIRST2(p, fsl);
        p = p->alt;
    }
    fs_table[head].fs = fsl;
    return fsl;
}

void FIRST2(pdt_pointer p, FSList fsl)
{
	int head;
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
        if (0)	// 是否已经求过First集
        {
            CreateFSL(fsl2);
            InsertFSL(fsl2, empty_id);
    		head = fs_lookup(p->sym.nsym->sym);
    		fs_table[head].fs = fsl2;
        }
        else
        {
 		 head = fs_lookup(p->sym.nsym->sym);
    		if (!fs_table[head].hasFS)
		{
		       	fsl2 = FIRST(p->sym.nsym);
		}
		else
		{
    			fsl2 = fs_table[head].fs;
		}
			//cout<<"fsl2: "<<endl;
			//PrintFSL(fsl2);
        }
        MergeFSL(fsl, fsl2);
	//cout<<"after: merged"<<endl;
	//PrintFSL(fsl);
        if (hasNullFirst(p->sym.nsym) && p->suc)
        {
            p = p->suc;
            FIRST2(p, fsl);
        }
        if (hasNullFirst(p->sym.nsym) && !p->suc)
        {
            if (!LocateFSL(fsl, empty_id))
            {
                InsertFSL(fsl, empty_id);
            }
        }
    }
    return;
}

int hasNullFirst(pdt_hpointer h)
{
    PtrToFSnode Node;
    int head = fs_lookup(h->sym);
    Node = fs_table[head].fs->Next;
    while(Node && Node->sym != empty_id)
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
    while(Node && Node->sym != empty_id)
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

FSList FIRST_Str(pdt_pointer p)
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
            if (!LocateFSL(fsl, empty_id))
            {
                InsertFSL(fsl, empty_id);
            }
        }
    }
    return fsl;
}

FSList FOLLOW(pdt_hpointer h)
{
    FSList fsl;
	CreateFSL(fsl);
    pdt_hpointer HeadNode;
    pdt_pointer p;
    HeadNode = pdt_list;
    if (h != NULL)
    {
	cout<<h->sym<<"xxxx"<<endl;
	if (strcmp(h->sym, pdt_list->sym) == 0)
	{
		InsertFSL(fsl, 999);	// 起始符号
	}
    }
    while (HeadNode != pdt_sentinel)
    {
        p = HeadNode->entry;
        while (p) 
        {
            FOLLOW2(p, fsl, h->sym, HeadNode);
	    // cout<<p->terminal<<endl;
            p = p->alt;
		
        }
        HeadNode = HeadNode->suc;
    }
    return fsl;
}

void FOLLOW2(pdt_pointer p, FSList fsl, char* sym, pdt_hpointer h)
{
    while (p)
    {
        if (p && p->terminal == 0 && (strcmp(p->sym.nsym->sym, sym) == 0))
        {
            if (p->suc)
            {
                FSList fsl2;
                fsl2 = FIRST_Str(p->suc);
                MergeFSL(fsl, fsl2);
		PrintFSL(fsl);
				if (hasNullFirst_Str(fsl2))
				{
					fsl2 = FOLLOW(h);
					// cout<<h->sym<<endl;
					MergeFSL(fsl, fsl2);
				}
            }
            else
            {
                // p->suc为空
            }    
        }
        p = p->suc;
    }
    return;
}

void FOLLOW_all()
{
	pdt_hpointer h = pdt_list;
	FSList l;
	cout<<"follow all:"<<endl;
	while(h != pdt_sentinel)
	{
		l = FOLLOW(h);
		PrintFSL(l);
		h = h->suc;
	}
}

FSList FOLLOW3(pdt_hpointer h, FSList &fsl)
{
	int head = fs_lookup(h->sym);
		fs_table[head].hasFO = 1;
    pdt_pointer p, q, r;
    pdt_hpointer HeadNode;
    FSList fsl2;
    HeadNode = pdt_list;
    if (h != NULL)
    {
	if (strcmp(h->sym, pdt_list->sym) == 0)
	{
		InsertFSL(fsl, 1);	// 起始符号
	}
    }
    while (HeadNode != pdt_sentinel)
    {
        p = HeadNode->entry;
        while (p) 
        {
		q = p;
		int not_null_first = 0;
		int has_null_first = 0;
		while (q)
		{
			if (q->terminal == 0 && (strcmp(q->sym.nsym->sym, h->sym) == 0))
			{
				// cout<<"equal syms: "<<q->sym.nsym->sym<<endl;
				r = q->suc;
				if (r)
				{
					// 求要求的FOLLOW非终结符结点的后继结点
					fsl2 = FIRST_Str(r);
					MergeFSL(fsl, fsl2);
					if (hasNullFirst_Str(fsl2))
					{
						r = r->suc;
						has_null_first = 1;
						while (r && has_null_first)
						{
							fsl2 = FIRST_Str(r);
							MergeFSL(fsl, fsl2);
							has_null_first = hasNullFirst_Str(fsl2);
							r = r->suc;
						}
						if (r)
						{
							// 还有下一个结点，但由于没有空串first而停止
						}

					}
					else
					{
						has_null_first = 0;
					}
				}
				if ((!r) || (has_null_first == 1))
				{
					// cout<<"follow: "<<HeadNode->sym<<endl;
					int nsym_fs_index;
					nsym_fs_index = fs_lookup(HeadNode->sym);
					if (!fs_table[nsym_fs_index].hasFO)
					{
						FOLLOW3(HeadNode, fsl);
					}
				}
			}
			q = q->suc;
		}
		
            p = p->alt;
		
        }
        HeadNode = HeadNode->suc;
    }
    return fsl;
}
