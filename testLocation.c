#include "parserDef.h"
#include "parser.h"
#include "token.h"
#include <stdio.h>

int main(){
    Grammar grammar;
    initGrammar(&grammar);

    LocationArray* locArr = getLocationArray();
    for(int i = 0; i < NON_TERMINAL_SIZE; i++){
        printf("%s ", nonTermToStr(i));
        printf("----------------------\n");
        for(int j = 0; j < locArr[i].size; j++){
            Location loc = locArr[i].location[j];
            printf("Non Term = %s, Rule Num = %d, Pos = %d \n", nonTermToStr(loc.nt), loc.ruleNo, loc.index);
        }
        printf("\n");
    }
    return 0;
}