#include "common.h"
#include "global.h"

void FIRST2(pointer p, char* symbol)
{
    int symbolIndex;
    if (p->terminal == 1 && p->sym.tsym != '#')
    {
        if (symbolIndex = getFirstIndex(*symbol)) {
            FS[symbolIndex].first[FS[symbolIndex].len++] = p->sym.tsym;
            // cout<<p->sym.tsym<<endl;
        }
        else
        {
            symbolIndex = FirstSetLength;
            FS[FirstSetLength++].symbol = *symbol;
            FS[FirstSetLength].len = 1;
            FS[symbolIndex].first[FS[symbolIndex].len++] = p->sym.tsym;
            // cout<<p->sym.tsym<<endl;
        } 
    }
    else if (p->terminal == 1 && p->sym.tsym == '#' && !p->suc)
    {
        if (symbolIndex = getFirstIndex(*symbol)) {
            FS[symbolIndex].first[FS[symbolIndex].len++] = '#';
        }
        else
        {
            symbolIndex = FirstSetLength;
            FS[FirstSetLength++].symbol = *symbol;
            FS[FirstSetLength].len = 1;
            FS[symbolIndex].first[FS[symbolIndex].len++] = '#';
        }
    }
    else if (p->terminal == 0)
    {
        // cout<<p->sym.nsym->sym<<endl;
        FIRST(p->sym.nsym, symbol);
        // cout<<hasNullFirst(p->sym.nsym)<<endl;
        if (hasNullFirst(p->sym.nsym) && p->suc)
        {
            cout<<"has null"<<endl;
            p = p->suc;
            FIRST2(p, symbol);
        }
        if (!hasNullFirst(p->sym.nsym) && !p->suc)
        {
            if (symbolIndex = getFirstIndex(*symbol)) {
                FS[symbolIndex].first[FS[symbolIndex].len++] = '#';
            }
            else
            {
                symbolIndex = FirstSetLength;
                FS[FirstSetLength++].symbol = *symbol;
                FS[FirstSetLength].len = 1;
                FS[symbolIndex].first[FS[symbolIndex].len++] = '#';
            }
        }
    }
    return;
}

void FIRST(hpointer h, char* sp)
{
    pointer p;
    p = h->entry;
    while (p != NULL)
    {
        FIRST2(p, sp);
        p = p->alt;
    }
}

int hasNullFirst(hpointer h)
{
    int index;
    int length = 9;
    index = getFirstIndex(h->sym);
    if (index == 0) {
        // cout<<"sorry, this symbol "<<h->sym<<" is undefined!"<<endl;
        // exit(1);
        index = FirstSetLength;
        FS[FirstSetLength++].symbol = h->sym;
    } 
    FS[index].first[0] = '#';
    for (length = 9; FS[index].first[length] != '#'; length--) 
    {
        ;
    }
    if (length == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int getFirstIndex(char ch)
{
    int length;
    FS[0].symbol = ch;
    for (length = 9; FS[length].symbol != ch; length--)
    {
        ;
    }
    return length;
}
