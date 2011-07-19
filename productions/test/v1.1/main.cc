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
	// cout<<h->entry->suc->sym.nsym->sym<<endl;
	hpointer list2;
	list2 = list;
    /*
    cout<<"the non-terminal symbol parsed: "<<list->suc->suc->suc->sym<<endl;
    if (list->suc->entry->alt == NULL)
    {
        cout<<"D's alt is null"<<endl;
    }
    */
    //cout<<list->suc->sym<<endl;
    FSList fsl, fsl2;
    CreateFSL(fsl);
    // cout<<fsl->sym<<endl;
    fsl = FIRST(list->suc);
    //fsl = FIRST_Str(list->suc->entry);
    PrintFSL(fsl);
    // cout<<"is null: "<<hasNullFirst(list->suc->suc->suc);

    //ArrayOutput(FS[1].first);
    // cout<<hasNullFirst(list->suc)<<endl;
	// cout<<"list: "<<list->sym<<endl;
    /*
	while (list2 != sentinel)
	{
		// cout<<"list: "<<list2->sym<<endl;
		list2 = list2->suc;
	}
    */
	return 1;
}
