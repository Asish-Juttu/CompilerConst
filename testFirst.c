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

int main(){
    Grammar grammar;
    initGrammar(&grammar);
    printf("fknsdljan");
    for(int i = 0; i < NON_TERMINAL_SIZE; i++){
        printf("%s [%d Rules]===> ", nonTermToStr(i), grammar.ruleArray[i].size);
        for(int j = 0; j < grammar.ruleArray[i].size; j++){
            for(int k = 0; k < grammar.ruleArray[i].rule[j].size; k++){
                Symbol sym = grammar.ruleArray[i].rule[j].symbol[k];
                if(sym.isTerminal){
                    printf("%s(%d) ", tokToStr(sym.symbol), sym.symbol);
                }
                else{
                    printf("%s ", nonTermToStr(sym.symbol));
                }
            }
            printf(" | ");
        }
        printf("\n");
    }
    
    FirstAndFollow fnf;
    initFirstAndFollow(&fnf, &grammar);

    for(int i = 0; i < NON_TERMINAL_SIZE; i++){
        printf("FIRST(%s) = {", nonTermToStr(i));
        for(int j = 0; j < fnf.first[i].size; j++){
            printf("%s ", tokToStr(fnf.first[i].elements[j].t));
        }
        printf("}\n");
    }
    return 0;
}