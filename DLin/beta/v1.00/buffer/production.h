/**
 * 语法产生式数据结构表示
 */
#define TSYMSTRMAX 9999
#define TSYMMAX 100 // 支持100个终结符

struct pdt_node;    // 语法产生式结点
struct pdt_header;  // 语法产生式非终结符结点
struct slr_item;

typedef struct pdt_node* pdt_pointer;	    // 语法产生式结点指针
typedef struct pdt_header* pdt_hpointer;    // 语法产生式头结点指针
typedef struct slr_item* slr_ipointer;	    // 项指针
typedef slr_ipointer slr_ilist;

extern pdt_hpointer pdt_list;	// 语法产生式头结点链表
extern pdt_hpointer pdt_sentinel;
extern slr_ipointer slr_sentinel;
extern pdt_hpointer pdt_headFound;
extern pdt_pointer pdt_tail;	// 指向语法产生式链表结构的末端结点
extern pdt_hpointer cleardo_G;

int pdt_declaration();			    // 处理phpyacc声明部分
void pdt_insertNewTerminal(int);	    // 将终结符插入终结符表
int pdt_terminalLookup(int lex_sym_index); // 查找该终结符是否已存在，已存在则不进行插入
int pdt_isTerminal(char s[]);		    // 根据字符串判断该字符串是否终结符
void pdt_headerFind(char* s, pdt_hpointer &h);	// 在非终结符链表中，查找符号串s，如果不存在，则插入到链表中
void pdt_initPdtSentinel();
pdt_pointer pdt_new_node();
pdt_pointer pdt_set_tsym_value(pdt_pointer & p, int index, pdt_pointer alt, pdt_pointer suc);
pdt_pointer pdt_set_nsym_value(pdt_pointer & p, pdt_hpointer, pdt_pointer alt, pdt_pointer suc);
void pdt_link_factor(pdt_pointer &a, pdt_pointer &b);
void pdt_link_term(pdt_pointer &p, pdt_pointer &a, pdt_pointer &b, pdt_pointer &c);
void expr_parse(pdt_hpointer &h, pdt_pointer &t);

// SLR算法数据结构
void slr_initPdtArr();
slr_ipointer slr_initI0();
void slr_itemInsert(slr_ilist list, slr_ipointer item);
void slr_itemlist_dump(slr_ipointer list);
void slr_item_closure(slr_ipointer item);
void slr_pdt_dump();
int slr_getPdtNo(pdt_pointer p);
void slr_pdtNoTest();
void slr_visit(slr_ipointer e);
int slr_isAdded(pdt_hpointer h);
void slr_added(pdt_hpointer h);
void slr_added_test();
slr_ipointer slr_goto(slr_ipointer list, int index);
int SlrCreateList(slr_ilist &L);
void SlrInsert(slr_ipointer X, slr_ilist L);
void SlrPrint(slr_ilist L);
void slr_item_op();
int slr_goto_added_tsym(pdt_hpointer h, int tsym_index);
void set_goto_added_tsym(pdt_hpointer h, int tsym_index);
int slr_goto_added_nsym(pdt_hpointer h, char* sym);
void set_goto_added_nsym(pdt_hpointer h, char* sym);
void added_goto_tsym();
void added_goto_nsym();
pdt_hpointer pdt_getheader(pdt_pointer pdt);
void pdt_getheader_test();
int SlrLocate(slr_ipointer fsl, slr_ipointer ch);
void SlrMerge(slr_ipointer fsl1, slr_ipointer fsl2);
slr_ilist slr_sort(slr_ilist p);
int slr_exsist(slr_ilist p, int count);
int slr_compare(slr_ilist L1, slr_ilist L2);

char* pdt_getTerminalSym(int index);
void pdt_list_dump();
void pdt_tsymtable_dump();
void pdt_header_output(pdt_hpointer);
void pdt_pointer_output(pdt_pointer p);
void pdt_tree_dump();
void fs_initFsTable();
void fs_tableDump();
int fs_lookup(char ch[]);


int pdt_isEmpty(int tsym_index);
int pdt_test_isEmpty();
void pdt_initTerminalTable();
