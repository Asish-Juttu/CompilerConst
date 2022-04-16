#include "semError.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ErrorList errorList;
int init = 0;

void addError(char* msg){
    if(errorList.size == errorList.cap){
        errorList.cap = (int)(errorList.cap * 1.4 + 1);
        errorList.errors = realloc(errorList.errors, errorList.cap * sizeof(char*));
    }
    errorList.errors[errorList.size] = msg;
    errorList.size++;
}

void initErrorList(){
    errorList.cap = 1;
    errorList.size = 0;
    if(init) free(errorList.errors);
    init = 1;
    errorList.errors = malloc(sizeof(char*));
}

void error(char* msg, int lineNo){
    char* emsg = malloc(sizeof(char) * (strlen(msg) + 50));
    sprintf(emsg, "In Line %d, %s", lineNo, msg);
    addError(emsg);
}

void reportErrors(){
    for(int i = 0; i < errorList.size; i++){
        printf("%s\n", errorList.errors[i]);
    }
}
