#define BUFFER_SIZE 5
#define empty '#'

extern FILE* infile;
extern int sym;
extern int cc;
extern int ll;
extern char ch;
extern char line[];

// Fisrt集的链表
typedef struct FSNode FSNode;
typedef FSNode *PtrToFSnode;
typedef PtrToFSnode FSList;
typedef PtrToFSnode Position;
struct FSNode {
    char sym;
    Position Next;
};

typedef struct node* pointer;
typedef struct header* hpointer;

// 产生式结点
struct node
{
    int terminal;
    union {
        char tsym;
        hpointer nsym;
    } sym;
    pointer suc;
    pointer alt;
};

struct header
{
    char sym;
    pointer entry;
    hpointer suc;
    FSList fset;
};

extern hpointer list, sentinel;
extern hpointer h;
extern pointer p;

void input();
void getch(void);
int getsym();
int parser();
void error();
void term(pointer &p, pointer &q, pointer &r);
void factor(pointer &p, pointer &q);
void expression(pointer &p, pointer &q);
void find(char, hpointer &h);

FSList FIRST(hpointer h);
FSList FIRST_Str(pointer);
void FIRST2(pointer p, FSList);
int hasNullFirst(hpointer h);

FSList FOLLOW(hpointer h);
void FOLLOW2(pointer, FSList, char, hpointer);
int hasNullFirst_Str(FSList);
	
void CreateFSL(FSList &fsl);
void InsertFSL(FSList, char);
int LocateFSL(FSList, char);
void MergeFSL(FSList fsl1, FSList fsl2);
void PrintFSL(FSList);
