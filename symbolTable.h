#ifndef _SYMBOL_TABLE_
#define _SYMBOL_TABLE_
#include "token.h"
#define SIZE 500

typedef enum{
	INT_TYPE, REAL_TYPE, REC_TYPE, 
	UNION_TYPE, GLOBAL_TYPE
} Type;

typedef struct{
char* name;
Token tk;
Type type;
} SymbolVal;

typedef struct{
char* name;
SymbolVal val;
} KeyVal;

typedef struct{
KeyVal kv;
LL* next;
} LL;

typedef struct{
    int sz;
    LL* head;
    LL* tail;
} TableEntry;

typedef struct{
    TableEntry tb[SIZE]
} SymbolTable;

int hash(char* name);
void insert(KeyVal kv);
SymbolVal find(char* str);
#endif