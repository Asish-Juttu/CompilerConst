#include "symbolTable.h"
#include <string.h>
SymbolTable st;
for(int i=0;i<500;i++){
    st.tb[i].sz = 0;
    st.tb[i].head = NULL;
    st.tb[i].tail = NULL;
}

int hash(char* name){
unsigned long hash = 5381;
int c;
while (c = *name++) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
hash =  hash % 500;
return (int)hash;
}

void insert(KeyVal kv){
int key = hash(kv.name);
TableEntry t = st.tb[key];
LL* entry = (LL*) malloc(sizeof(LL));
entry->kv = kv;
entry->next = NULL;
if(t.sz==0){
    t.head = entry;
    t.tail = entry;
    t.sz=1;
}
else{
    t.tail->next = entry;
    t.tail = entry;
    t.sz++;
}
}

SymbolVal find(char* str){
int key = hash(name);
TableEntry t = st.tb[key];
if(t.sz==0) return NULL;
LL* curr = t.head;
while(curr!=NULL){
if(strcmp(curr->kv.name,str)==0) return curr->kv.val;
curr = curr->next;
}
return NULL;
}