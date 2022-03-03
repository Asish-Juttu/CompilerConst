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