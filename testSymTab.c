#include "symbolTable.h"
#include <stdio.h>

int main(){
    insert(&symbolTable, (KeyVal){"Hello", {"Hello", TK_AND, NONE}});
    SymbolVal* symInfo = find(&symbolTable, "Hello");
    printf("Is Null %d", (symInfo == NULL));
    return 1;
}