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
void Inorder(ParseTreeElement* head, FILE* fptr);
int isEmpty(Stack* head);
void initFirstAndFollow(FirstAndFollow* firstNFolllow, Grammar* grammar);
ParseTree parse(char* file);
#endif