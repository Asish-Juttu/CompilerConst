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

#ifndef _LEXER_
#define _LEXER_
#include "token.h"
#include "twinBuffer.h"

typedef struct{
    TokenInfo* tokenInf;
    int size;
    int capacity;
} TokenInfoArray;

TokenInfo getNextToken(TwinBuffer* tbuf);
void removeComments(char* testCaseFile, char* cleanFile);
TokenInfoArray tokenize(char* file);
#endif
