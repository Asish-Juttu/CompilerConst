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
#include "typeDef.h"
#include "ast_def.h"

#define HASHTABLE_SIZE 500

typedef struct{
    char* name; // key (or alias of some record)
    char* to; // the actual reocrd referenced by alias
    Token token; // lexeme
    Datatype type; // RNUM, NUM, UNION, RECORD
    char* typeName; // name
    Ast_FieldDefinitions* fieldDefs; // storing pointer to field def list of a record/union
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

struct _SybolTableList;
typedef struct _SymbolTable{
    TableEntry* tb;
    char* name;
} SymbolTable;


typedef struct _SymbolTableList{
    SymbolTable** symTables;
    int size;
    int cap;
    int current;
} SymbolTableList;

extern SymbolTableList localSymbolTableList;

void growIfFull(SymbolTableList* list);
void pushSymbolTable(char* fname);
SymbolTable* topSymbolTable();
SymbolTable* currentSymbolTable();
void resetCurrentSymbolTable();
void loadNextSymbolTable();

typedef struct {

} SymbolTableStack;


extern SymbolTable lexerSymbolTable;
extern SymbolTable typeDefSymbolTable;
extern SymbolTable typeRedefSymbolTable;
extern SymbolTable globVarSymbolTable;
extern SymbolTable fieldSymbolTable;

void initLexerSymbolTable();
void initTypeDefSymbolTable();
void initTypeRedefSymbolTable();
void initGlobVarSymbolTable();
void initGlobalSymbolTables();


void insertVar(char* name, Datatype datatype, char* typeName);
SymbolVal* findVar(char* name);
void insertIntoLexSymbolTable(char* lexeme, Token tk, Datatype t);
void insertTypeDef(char* name, Datatype recOrUn);
void insertTypeRedef(char* name, char* to);
void insertGlobVar(char* name, Datatype t, char* typeName);
void insert(SymbolTable* symTable, KeyVal kv);


SymbolVal* find(SymbolTable* symTable, char* str);
SymbolVal* findType(Ast_SingleOrRecId* id);

int hash(char* name);

#endif