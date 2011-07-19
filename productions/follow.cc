#include "common.h"
#include "global.h"

FSList FOLLOW(hpointer h)
{
    FSList fsl;
	CreateFSL(fsl);
    hpointer HeadNode;
    pointer p;
    HeadNode = list;
	if (h->sym == 'A')
	{
		InsertFSL(fsl, '$');	// 起始符号
	}
    while (HeadNode != sentinel)
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

void FOLLOW2(pointer p, FSList fsl, char sym, hpointer h)
{
    while (p)
    {
        if (p->terminal == 0 && p->sym.nsym->sym == sym)
        {
            if (p->suc)
            {
                FSList fsl2;
                fsl2 = FIRST_Str(p->suc);
                MergeFSL(fsl, fsl2);
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
