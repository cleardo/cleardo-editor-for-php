/**
 * �﷨����ʽ���ݽṹ�������������
 */
#include "production.h"

char tsym_chars[TSYMSTRMAX];	// ���ڱ����ս�����ַ���
char* tsym_table[TSYMMAX];	// ���ڱ����ս����

struct pdt_node
{
    int terminal;   // �Ƿ��ս����1 - �ս��; 2 - ���ս��
    union {
	int tsym;	    // �ս�������ս����������ʾ
	pdt_hpointer nsym;  // ���ս�����÷��ս��ͷָ���ʾ
    }sym;
    pdt_pointer suc;	    // ָ����һ���﷨����ʽ���
    pdt_pointer alt;	    // ָ���ѡ�﷨����ʽ���
};

struct pdt_header
{
    char* sym;
    pdt_pointer entry;	// ָ���﷨����ʽ��ʼ���
    pdt_hpointer suc;	// ָ���ѡ�﷨����ʽ
};