#ifndef _PARSER_
#define _PARSER_

#include "token.h"
#include "parserDef.h"
void initGrammar(Grammar* grammar);
TokenSet first(Grammar* grammar, NonTerminal nt);
TokenSet follow(Grammar* grammar, NonTerminal nt);
Token* toArray(TokenSet tSet);
ParseTable initParseTable(Grammar* grammar,FirstAndFollow* f);
void initFirstAndFollow(FirstAndFollow* firstNFolllow, Grammar* grammar);
#endif