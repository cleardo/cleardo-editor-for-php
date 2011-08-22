/**
 * �﷨����ʽ���ݽṹ�������������
 */
#include "common.h"
#include "global.h"
#include "production.h"
#include "lexer.h"

char tsym_chars[TSYMSTRMAX];	// ���ڱ����ս�����ַ���
int pdt_lastchar = -1;		// �ս���ַ���ʹ�õ����һ������
char* tsym_table[TSYMMAX];	// ���ڱ����ս����
int pdt_lastentry = 0;		// �ս����������

struct pdt_node
{
    int terminal;	    // �Ƿ��ս����1 - �ս��; 2 - ���ս��
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
    pdt_pointer entry;	    // ָ���﷨����ʽ��ʼ���
    pdt_hpointer suc;	    // ָ���ѡ�﷨����ʽ
};

void pdt_declaration()
{
    char* lexeme_str;
    while ((lexical_token = nexttoken()) == PDT_NEWTERMINAL)
    {
	lexical_token = nexttoken();
	cout<<lexical_token<<endl;
	//lex_get_lexeme(lexeme_str);
	//printf("%s\t", lexeme_str);
	//pdt_insertNewTerminal(lexeme_str);
    }
    cout<<lexical_token<<endl;
}

void pdt_insertNewTerminal(char s[])
{
    
}

void pdt_terminalLookup()
{
}