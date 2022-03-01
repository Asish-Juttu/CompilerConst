#ifndef _LEXER_
#define _LEXER_
#include "token.h"
#include "twinBuffer.h"

TokenInfo getNextToken(TwinBuffer* tbuf);
void removeComments(char* testCaseFile, char* cleanFile);

#endif
