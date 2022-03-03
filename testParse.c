#include "parserDef.h"
#include "parser.h"
#include "token.h"
#include <stdio.h>


int main(){
    Grammar grammar;
    initGrammar(&grammar);

    FirstAndFollow fnf;
    initFirstAndFollow(&fnf, &grammar);
    ParseTable ptable = initParseTable(&grammar, &fnf);
    // printf("Hello");
    // for(int i = 0; i < NON_TERMINAL_SIZE; i++){
    //     for(int j = 0; j < TOKEN_SIZE; j++){
    //         Rule rule = ptable.table[i][j];
    //         if(rule.symbol != NULL){
    //             printf("%s , %s ==> ");
    //             printSymbols(rule.symbol, rule.size);
    //             printf("\n");
    //         }
    //     }
    // }
    return 0;
}