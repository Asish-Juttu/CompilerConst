#include "lexer.h"
#include "twinBuffer.h"

int main(){
    TwinBuffer tb;
    TwinBuffer* tbuf = &tb;

    initTwinBuffer(tbuf, "testcases/t1.txt");
    TokenInfo tinf;
    while((tinf = getNextToken(tbuf)).token != EPSILON){
        printf("[TOKEN %s]\n", tokToStr(tinf.token));
    }
    return 1;
}