#ifndef _SYMBOL_TABLE_
#define _SYMBOL_TABLE_
#include "token.h"
#define HASHTABLE_SIZE 500


typedef enum{
	INT_TYPE, REAL_TYPE, REC_TYPE, 
	UNION_TYPE, GLOBAL_TYPE, NONE
} Type;

typedef struct{
    char* name;
    Token token;
    Type type;
} SymbolVal;

typedef struct{
    char* name;
    SymbolVal val;
} KeyVal;


struct ll{
    KeyVal kv;
    struct ll* next;
};

typedef struct ll LL;

typedef struct{
    int sz;
    LL* head;
    LL* tail;
} TableEntry;

typedef struct{
    TableEntry tb[HASHTABLE_SIZE];
} SymbolTable;
extern SymbolTable symbolTable;

void initSymbolTable(SymbolTable* symbolTable);
int hash(char* name);
void insert(SymbolTable* symTable, KeyVal kv);
SymbolVal* find(SymbolTable* symTable, char* str);
#endif