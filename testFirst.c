#include "parserDef.h"
#include "parser.h"
#include "token.h"
#include <stdio.h>

int main(){
    Grammar grammar;
    initGrammar(&grammar);
    //printf("fknsdljan");
    for(int i = 0; i < NON_TERMINAL_SIZE; i++){
        printf("%s ===> ", nonTermToStr(i));
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
    
    printf("FINISHED  ");
    return 0;
}