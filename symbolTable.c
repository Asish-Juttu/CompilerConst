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
#include "type.h"

#include "semError.h"
SymbolTable lexerSymbolTable;
SymbolTable typeDefSymbolTable;
SymbolTable typeRedefSymbolTable;
SymbolTable globVarSymbolTable;
SymbolTableList localSymbolTableList;
SymbolTable funSymbolTable; // function - symTable map

// char* allTdefs[500];
// int tIndex = 0;
int init = 0;


SymbolTable* curSymTable = NULL;

int isVoid(TypeExpression t){
    return t.basicType == BTYPE_VOID;
}
void computeType(char* name, SymbolVal* symVal){
    SymbolTable* fTab = symVal->symbolTable;
    LL* node = fTab->keys.head;
    int k = 0;
    for(int i = 0; i < fTab->keys.sz; i++){
        if(node->kv.val.type == DT_NUM){
            addToRecord(symVal->typeExpr, numTypeExpression());
        }
        else if(node->kv.val.type == DT_RNUM){
            addToRecord(symVal->typeExpr, rnumTypeExpression());
        }
        else if(node->kv.val.type == DT_UNION){
            SymbolVal* tgVal = find(fTab, "tagvalue");
            if(k){
                printf("Errror ! More than one union in variant record");
                symVal->typeExpr.basicType = BTYPE_ERROR;
            }
            else if(tgVal == NULL || (tgVal->type != DT_NUM && tgVal->type != DT_RNUM)) {
                printf("Error ! No tagvalue field for variant record %s", name);
                symVal->typeExpr.basicType = BTYPE_ERROR;
            }
            else{
                if(isVoid(node->kv.val.typeExpr)){
                    computeType(node->kv.val.typeName, findTypeDefinition(node->kv.val.typeName));
                }
                addToRecord(symVal->typeExpr, findTypeDefinition(node->kv.val.typeName)->typeExpr);
            }
            k = 1;
        }
        else if(node->kv.val.type == DT_RECORD){
                if(isVoid(node->kv.val.typeExpr)){
                    computeType(node->kv.val.typeName, findTypeDefinition(node->kv.val.typeName));
                }
                addToRecord(symVal->typeExpr, findTypeDefinition(node->kv.val.typeName)->typeExpr);
        }
    }
}

void computeTypes(){
    LL* node = typeDefSymbolTable.keys.head;
    for(int i = 0; i < typeDefSymbolTable.keys.sz; i++){
        if(isVoid(node->kv.val.typeExpr))
            computeType(node->kv.name, &node->kv.val);
        printTypeExpr(node->kv.val.typeExpr);

        node = node->next;
    }
}
KeyVal keyVal(char* name){
    return (KeyVal){name, {name, NULL, 0, 0, NULL, NULL, 0, 0, typeVoid()}};
}

void loadSymbolTable(char* funId){
    SymbolVal* sval = findFunc(funId);
    curSymTable = sval->symbolTable;
}

void initSymTable(SymbolTable* symTable){
    symTable->tb = malloc(HASHTABLE_SIZE * sizeof(TableEntry));
    for(int i = 0; i < HASHTABLE_SIZE; i++){
        symTable->tb[i].sz = 0;
        symTable->tb[i].head = NULL;
        symTable->tb[i].tail = NULL;
    }
    symTable->name = NULL;
}

void initTypeDefSymbolTable(){
    //typeDefSymbolTable.tb = malloc(HASHTABLE_SIZE * sizeof(TableEntry));
    initSymTable(&typeDefSymbolTable);
}

void initTypeRedefSymbolTable(){
    //typeRedefSymbolTable.tb = malloc(HASHTABLE_SIZE * sizeof(TableEntry));
    initSymTable(&typeRedefSymbolTable);
}

void setCurrentSymbolTable(SymbolTable* symbolTable){
    curSymTable = symbolTable;
}

void initGlobVarSymbolTable(){
    //globVarSymbolTable.tb = malloc(HASHTABLE_SIZE * sizeof(TableEntry));
    initSymTable(&globVarSymbolTable);
}

void initLocalSymbolTable(){
    localSymbolTableList.cap = 1;
    localSymbolTableList.size = 0;
    localSymbolTableList.symTables = malloc(sizeof(SymbolTable*));
}

void initFunSymbolTable(){
    initSymTable(&funSymbolTable);
}
void initGlobalSymbolTables(){
    if(init){
        free(lexerSymbolTable.tb);
        free(typeDefSymbolTable.tb);
        free(typeRedefSymbolTable.tb);
        free(globVarSymbolTable.tb);
        free(localSymbolTableList.symTables);
        free(funSymbolTable.tb);
    }
    init = 1;
    // typeDefSymbolTable.previous = NULL;
    // typeRedefSymbolTable.previous = NULL;
    // lexerSymbolTable.previous = NULL;
    // globVarSymbolTable.previous = NULL;
    //localSymbolTableList.current = -1;

    initLexerSymbolTable();
    initTypeDefSymbolTable();
    initTypeRedefSymbolTable();
    initGlobVarSymbolTable();
    initLocalSymbolTable();
    initFunSymbolTable();
}

void growSlistIfFull(SymbolTableList* list){
    if(list->size == list->cap){
        list->cap = (int)(list->cap * 1.4 + 1);
        list->symTables = realloc(list->symTables, list->cap * sizeof(SymbolTable*));
    }
}
void pushSymbolTable(char* fname){
    growSlistIfFull(&localSymbolTableList);
    SymbolTable* symTable = malloc(sizeof(SymbolTable));
    initSymTable(symTable);
    localSymbolTableList.symTables[localSymbolTableList.size] = symTable;
    localSymbolTableList.size++;
    insertFunc(fname, symTable);
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
    return curSymTable;
}

void insertIntoLexSymbolTable(char* lexeme, Token tk, Datatype t){
    KeyVal kv = keyVal(lexeme);
    kv.val.token = tk;
    kv.val.type = t;

    insert(&lexerSymbolTable, kv);
}

SymbolVal* findTypeDefinition(char* name){
    SymbolVal* tdef = find(&typeDefSymbolTable, name);
    if(tdef == NULL){
        SymbolVal* trdef = find(&typeRedefSymbolTable, name);
        if(trdef == NULL) return NULL;
        return find(&typeDefSymbolTable, trdef->to);
    }

    return tdef;
}

// if returns null => no such variable
SymbolVal* findType(Ast_SingleOrRecId* id){
    SymbolVal* firstId = findVar(id->id);
    if(firstId == NULL) return NULL;

    AstList* list = id->fieldNameList;
    if(list->size == 0)
        return firstId;
    
    SymbolVal* fVal = NULL;
    char* typeName = firstId->typeName;
    for(int i = 0; i < list->size; i++){
        if(typeName == NULL){
            return NULL;
        }
        fVal = findTypeDefinition(typeName);
        char* id = nodeToAst(list->nodes[i], id)->id;
        fVal = find(fVal->symbolTable, id);
        typeName = fVal->typeName;
    }
    
    return fVal;
}

void insertVar(char* name, Datatype datatype, char* typeName){
    KeyVal kv = keyVal(name);
    kv.val.type = datatype;
    kv.val.typeName = typeName;

    insert(currentSymbolTable(), kv);
}

SymbolVal* findVar(char* name){
    SymbolVal* res = find(currentSymbolTable(), name);
    return res;
}

void insertFunc(char* name, SymbolTable* symbolTable){
    KeyVal kv = keyVal(name);
    kv.val.symbolTable = symbolTable;
    
    insert(&funSymbolTable, kv);
}

SymbolVal* findFunc(char* name){
    return find(&funSymbolTable, name);
}

void insertTypeDef(char* name, Datatype recOrUn, Ast_FieldDefinitions* fieldDefs){
    printf("insertTypeDef %s %s\n", name, dtypeToStr(recOrUn));
    KeyVal kv = keyVal(name);
    kv.val.type = recOrUn;

    AstList* list = fieldDefs->fieldDefinitionList;
    SymbolTable* flistSymTab = malloc(sizeof(SymbolTable));
    initSymTable(flistSymTab);
    for(int i = 0; i < list->size; i++){
        Ast_FieldDefinition* fdef = nodeToAst(list->nodes[i], fieldDefinition);

        KeyVal kv = keyVal(fdef->id);
        kv.val.type = fdef->fieldType->datatype;
        kv.val.typeName = fdef->fieldType->name;

        insert(flistSymTab, kv);
    }

    kv.val.symbolTable = flistSymTab;
    insert(&typeDefSymbolTable, kv);
    // allTdefs[tIndex] = name;
    // tIndex++;
}

void insertTypeRedef(char* name, char* to){
    printf("insertTypeRedef %s %s\n", name, to);
    KeyVal kv = keyVal(name);
    kv.val.to = to;

    insert(&typeRedefSymbolTable, kv);
}

void insertGlobVar(char* name, Datatype t, char* typeName){
    printf("insertGlobalVar %s %s %s\n", name, dtypeToStr(t), typeName);
    KeyVal kv = keyVal(name);
    kv.val.type = t;
    kv.val.typeName = typeName;

    insert(&globVarSymbolTable, kv);
}

void initLexerSymbolTable(){
    SymbolTable* symTable = &lexerSymbolTable;
    //symTable->tb = malloc(HASHTABLE_SIZE * sizeof(TableEntry));

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
    
    if(st == NULL){
        printf("ERROR sym table null while trying to insert %s", kv.name);
        return;
    }
    int key = hash(kv.name);
    TableEntry* t = &st->tb[key];
    if(t == NULL){
        printf("ERROR sym tableEntry null while trying to insert %s", kv.name);
        return;
    }
    LL* entry = (LL*) malloc(sizeof(LL));
    entry->kv = kv;
    entry->next = NULL;
    if(t->sz==0){
        t->head = entry;
        t->tail = entry;
        t->sz=1;

        st->keys.head = entry;
        st->keys.tail = entry;
        st->keys.sz = 1;
    }
    else{
        t->tail->next = entry;
        t->tail = entry;
        t->sz++;

        st->keys.tail->next = entry;
        st->keys.tail = entry;
        st->keys.sz++;
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