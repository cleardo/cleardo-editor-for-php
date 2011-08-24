/**
 * 语法产生式数据结构定义与操作函数
 */
#include "common.h"
#include "global.h"
#include "production.h"
#include "lexer.h"
#include "symbol.h"
#include "pdt_parser.h"
#include "slr_queue.h"

char tsym_chars[TSYMSTRMAX];	// 用于保存终结符的字符串
int pdt_lastchar = -1;		// 终结符字符串使用的最后一个索引
char* tsym_table[TSYMMAX];	// 用于保存终结符表
int pdt_lastentry = 0;		// 终结符表项索引

struct slr_add_entry {
    pdt_hpointer nsym;
    int added;
};

struct slr_add_goto_tsym {
    pdt_hpointer pdt;
    int tsym;
    int added;
};

struct slr_add_goto_nsym {
    pdt_hpointer pdt;
    char* sym;
    int added;
};

struct slr_add_entry slr_added_arr[200];
int slr_added_lastentry = 0;

struct slr_add_goto_tsym slr_added_goto_tsym_arr[200];
int slr_added_goto_tsym_lastentry = 0;

struct slr_add_goto_nsym slr_added_goto_nsym_arr[200];
int slr_added_goto_nsym_lastentry = 0;

pdt_hpointer pdt_sentinel = NULL;
pdt_hpointer pdt_list = NULL;
pdt_hpointer pdt_headFound = NULL;
pdt_hpointer cleardo_G = NULL;	// 扩展文法的起始符号(G')

pdt_pointer slr_productions[200];   // 语法产生式编号
slr_ipointer slr_sentinel = NULL;
slr_ipointer slr_I[500];

struct slr_item 
{
    int pdt_index;  // 语法产生式索引
    int item_pos;   // 项标示，即item中点的位置
    slr_ipointer next;	// 下一个项结点
};



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

int pdt_declaration()
{
    char* lexeme_str;
    int token;
    while ((token = nexttoken()) == PDT_NEWTERMINAL)
    {
	token = nexttoken();
	//cout<<token<<endl;
	//printf("%s\t", lexeme_str);
	pdt_insertNewTerminal(lexical_value);
    }
    return token;
}

void pdt_insertNewTerminal(int tsym_index)
{
    tsym_table[++pdt_lastentry] = sym_getSymbolEntry(tsym_index);
}

int pdt_terminalLookup(int lex_value)
{
	int p;
	char* s = sym_getSymbolEntry(lex_value);
	for (p = pdt_lastentry; p > 0; p--)
	{
		// cout<<lastentry<<endl;
		if (strcmp(tsym_table[p], s) == 0)
			return p;
	}
	return 0;
}

int pdt_isTerminal()
{

}

/**
 * 初始化语法产生式头结点链表哨兵
 */
void pdt_initPdtSentinel()
{
    pdt_sentinel = (pdt_hpointer)malloc(sizeof(struct pdt_header));
    pdt_sentinel->suc = NULL;
    pdt_sentinel->entry = NULL;
    pdt_list = pdt_sentinel;
}

void pdt_headerFind(char* s, pdt_hpointer &h)
{
	pdt_hpointer h1;    // 
	h1 = pdt_list;
	//cout<<s<<endl;
	strcpy(pdt_sentinel->sym, s);

	
	while (strcmp(h1->sym, s) != 0)
	{
		h1 = h1->suc;
	}
	if (h1 == pdt_sentinel)
	{
		pdt_sentinel = (pdt_hpointer)malloc(sizeof(struct pdt_header));
		h1->suc = pdt_sentinel;
		h1->entry = NULL;
	}
	h = h1;
	// cout<<"h1'sym: "<<h1->sym<<endl;
}

pdt_pointer pdt_new_node()
{
    pdt_pointer p;
    p = (pdt_pointer)malloc(sizeof(struct pdt_node));
    return p;
}

pdt_pointer pdt_set_tsym_value(pdt_pointer & p, int index, pdt_pointer alt, pdt_pointer suc)
{
    p->terminal = 1;
    p->sym.tsym = index;	// 在终结符号表中的索引
    p->alt = alt;
    p->suc = suc;
}

pdt_pointer pdt_set_nsym_value(pdt_pointer & p, pdt_hpointer h, pdt_pointer alt, pdt_pointer suc)
{
    p->terminal = 0;
    p->sym.nsym = h;	// 在终结符号表中的索引
    p->alt = alt;
    p->suc = suc;
}

void pdt_link_factor(pdt_pointer &a, pdt_pointer &b)
{
	pdt_factor(a->suc, b);
	b->alt = b->suc = NULL;
	a = b;	// a移到下一个结点，在下一次调用factor()时，a->suc才能指向下下个结点
}

void pdt_link_term(pdt_pointer &p, pdt_pointer &a, pdt_pointer &b, pdt_pointer &c)
{
	pdt_term(a->alt, b, c);
	c->suc = NULL;
	a = b;
}

void expr_parse(pdt_hpointer &h, pdt_pointer &t)
{
    pdt_expression(h->entry, t);	// 开始解析语法产生式右半部分
}

/**
 * 初始化语法产生式数组
 */
void slr_initPdtArr()
{
    
    cleardo_G = (pdt_hpointer)malloc(sizeof(struct pdt_header));	// 扩展方法的起始符号
    char clear_sym[200] = "cleardo_G";
    strcpy(cleardo_G->sym, clear_sym); 
    
    pdt_pointer cleardo_begin;
    cleardo_begin = (pdt_pointer)malloc(sizeof(struct pdt_node));
    cleardo_begin->terminal = 0;
    cleardo_begin->alt = cleardo_begin->suc = NULL;
    cleardo_begin->sym.nsym = pdt_list;

    cleardo_G->entry = cleardo_begin;
    // cout<<cleardo_G->entry->sym.nsym->sym<<endl;

    pdt_hpointer cursor = pdt_list;
    slr_productions[0] = cleardo_G->entry;
    slr_sentinel = (slr_ipointer)malloc(sizeof(struct slr_item));

    cursor = pdt_list;
    int pdt_i = 1;
    while ( cursor != pdt_sentinel) 
    {
	pdt_pointer p = cursor->entry;
	while (p)
	{
	    slr_productions[pdt_i++] = p;
	    p = p->alt;
	}
	cursor = cursor->suc;
    }
    slr_productions[pdt_i] = NULL;
}


slr_ipointer slr_initI0()
{
    slr_ilist L;
    SlrCreateList(L);
    slr_I[0] = L;
    slr_ipointer slr_i_0_0;
    slr_i_0_0 = (slr_ipointer)malloc(sizeof(struct slr_item));
    slr_i_0_0->pdt_index = 0;
    slr_i_0_0->item_pos = 0;
    SlrInsert(slr_i_0_0, slr_I[0]);
    return slr_I[0];
}

/**
 * 插入新的项
 */
void slr_itemInsert(slr_ilist list, slr_ipointer item)
{
    slr_ipointer cursor = list;
    while (cursor != slr_sentinel)
    {
	cursor = cursor->next;
    }
    if (cursor == slr_sentinel) {
	slr_sentinel = (slr_ipointer)malloc(sizeof(struct slr_item));
	//cout<<item->pdt_index<<'\t'<<item->item_pos<<endl;
	cursor->pdt_index = item->pdt_index;
	cursor->item_pos = item->item_pos;
	cursor->next = slr_sentinel;
    }
}

int slr_isAdded(pdt_hpointer h)
{
    int p;
    for (p = slr_added_lastentry; p > 0; p--)
    {
	    // cout<<lastentry<<endl;
	    if (slr_added_arr[p].nsym == h)
		    return slr_added_arr[p].added;
    }
    return 0;
}

void slr_added(pdt_hpointer h)
{
    slr_added_lastentry++;
    slr_added_arr[slr_added_lastentry].nsym = h;
    slr_added_arr[slr_added_lastentry].added = 1;
}

void slr_added_test()
{
    int p;
    slr_added(pdt_list);
    slr_added(pdt_list->suc);
    for (p = slr_added_lastentry; p > 0; p--)
    {
	    cout<<slr_added_arr[p].nsym->sym<<"\t"<<slr_added_arr[p].added;
    }
    cout<<"line: added?"<<slr_isAdded(pdt_list->suc->suc)<<endl;
}

/**
 * 求项的闭包
 */
void slr_item_closure(slr_ipointer item)
{
    LinkQueue Q;
    InitQueue(Q);
    if (item->next) {
	EnQueue(Q, item->next);
    }
    // QueueTraverse(Q, slr_visit);
    slr_ipointer slr_elem;
    while (DeQueue(Q, slr_elem))
    {
	int pdt_id = slr_elem->pdt_index;
	//cout<<"prod no:"<<pdt_id<<endl;
	int i = slr_elem->item_pos;
	pdt_pointer pdt, pdt_entry;
	pdt = slr_productions[pdt_id];  // 取得指定项的产生式
	while (i > 0)
	{ 
	    pdt = pdt->suc;
	    i--;
	}
	if (!pdt)
	{
	    continue;
	}
	if (pdt->terminal == 0 && !slr_isAdded(pdt->sym.nsym))
	{
	    pdt_entry = pdt->sym.nsym->entry;
	    while (pdt_entry)
	    {
		int pno = slr_getPdtNo(pdt_entry);
		slr_ipointer slr_new_item;
		slr_new_item = (slr_ipointer)malloc(sizeof(struct slr_item));
		slr_new_item->pdt_index = pno;
		slr_new_item->item_pos = 0;
		SlrInsert(slr_new_item, item);
		EnQueue(Q, slr_new_item);
		pdt_entry = pdt_entry->alt;
	    }
	    slr_added(pdt->sym.nsym);
	} 
	// break;
    }
}

pdt_hpointer pdt_getheader(pdt_pointer pdt)
{
    pdt_hpointer h;
    pdt_pointer p;
    h = pdt_list;
    while (h!= pdt_sentinel)
    {
	p = h->entry;
	while (p)
	{
	    if (p == pdt)
	    {
		return h;
	    }
	    p = p->alt;
	}
	h = h->suc;
    }
}

void pdt_getheader_test()
{
    pdt_hpointer h;
    h = pdt_getheader(slr_productions[1]);
    cout<<"get head: "<<h->sym<<endl;
}

/**
 * 求项的GOTO集
 */
slr_ipointer slr_goto(slr_ipointer list, int index)
{
    int tsym_index;	// 用于判断是否非终结符
    char* sym;
    slr_ilist itemlist;	// 新的Item集合
    slr_ilist nolist;	// 空的Item集合
    SlrCreateList(itemlist);	// 创建新Item链表
    SlrCreateList(nolist);	// 创建新Item链表

    slr_ilist tsymlist;	// 空的Item集合
    SlrCreateList(tsymlist);	// 创建新Item链表
    slr_ilist nsymlist;	// 空的Item集合
    SlrCreateList(nsymlist);	// 创建新Item链表

    slr_ipointer item_cursor, item;
    tsym_index = pdt_terminalLookup(index); // tsym_index != 0，则表示GOTO
    //cout<<"tsym_index: "<<tsym_index<<endl;
    
    if (tsym_index == 0)
    {
	sym = sym_getSymbolEntry(index);
    }
    //cout<<sym<<endl;
    item_cursor = list->next;
    while(item_cursor)
    {
	int insertable = 0;
	int pdt_id = item_cursor->pdt_index;
	//cout<<"pdt_id:"<<pdt_id<<endl;
	int i = item_cursor->item_pos;
	int new_pos = item_cursor->item_pos;
	pdt_pointer pdt, pdt_entry;
	pdt = slr_productions[pdt_id];
	pdt_hpointer h;
	h = pdt_getheader(pdt);
	while (i > 0)
	{
	    pdt = pdt->suc;
	    i--;
	}
	if (!pdt)
	{
	    item_cursor = item_cursor->next;
	    continue;
	}
        if ((pdt->terminal == 1) && tsym_index)
	{
	    if (pdt->sym.tsym == tsym_index && (!slr_goto_added_tsym(h, tsym_index)))
	    {
		item = (slr_ipointer)malloc(sizeof(struct slr_item));
		item->pdt_index = pdt_id;
		item->item_pos = new_pos + 1;
		SlrInsert(item, itemlist);
		insertable = 1;
		set_goto_added_tsym(h, tsym_index);
	    } else if (pdt->sym.tsym == tsym_index && slr_goto_added_tsym(h, tsym_index))
	    {
		item = (slr_ipointer)malloc(sizeof(struct slr_item));
		item->pdt_index = pdt_id;
		item->item_pos = new_pos + 1;
		SlrInsert(item, tsymlist);
	    }
	   
	    
	} else if ((pdt->terminal == 0) && (tsym_index == 0)) {
	    if (strcmp(pdt->sym.nsym->sym, sym) == 0 && (!slr_goto_added_nsym(h, sym)))
	    {
		item = (slr_ipointer)malloc(sizeof(struct slr_item));
		item->pdt_index = pdt_id;
		item->item_pos = new_pos + 1;
		SlrInsert(item, itemlist);
		insertable = 1;
		set_goto_added_nsym(h, sym);
	    } else if (strcmp(pdt->sym.nsym->sym, sym) == 0 && slr_goto_added_nsym(h, sym))
	    {
		item = (slr_ipointer)malloc(sizeof(struct slr_item));
		item->pdt_index = pdt_id;
		item->item_pos = new_pos + 1;
		SlrInsert(item, nsymlist);
	    }
	}
	if (insertable == 1)
	{
	    SlrMerge(itemlist, tsymlist);
	    SlrMerge(itemlist, nsymlist);
	}
	item_cursor = item_cursor->next;    // 下一个I[n]链表项
    }
    slr_added_lastentry = 0;	    // 重新初始化added数组
    slr_item_closure(itemlist);
    return itemlist;
}

int slr_goto_added_tsym(pdt_hpointer pdt, int tsym_index)
{
    int p;
    for (p = slr_added_goto_tsym_lastentry; p > 0; p--)
    {
	    // cout<<lastentry<<endl;
	    if ((slr_added_goto_tsym_arr[p].pdt == pdt) && (slr_added_goto_tsym_arr[p].tsym == tsym_index))
		    return slr_added_goto_tsym_arr[p].added;
    }
    return 0;
}

void set_goto_added_tsym(pdt_hpointer pdt, int tsym_index)
{
    slr_added_goto_tsym_lastentry++;
    slr_added_goto_tsym_arr[slr_added_goto_tsym_lastentry].pdt = pdt;
    slr_added_goto_tsym_arr[slr_added_goto_tsym_lastentry].tsym = tsym_index;
    slr_added_goto_tsym_arr[slr_added_goto_tsym_lastentry].added = 1;
}

int slr_goto_added_nsym(pdt_hpointer pdt, char* sym)
{
    int p;
    for (p = slr_added_goto_nsym_lastentry; p > 0; p--)
    {
	    // cout<<lastentry<<endl;
	    if ((slr_added_goto_nsym_arr[p].pdt == pdt) && (strcmp(slr_added_goto_nsym_arr[p].sym, sym) == 0))
		    return slr_added_goto_nsym_arr[p].added;
    }
    return 0;
}

void set_goto_added_nsym(pdt_hpointer pdt, char* sym)
{
    slr_added_goto_nsym_lastentry++;
    slr_added_goto_nsym_arr[slr_added_goto_nsym_lastentry].pdt = pdt;
    slr_added_goto_nsym_arr[slr_added_goto_nsym_lastentry].sym = sym;
    slr_added_goto_nsym_arr[slr_added_goto_nsym_lastentry].added = 1;
}

void slr_item_op()
{
    LinkQueue Q;
    InitQueue(Q);
    EnQueue(Q, slr_I[0]);
    int item_count = 1;
    slr_ipointer slr_elem, p;
    int symcount = sym_getEntryIndex();
    while (DeQueue(Q, slr_elem))
    {
	//SlrPrint(slr_elem);
	//cout<<endl<<"symcount: "<<symcount<<endl;
	
	for (int i = symcount; i > 0; i--)
	{
	    cout<<endl;
	    // SlrPrint(slr_elem);
	    p = slr_goto(slr_elem, i);
	    if (p->next != NULL)
	    {
		cout<<endl<<"item_count: "<<item_count<<'\t'<<i<<endl;
		//if (item_count > 10) break;
		SlrPrint(p);
		slr_I[item_count++] = p;

		EnQueue(Q, p);
	    }
	}
	//break;
    }
    //cout<<endl<<"item_count: "<<item_count<<endl;
    //SlrPrint(slr_I[1]);
    //slr_ipointer item_test = slr_goto(slr_I[1], 9);
    //SlrPrint(item_test);
}

int slr_getPdtNo(pdt_pointer p)
{
    int i = 0;
    while (slr_productions[i] != NULL)
    {
	if (slr_productions[i] == p)
	{
	    return i;
	}
	i++;
    }
    return -1;
}

int SlrCreateList(slr_ilist &L)
{
    slr_ipointer Node;
    Node = (slr_ipointer)malloc(sizeof(struct slr_item));
    Node->pdt_index = -1;
    Node->item_pos = -1;
    Node->next = NULL;
    L = Node;
}

void SlrInsert(slr_ipointer item, slr_ilist L)
{
    slr_ipointer Node;
    Node = (slr_ipointer)malloc(sizeof(struct slr_item));
    Node->pdt_index = item->pdt_index;
    Node->item_pos = item->item_pos;
    Node->next = L->next;
    L->next = Node;
}

int SlrLocate(slr_ipointer fsl, slr_ipointer ch)
{
    slr_ipointer Node;
    Node = fsl->next;
    while (Node && Node->pdt_index != ch->pdt_index && Node->item_pos != ch->item_pos)
    {
        Node = Node->next;
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

void SlrMerge(slr_ipointer fsl1, slr_ipointer fsl2)
{
    slr_ipointer Node;
    Node = fsl2->next;
    while (Node)
    {   
        if (!SlrLocate(fsl1, Node))
        {
            SlrInsert(Node, fsl1);
        }
        Node = Node->next;
    }
}

void SlrPrint(slr_ilist L)
{
    slr_ipointer cursor;
    cursor = L;
    while(cursor = cursor->next) {
         cout<<cursor->pdt_index<<'\t'<<cursor->item_pos<<endl;
    }
}

void slr_pdtNoTest()
{
    cout<<"pdt NO: "<<slr_getPdtNo(pdt_list->suc->entry);
}

void added_goto_tsym()
{
	int i;
	for (i = 1; i <= slr_added_goto_tsym_lastentry; i++)
	{
		cout<<slr_added_goto_tsym_arr[i].pdt->sym<<'\t'<<slr_added_goto_tsym_arr[i].tsym<<endl;
	}
	return;
}

void added_goto_nsym()
{
	int i;
	for (i = 1; i <= slr_added_goto_nsym_lastentry; i++)
	{
		cout<<slr_added_goto_nsym_arr[i].pdt->sym<<'\t'<<slr_added_goto_nsym_arr[i].sym<<endl;
	}
	return;
}

void slr_pdt_dump()
{
    int i = 0;
    cout<<"产生式数组："<<endl;
    for (i = 0; i <= 7; i++)
    {
	if (slr_productions[i]->terminal == 1)
	{
	    cout<<slr_productions[i]->sym.tsym<<endl;
	}
	else
	{
	    cout<<slr_productions[i]->sym.nsym->sym<<endl;
	}
    }
}

void slr_itemlist_dump(slr_ipointer list)
{
    slr_ipointer cursor = list;
    cout<<"项集合："<<endl;
    while (cursor != slr_sentinel)
    {
	cout<<cursor->pdt_index<<'\t'<<cursor->item_pos<<endl;
	cursor = cursor->next;
    }
}

/**
 * 测试排错函数
 */
void pdt_list_dump()
{
    cout<<"开始打印语法产生式链表结构："<<endl;
    cout<<cleardo_G->sym<<"->";
    pdt_hpointer cursor = pdt_list;
    while (cursor != pdt_sentinel)
    {
	cout<<cursor->sym<<"->";
	cursor = cursor->suc;
    }
    
    cout<<endl;
}

void pdt_tsymtable_dump()
{
	int i;
	for (i = 1; i <= pdt_lastentry; i++)
	{
		cout<<tsym_table[i]<<endl;
	}
	return;
}

void pdt_header_output(pdt_hpointer h)
{
    cout<<endl;
    cout<<"非终结符值：\t";
    cout<<h->sym;
}

void pdt_pointer_output(pdt_pointer p)
{
    cout<<endl;
    cout<<"终结符值：\t";
    cout<<p->sym.tsym<<'\t';
    if (p->suc == NULL) {
	cout<<"suc: 0"<<'\t';
    }
    if (p->alt == NULL) {
	cout<<"alt: 0"<<'\t';
    }
}


void slr_visit(slr_ipointer e)
{
    cout<<e->pdt_index<<"\t";
    cout<<e->item_pos<<endl;
}

void pdt_tree_dump()
{
    	pdt_header_output(pdt_list->entry->sym.nsym);			    // line 的第一个结点: expr
	pdt_pointer_output(pdt_list->entry->suc);			    // line 的第二个结点: 2
	pdt_header_output(pdt_list->suc->entry->sym.nsym);		    // expr 的第一个结点: expr
	pdt_pointer_output(pdt_list->suc->entry->suc);			    // expr 的第二个结点: 3
	pdt_header_output(pdt_list->suc->entry->suc->suc->sym.nsym);	    // expr 的第三个结点: term
	pdt_header_output(pdt_list->suc->entry->alt->sym.nsym);		    // expr 的第一个可选结点: term
	pdt_header_output(pdt_list->suc->suc->entry->sym.nsym);		    // term 的第一个结点: term
	pdt_pointer_output(pdt_list->suc->suc->entry->suc);		    // term 的第二个结点: 4
	pdt_header_output(pdt_list->suc->suc->entry->suc->suc->sym.nsym);   // term 的第三个结点: factor
	pdt_header_output(pdt_list->suc->suc->entry->alt->sym.nsym);	    // term 的第一个可选结点: factor
	pdt_pointer_output(pdt_list->suc->suc->suc->entry);		    // factor 的第一个结点: 5
	pdt_header_output(pdt_list->suc->suc->suc->entry->suc->sym.nsym);   // factor 的第二个结点: expr
	pdt_pointer_output(pdt_list->suc->suc->suc->entry->suc->suc);	    // factor 的第三个结点：6
	pdt_pointer_output(pdt_list->suc->suc->suc->entry->alt);	    // factor 的可选第一个结点：1
	pdt_pointer_output(pdt_list->suc->suc->suc->entry->alt->alt);	    // factor 的可选第一个结点：2
}