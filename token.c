#include "token.h"
TokenSet singletonTokenSet(Token t){
    TokenSet tset;
    tset.bitMask = (1 << t);

    return tset;
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

int equalsTokenSet(TokenSet t1, TokenSet t2){
    return t1.bitMask == t2.bitMask;
}

