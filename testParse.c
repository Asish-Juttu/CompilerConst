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

#include "parserDef.h"
#include "parser.h"
#include "token.h"
#include <stdio.h>
#include "lexer.h"
#include "symbolTable.h"

int main(){
    Grammar grammar;
    initGrammar(&grammar);
    initLexerSymbolTable(&symbolTable);
    FirstAndFollow fnf;
    initFirstAndFollow(&fnf, &grammar);
    ParseTable ptable;
    initParseTable(&grammar, &fnf, &ptable);
    TwinBuffer tbuf;
    initTwinBuffer(&tbuf, "testcases/t2.txt");
    ParseTree parse = initParseTree(&grammar, &ptable, &tbuf);
    // printf("Hello");
   // Inorder(parse.head);

    return 0;
}