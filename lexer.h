#ifndef _LEXER_
#define _LEXER_

TokenInfo getNextToken(TwinBuffer* tbuf);
void removeComments(char* testCaseFile, char* cleanFile);

#endif
