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

#include "symbolTable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

SymbolTable lexerSymbolTable;
SymbolTable typeDefSymbolTable;
SymbolTable typeRedefSymbolTable;
SymbolTable globVarSymbolTable;
SymbolTableList localSymbolTableList;
int init = 0;

void initSymTable(SymbolTable* symTable){
    for(int i = 0; i < HASHTABLE_SIZE; i++){
        symTable->tb[i].sz = 0;
        symTable->tb[i].head = NULL;
        symTable->tb[i].tail = NULL;
    }
    symTable->name = NULL;
}

void initTypeDefSymbolTable(){
    typeDefSymbolTable.tb = malloc(HASHTABLE_SIZE * sizeof(TableEntry));
    initSymTable(&typeDefSymbolTable);
}

void initTypeRedefSymbolTable(){
    typeRedefSymbolTable.tb = malloc(HASHTABLE_SIZE * sizeof(TableEntry));
    initSymTable(&typeRedefSymbolTable);
}

void initGlobVarSymbolTable(){
    globVarSymbolTable.tb = malloc(HASHTABLE_SIZE * sizeof(TableEntry));
    initSymTable(&globVarSymbolTable);
}

void initGlobalSymbolTables(){
    if(init){
        free(lexerSymbolTable.tb);
        free(typeDefSymbolTable.tb);
        free(typeRedefSymbolTable.tb);
        free(globVarSymbolTable.tb);
        free(localSymbolTableList.symTables);
    }
    init = 1;
    // typeDefSymbolTable.previous = NULL;
    // typeRedefSymbolTable.previous = NULL;
    // lexerSymbolTable.previous = NULL;
    // globVarSymbolTable.previous = NULL;

    localSymbolTableList.cap = 1;
    localSymbolTableList.size = 0;
    localSymbolTableList.current = -1;

    initLexerSymbolTable();
    initTypeDefSymbolTable();
    initTypeRedefSymbolTable();
    initGlobVarSymbolTable();
}

void growIfFull(SymbolTableList* list){
    if(list->size == list->cap){
        list->cap = (int)(list->cap * 1.4 + 1);
        list->symTables = realloc(list->symTables, list->cap * sizeof(SymbolTable*));
    }
}
void pushSymbolTable(char* fname){
    growIfFull(&localSymbolTableList);
    SymbolTable* symTable = malloc(sizeof(SymbolTable));
    localSymbolTableList.symTables[localSymbolTableList.size] = symTable;
    localSymbolTableList.size++;
}

SymbolTable* topSymbolTable(){
    int index = localSymbolTableList.size - 1;
    if(index < 0){
        printf("Local symbol table list is empty\n");
        return NULL;
    }
    return localSymbolTableList.symTables[index];
}

SymbolTable* currentSymbolTable(){
    int index = localSymbolTableList.current;
    if(index < 0){
        printf("Local symbol table list current is pointing to -1\n");
        return NULL;
    }
    return localSymbolTableList.symTables[index];
}

void resetCurrentSymbolTable(){
    localSymbolTableList.current = -1;
}

void loadNextSymbolTable(){
    localSymbolTableList.current++;
}

void insertIntoLexSymbolTable(char* lexeme, Token tk, Datatype t){
    insert(&lexerSymbolTable, (KeyVal){lexeme, {lexeme, NULL, tk, t, NULL, NULL}});
}

SymbolVal* findType(Ast_SingleOrRecId* id){
    // SymbolVal* tdef = find(&typeDefSymbolTable, name);
    // if(tdef == NULL){
    //     SymbolVal* trdef = find(&typeRedefSymbolTable, name);
    //     return trdef != NULL ? find(&typeDefSymbolTable, trdef->to) : NULL;
    // }

    // return tdef;

}

void insertVar(char* name, Datatype datatype, char* typeName){
    KeyVal kv = (KeyVal){name, {name, NULL, 0, datatype, typeName, NULL}};
    insert(currentSymbolTable(), kv);
}

SymbolVal* findVar(char* name){
    SymbolVal* res = find(currentSymbolTable(), name);
    return res;
}

void insertTypeDef(char* name, Datatype recOrUn){
    printf("insertTypeDef %s %s\n", name, dtypeToStr(recOrUn));
    KeyVal kv = (KeyVal){name, {name, NULL, 0, recOrUn, name, NULL}};
    insert(&typeDefSymbolTable, kv);
}

void insertTypeRedef(char* name, char* to){
    printf("insertTypeRedef %s %s\n", name, to);
    KeyVal kv = (KeyVal){name, {name, to, 0, 0, name, NULL}};
    insert(&typeRedefSymbolTable, kv);
}

void insertGlobVar(char* name, Datatype t, char* typeName){
    printf("insertGlobalVar %s %s %s\n", name, dtypeToStr(t), typeName);
    KeyVal kv = (KeyVal){name, {name, NULL, 0, t, typeName, NULL}};
    insert(&globVarSymbolTable, kv);
}

void initLexerSymbolTable(){
    SymbolTable* symTable = &lexerSymbolTable;
    symTable->tb = malloc(HASHTABLE_SIZE * sizeof(TableEntry));

    initSymTable(symTable);

    //initalize keywords
    insertIntoLexSymbolTable("with", TK_WITH, DT_NONE);
    insertIntoLexSymbolTable("parameters", TK_PARAMETERS, DT_NONE);
    insertIntoLexSymbolTable("while", TK_WHILE, DT_NONE);
    insertIntoLexSymbolTable("union", TK_UNION, DT_NONE);
    insertIntoLexSymbolTable("endunion", TK_ENDUNION, DT_NONE);
    insertIntoLexSymbolTable("definetype", TK_DEFINETYPE, DT_NONE);
    insertIntoLexSymbolTable("as", TK_AS, DT_NONE);
    insertIntoLexSymbolTable("type", TK_TYPE, DT_NONE);
    insertIntoLexSymbolTable("_main", TK_MAIN, DT_NONE);
    insertIntoLexSymbolTable("global", TK_GLOBAL, DT_NONE);
    insertIntoLexSymbolTable("parameter", TK_PARAMETER, DT_NONE);
    insertIntoLexSymbolTable("list", TK_LIST, DT_NONE);
    insertIntoLexSymbolTable("input", TK_INPUT, DT_NONE);
    insertIntoLexSymbolTable("output", TK_OUTPUT, DT_NONE);
    insertIntoLexSymbolTable("int", TK_INT, DT_NONE);
    insertIntoLexSymbolTable("real", TK_REAL, DT_NONE);
    insertIntoLexSymbolTable("endwhile", TK_ENDWHILE, DT_NONE);
    insertIntoLexSymbolTable("if", TK_IF, DT_NONE);
    insertIntoLexSymbolTable("then", TK_THEN, DT_NONE);
    insertIntoLexSymbolTable("endif", TK_ENDIF, DT_NONE);
    insertIntoLexSymbolTable("read", TK_READ, DT_NONE);
    insertIntoLexSymbolTable("write", TK_WRITE, DT_NONE);
    insertIntoLexSymbolTable("return", TK_RETURN, DT_NONE);
    insertIntoLexSymbolTable("call", TK_CALL, DT_NONE);
    insertIntoLexSymbolTable("record", TK_RECORD, DT_NONE);
    insertIntoLexSymbolTable("endrecord", TK_ENDRECORD, DT_NONE);
    insertIntoLexSymbolTable("else", TK_ELSE, DT_NONE);
    insertIntoLexSymbolTable("end", TK_END, DT_NONE);
}

int hash(char* name){
    unsigned long hash = 5381;
    int c;
    while (c = *name++) 
        hash = ((hash << 5) + hash) + c; 
    hash =  hash % 500;
    return (int)hash;
}

void insert(SymbolTable* st, KeyVal kv){
    int key = hash(kv.name);
    TableEntry* t = &st->tb[key];
    LL* entry = (LL*) malloc(sizeof(LL));
    entry->kv = kv;
    entry->next = NULL;
    if(t->sz==0){
        t->head = entry;
        t->tail = entry;
        t->sz=1;
    }
    else{
        t->tail->next = entry;
        t->tail = entry;
        t->sz++;
    }
}

SymbolVal* find(SymbolTable* st, char* str){
    int key = hash(str);
    TableEntry t = st->tb[key];
    if(t.sz==0) return NULL;
    LL* curr = t.head;
    while(curr!=NULL){
        if(strcmp(curr->kv.name,str)==0) return &(curr->kv.val);
        curr = curr->next;
    }
    return NULL;
}