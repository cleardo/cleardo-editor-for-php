// 符号表声明
#define NUM 		256
#define ID		257
#define RELOP		258
#define LE		259
#define LT		260
#define NE		261
#define GE		262
#define GT		263
#define EQ		264
#define PDT_DECL_BEGIN	265
#define PDT_DECL_END	266
#define PDT_DELIMIT	267
#define PDT_NEWTERMINAL 268
#define PDT_DERIV	269
#define PDT_LITERAL	270
#define PDT_EXPR_END	271
#define PDT_ALTERNATE	272

#define DONE		300

extern int lexical_value;	// 词素属性，数字的值，ID字符串，ID索引等，为处理方便，这里统一用索引，故使用整型
extern int lexical_token;	// 词素类型
extern char* lexeme_str;	// 词素字符串值
extern int lineno;		// 行号
