#include "common.h"
#include "global.h"

void factor(pointer &p, pointer &q)
{
	pointer a,b;
	hpointer h;

	if ((sym >= 65 && sym <= 90) || (sym >= 97 && sym <= 122) || sym ==  empty || sym == '+' || sym == '*' || sym == '(' || sym == ')') 
	{
		a = (pointer)malloc(sizeof(struct node));
		if (sym >= 65 && sym <= 90)
		{
			find(sym, h);
			a->terminal = 0;
			a->sym.nsym = h;
            a->alt = NULL;
		}
		else
		{
			a->terminal = 1;
			a->sym.tsym = sym;
            a->alt = NULL;
		}
		p = a;
		q = a;	
		getsym();
	}
}

void term(pointer &p, pointer &q, pointer &r)
{
	pointer a,b,c;
	factor(p, a);
	q = a;
	// cout<<"the x: "<<a->terminal<<"\t"<<a->sym.tsym<<endl;
	// cout<<"the A: "<<a->terminal<<"\t"<<a->sym.nsym->sym<<endl;
	while ( (sym >= 65 && sym <= 90) || sym == empty || sym == '+' || sym == '*' || (sym >= 97 && sym <= 122) || sym =='[' || sym == '(' || sym == ')')
	{
		factor(a->suc, b);
		b->alt = NULL;
		a = b;
	}
	r = a;
	// cout<<"the p of term: "<<p->sym.tsym<<endl;
	// cout<<"the q of term: "<<q->sym.tsym<<endl;
	// cout<<"the r of term: "<<r->sym.nsym->sym<<endl;
}

void expression(pointer &p, pointer &q)
{
	pointer a,b,c;
	term(p, a, c);
	while (sym == ',')
	{
		getsym();
		term(a->alt, b, c);
		c->suc = NULL;
		a = b;
	}
	q = a;
	// cout<<"the p of term: "<<p->sym.tsym<<endl;
	// cout<<"the q of term: "<<q->sym.tsym<<endl;
}
