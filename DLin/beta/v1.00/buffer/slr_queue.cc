#include "common.h"
#include "production.h"
#include "slr_queue.h"


Status InitQueue(LinkQueue &Q)
{
		Q.front = Q.rear = (QueuePtr)malloc(sizeof(QNode));
		if(!Q.front) 
				exit(OVERFLOW);
		Q.front->next = NULL;
		return OK;
}

Status EnQueue(LinkQueue &Q, QElemType e)
{
		QueuePtr p;
		p = (QueuePtr)malloc(sizeof(QNode));
		if(!p) exit(OVERFLOW);
		p->data = e;
		p->next = NULL;
		Q.rear->next = p;
		Q.rear = p;
		return OK;
}

Status DeQueue(LinkQueue &Q, QElemType &e)
{
		if(Q.front == Q.rear)
				return ERROR;
		QueuePtr p;
		p = Q.front->next;
		e = p->data;
		Q.front->next = p->next;
		if(Q.rear == p)
				Q.rear = Q.front;
		free(p);
		return OK;
}

Status QueueTraverse(LinkQueue Q, void (*visit)(QElemType e))
{
		cout<<endl<<"项处理队列中元素："<<endl;
		QueuePtr p;
		p = Q.front->next;
		while(p) {
				slr_visit(p->data);
				p = p->next;
		}
		cout<<endl;
		return OK;
}

