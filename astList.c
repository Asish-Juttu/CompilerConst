#include "astList.h"
#include <stdlib.h>

AstList* createAstList(){
    AstList* astList = (AstList*)malloc(sizeof(AstList));
    astList->cap = 1;
    astList->size = 0;

    astList->nodes = (AstNode**)malloc(sizeof(AstNode*));

    return astList;
}

void insertTo(AstList* list, AstNode* node){
    if(list->cap == list->size){
        list->cap = (int)(1.4 * list->cap + 1);
        list->nodes = (AstNode**)realloc(list->nodes, list->cap * sizeof(AstNode*));
    }

    list->nodes[list->size] = node;
    list->size++;
}