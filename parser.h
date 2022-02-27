#ifndef _PARSER_
#define _PARSER_

#include "token.h"
void initGrammar(Grammar* grammar);
TokenSet first(Grammar* grammar, NonTerminal nt);
TokenSet follow(Grammar* grammar, NonTerminal nt)
Token* toArray(TokenSet tSet);
#endif