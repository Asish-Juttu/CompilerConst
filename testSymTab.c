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
#include "symbolTable.h"
#include <stdio.h>

int main(){
    insert(&lexerSymbolTable, (KeyVal){"Hello", {"Hello", TK_AND, NONE}});
    SymbolVal* symInfo = find(&lexerSymbolTable, "Hello");
    printf("Is Null %d", (symInfo == NULL));
    return 1;
}