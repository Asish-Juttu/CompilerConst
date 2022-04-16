#ifndef _SEM_ERROR_

typedef struct {
    char** errors;
    int size;
    int cap;
} ErrorList;

extern ErrorList errorList;
void error(char* msg, int lineNo);
void initErrorList();
void reportErrors();

#define _SEM_ERROR_
#endif