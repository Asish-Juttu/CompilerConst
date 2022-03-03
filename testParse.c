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

int main(){
    Grammar grammar;
    initGrammar(&grammar);

    FirstAndFollow fnf;
    initFirstAndFollow(&fnf, &grammar);
    ParseTable ptable;
    initParseTable(&grammar, &fnf, &ptable);
    // printf("Hello");
    // for(int i = 0; i < NON_TERMINAL_SIZE; i++){
    //     for(int j = 0; j < TOKEN_SIZE; j++){
    //         Rule rule = ptable.table[i][j];
    //         if(rule.symbol != NULL){
    //             printf("%s , %s (%d) ==> ", nonTermToStr(i), tokToStr(j), rule.size);
    //             printSymbols(rule.symbol, rule.size);
    //             printf("\n");
    //         }
    //     }
    // }

    TokenInfoArray tArray = tokenize("testcases/t3.txt");
    ParseTree parse = initParseTree(&grammar, &ptable,tArray.tokenInf, tArray.size);
    //printf("Hello");
    Inorder(parse.head);
    return 0;
}