#include "token.h"
TokenSet singletonTokenSet(Token t){
    TokenSet t;
    t.bitMask = (1 << t);

    return t;
}

TokenSet nullTokenSet(){
    TokenSet t;
    t.bitMask = 0;
    return t;
}

TokenSet tokenSetUnion(TokenSet t1, TokenSet t2){
    TokenSet t3;
    t3.bitMask = t1.bitMask | t2.bitMask;

    return t3;
}