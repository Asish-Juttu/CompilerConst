#ifndef _PARSER_
#define _PARSER_

#include "token.h"
#include "parserDef.h"
#include "twinBuffer.h"


void initGrammar(Grammar* grammar);
TokenSet first(Grammar* grammar, NonTerminal nt);
TokenSet follow(Grammar* grammar, NonTerminal nt);
Token* toArray(TokenSet tSet);

void initParseTable(Grammar* grammar,FirstAndFollow* f, ParseTable* ptable);
ParseTree initParseTree(Grammar* grammar, ParseTable* parseTable, TwinBuffer* tbuf);
Stack* stackPush(Stack* head,ParseTreeElement* e);
Stack* stackPop(Stack* head);
ParseTreeElement* stackTop(Stack* head);
void Inorder(ParseTreeElement* head);
int isEmpty(Stack* head);
void initFirstAndFollow(FirstAndFollow* firstNFolllow, Grammar* grammar);
#endif