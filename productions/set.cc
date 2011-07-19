#include "common.h"
#include "global.h"

/**
 * 在非终结符链表中，查找符号s，如果不存在，则插入到链表中
 */
void find(char s, hpointer &h)
{
	hpointer h1;
	h1 = list;
	sentinel->sym = s;
	while (h1->sym != s)
	{
		h1 = h1->suc;
	}
	if (h1 == sentinel)
	{
		sentinel = (hpointer)malloc(sizeof(struct header));
		h1->suc = sentinel;
		h1->entry = NULL;
	}
	h = h1;
	// cout<<"h1'sym: "<<h1->sym<<endl;
}
