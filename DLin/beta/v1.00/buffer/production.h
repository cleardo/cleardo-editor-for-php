/**
 * 语法产生式数据结构表示
 */
#define TSYMSTRMAX 9999
#define TSYMMAX 100 // 支持100个终结符

struct pdt_node;    // 语法产生式结点
struct pdt_header;  // 语法产生式非终结符结点

typedef struct pdt_node* pdt_pointer;	    // 语法产生式结点指针
typedef struct pdt_header* pdt_hpointer;    // 语法产生式头结点指针