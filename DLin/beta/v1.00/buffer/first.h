// Fisrt集的链表
typedef struct FSNode FSNode;
typedef FSNode *PtrToFSnode;
typedef PtrToFSnode FSList;
typedef PtrToFSnode Position;

FSList FIRST(pdt_hpointer h);	// 对头结点求first集
FSList FIRST_Str(pdt_pointer);
void FIRST2(pdt_pointer p, FSList);
int hasNullFirst(pdt_hpointer h);

FSList FOLLOW(pdt_hpointer h);
void FOLLOW2(pdt_pointer, FSList, char, pdt_hpointer);
int hasNullFirst_Str(FSList);
	
void CreateFSL(FSList &fsl);
void InsertFSL(FSList, int);
int LocateFSL(FSList, int);
void MergeFSL(FSList fsl1, FSList fsl2);
void PrintFSL(FSList);
