#include "parserDef.h"
#include "parser.h"
#include "token.h"
#include <stdio.h>
#include "lexer.h"
#include "symbolTable.h"

int main(){
    Grammar grammar;
    initGrammar(&grammar);
    initSymbolTable(&symbolTable);
    FirstAndFollow fnf;
    initFirstAndFollow(&fnf, &grammar);
    ParseTable ptable;
    initParseTable(&grammar, &fnf, &ptable);
    TwinBuffer tbuf;
    initTwinBuffer(&tbuf, "testcases/t6.txt");

    ParseTree parse = initParseTree(&grammar, &ptable, &tbuf);
    printf("Hello");
    // Inorder(parse.head);
    return 0;
}