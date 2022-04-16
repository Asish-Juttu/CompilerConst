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

SymbolTable symbolTable;

void initLexerSymbolTable(SymbolTable* symTable){
    symTable->tb = malloc(HASHTABLE_SIZE * sizeof(TableEntry));

    for(int i = 0; i < HASHTABLE_SIZE; i++){
        symTable->tb[i].sz = 0;
        symTable->tb[i].head = NULL;
        symTable->tb[i].tail = NULL;
    }

    //initalize keywords
    insert(symTable, (KeyVal){"with", {"with", TK_WITH, NONE}});
    insert(symTable, (KeyVal){"parameters", {"parameters", TK_PARAMETERS, NONE}});
    insert(symTable, (KeyVal){"while", {"while", TK_WHILE, NONE}});
    insert(symTable, (KeyVal){"union", {"union", TK_UNION, NONE}});
    insert(symTable, (KeyVal){"endunion", {"endunion", TK_ENDUNION, NONE}});
    insert(symTable, (KeyVal){"definetype", {"definetype", TK_DEFINETYPE, NONE}});
    insert(symTable, (KeyVal){"as", {"as", TK_AS, NONE}});
    insert(symTable, (KeyVal){"type", {"type", TK_TYPE, NONE}});
    insert(symTable, (KeyVal){"_main", {"_main", TK_MAIN, NONE}});
    insert(symTable, (KeyVal){"global", {"global", TK_GLOBAL, NONE}});
    insert(symTable, (KeyVal){"parameter", {"parameter", TK_PARAMETER, NONE}});
    insert(symTable, (KeyVal){"list", {"list", TK_LIST, NONE}});
    insert(symTable, (KeyVal){"input", {"input", TK_INPUT, NONE}});
    insert(symTable, (KeyVal){"output", {"output", TK_OUTPUT, NONE}});
    insert(symTable, (KeyVal){"int", {"int", TK_INT, NONE}});
    insert(symTable, (KeyVal){"real", {"real", TK_REAL, NONE}});
    insert(symTable, (KeyVal){"endwhile", {"endwhile", TK_ENDWHILE, NONE}});
    insert(symTable, (KeyVal){"if", {"if", TK_IF, NONE}});
    insert(symTable, (KeyVal){"then", {"then", TK_THEN, NONE}});
    insert(symTable, (KeyVal){"endif", {"endif", TK_ENDIF, NONE}});
    insert(symTable, (KeyVal){"read", {"read", TK_READ, NONE}});
    insert(symTable, (KeyVal){"write", {"write", TK_WRITE, NONE}});
    insert(symTable, (KeyVal){"return", {"return", TK_RETURN, NONE}});
    insert(symTable, (KeyVal){"call", {"call", TK_CALL, NONE}});
    insert(symTable, (KeyVal){"record", {"record", TK_RECORD, NONE}});
    insert(symTable, (KeyVal){"endrecord", {"endrecord", TK_ENDRECORD, NONE}});
    insert(symTable, (KeyVal){"else", {"else", TK_ELSE, NONE}});
    insert(symTable, (KeyVal){"end", {"end", TK_END, NONE}});
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