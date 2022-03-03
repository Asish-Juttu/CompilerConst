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

#include "token.h"
TokenSet singletonTokenSet(Token t){
    TokenSet tset;
    tset.bitMask = (1ULL << t);

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

int tokenSetContains(TokenSet tSet, Token token){
    return ( tSet.bitMask >> token ) & 1ULL;
}
