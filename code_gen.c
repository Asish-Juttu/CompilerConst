#include "code_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initStringList(StringList* list){
    list->cap = 1;
    list->size = 0;
    list->code = malloc(sizeof(char*));
}

void growIfFull(StringList* list){
    if(list->cap == list->size){
        list->cap = (int)(list->cap * 1.4 + 1);
        list->code = realloc(list->code, list->cap * sizeof(char*));
    }
}
void append(StringList* list, char* str){
    growIfFull(list);
    list->code[list->size] = str;
    list->size ++;
}

void genProgram(Ast_Program* program){
    //Declare global variables

    
}