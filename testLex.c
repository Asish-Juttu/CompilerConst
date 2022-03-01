#include "lexer.h"
#include "twinBuffer.h"
#include "symbolTable.h"
int main(){
    TwinBuffer tb;
    TwinBuffer* tbuf = &tb;

    initTwinBuffer(tbuf, "testcases/t1.txt");
    initSymbolTable(&symbolTable);
    TokenInfo tinf;
    while((tinf = getNextToken(tbuf)).token != EPSILON){
        if(tinf.token != ERROR_TOKEN)
            printf("Line Number : %d \t Lexeme : %s \t Token : %s\n", tinf.lineNumber, tinf.lexeme, tokToStr(tinf.token));
        else
            printf("Line Number : %d Error : Unknow pattern %s\n", tinf.lineNumber, tinf.lexeme);
    }
    return 1;
}