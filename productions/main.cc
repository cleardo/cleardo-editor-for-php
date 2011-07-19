#include "common.h"
#include "global.h"

int main()
{
	cout<<"data structure for production"<<endl;
	pointer p;  // 指向产生式数据结构的末尾
	sentinel = (hpointer)malloc(sizeof(struct header));
    sentinel->suc = NULL;
    sentinel->entry = NULL;
	list = sentinel;
	input();
	while (getsym() != 256) 
	{
	find(sym, h);
    //cout<<"the h: "<<h->sym<<endl;
	// printf("%c\n", sym);
	if (sym >= 65 && sym <= 90)
	{
		getsym();
	}
	else
	{
		error();
	}
	if (sym == '=')
	{
		getsym();
	}
	else
	{
		error();
	}
	expression(h->entry, p);
	p->alt = NULL;
	if (sym != '.')
	{
		error();
	}
	}
	// getsym();
	// printf("%c", sym);
	hpointer list2;
	list2 = list;
    FSList fsl, fsl2;
    CreateFSL(fsl);
    fsl = FIRST(list);
    PrintFSL(fsl);
    // fsl = FIRST_Str(list->suc->entry);
    fsl = FOLLOW(list);
    // FOLLOW2(list->suc->suc->suc->entry, fsl, 'A');
    PrintFSL(fsl);

    /*
	while (list2 != sentinel)
	{
		// cout<<"list: "<<list2->sym<<endl;
		list2 = list2->suc;
	}
    */
	return 1;
}
