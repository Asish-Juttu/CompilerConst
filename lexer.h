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
