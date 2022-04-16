/****   GROUP NUMBER: 43
Name: Rahul B
ID: 2019A7PS0134P

Name: Asish Juttu
ID: 2019A7PS0039P

Name: Narasimha Gupta Jangala
ID: 2019A7PS0138P

Name: Praneeth Chaitanya Jonnavithula
ID: 2019A7PS1111P

Name: Damargidda Rohan Raj Goud
ID: 2019A7PS0065P
****/

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
    TableEntry* tb;
    SymbolTable* previous;
} SymbolTable;
extern SymbolTable symbolTable;

void initLexerSymbolTable(SymbolTable* symbolTable);

SymbolTable* createSymbolTable(SymbolTable* previous);

void insert(SymbolTable* symTable, KeyVal kv);
SymbolVal* find(SymbolTable* symTable, char* str);
int hash(char* name);

#endif