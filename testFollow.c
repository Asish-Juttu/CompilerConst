#include "parserDef.h"
#include "parser.h"
#include "token.h"
#include <stdio.h>

int main(){
    Grammar grammar;
    initGrammar(&grammar);

    FirstAndFollow fnf;

    FollowHelperSet helper[NON_TERMINAL_SIZE];

    initFirstAndFollow(&fnf, &grammar);
    for(int i = 0; i < NON_TERMINAL_SIZE; i++){
        printf("\nTerminals in FOLLOW(%s) \n", nonTermToStr(i));
        printf("---------------------------\n");

        FollowHelperSet fhset = followHelper(&grammar, i);
        helper[i] = fhset;
        printTSet(fhset.tSet);

        printf("Non Terminal Dependencies => \n");
        printNTSet(fhset.ntSet);
        printf("\n");
    }
    
    int k = 0;
    while(1){
        int changed = 0;
        for(int i = 0; i < NON_TERMINAL_SIZE; i++){
            for(int j = 0; j < NON_TERMINAL_SIZE; j++){
                if(nonTermSetContains(helper[i].ntSet, j)){
                    TokenSet old = helper[i].tSet;
                    helper[i].tSet = tokenSetUnion(helper[i].tSet, helper[j].tSet);
                    changed |= !equalsTokenSet(old, helper[i].tSet);
                }
            }
        }
        k++;
        if(!changed)
            break;
    }
    printf("%d 9999999999999999 ", k);
    for(int i = 0; i < NON_TERMINAL_SIZE; i++){
        printf("FOLLOW(%s)\n", nonTermToStr(i));
        printf("==============================\n");


        for(int j = 0; j < TOKEN_SIZE; j++){
            if(tokenSetContains(helper[i].tSet, j)){
                printf("%s ", tokToStr(j));
            }   
        }

        printf("\n\n");
    }
    return 0;
}