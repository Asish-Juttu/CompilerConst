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

char* tokToStr(Token t){
    switch(t){
        case  TK_ASSIGNOP: return "TK_ASSIGNOP";
        case  TK_OP: return "TK_OP";
        case  TK_AND: return "TK_AND";
        case  TK_AS: return "TK_AS";
        case  TK_CALL: return "TK_CALL";
        case  TK_CL: return "TK_CL";
        case  TK_COLON: return "TK_COLON";
        case  TK_COMMA: return "TK_COMMA";
        case  TK_COMMENT: return "TK_COMMENT";
        case  TK_DEFINETYPE: return "TK_DEFINETYPE";
        case  TK_DIV: return "TK_DIV";
        case  TK_DOT: return "TK_DOT";
        case  TK_ELSE: return "TK_ELSE";
        case  TK_END: return "TK_END";
        case  TK_ENDIF: return "TK_ENDIF";
        case  TK_ENDRECORD: return "TK_ENDRECORD";
        case  TK_ENDUNION: return "TK_ENDUNION";
        case  TK_ENDWHILE: return "TK_ENDWHILE";
        case  TK_FIELDID: return "TK_FIELDID";
        case  TK_FUNID: return "TK_FUNID";
        case  TK_GLOBAL: return "TK_GLOBAL";
        case  TK_GT: return "TK_GT";
        case  TK_ID: return "TK_ID";
        case  TK_IF: return "TK_IF";
        case  TK_INPUT: return "TK_INPUT";
        case  TK_INT: return "TK_INT";
        case  TK_LE: return "TK_LE";
        case  TK_LIST: return "TK_LIST";
        case  TK_LT: return "TK_LT";
        case  TK_MAIN: return "TK_MAIN";
        case  TK_MINUS: return "TK_MINUS";
        case  TK_MUL: return "TK_MUL";
        case  TK_NE: return "TK_NE";
        case  TK_NOT: return "TK_NOT";
        case  TK_NUM: return "TK_NUM";
        case  TK_OR: return "TK_OR";
        case  TK_OUTPUT: return "TK_OUTPUT";
        case  TK_PARAMETER: return "TK_PARAMETER";
        case  TK_PARAMETERS: return "TK_PARAMETERS";
        case  TK_PLUS: return "TK_PLUS";
        case  TK_READ: return "TK_READ";
        case  TK_REAL: return "TK_REAL";
        case  TK_RECORD: return "TK_RECORD";
        case  TK_RETURN: return "TK_RETURN";
        case  TK_RNUM: return "TK_RNUM";
        case  TK_RUID: return "TK_RUID";
        case  TK_SEM: return "TK_SEM";
        case  TK_SQL: return "TK_SQL";
        case  TK_SQR: return "TK_SQR";
        case  TK_THEN: return "TK_THEN";
        case  TK_TYPE: return "TK_TYPE";
        case  TK_UNION: return "TK_UNION";
        case  TK_WHILE: return "TK_WHILE";
        case  TK_WITH: return "TK_WITH";
        case  TK_WRITE: return "TK_WRITE";
        case  EPSILON: return "EPSILON";
        case  TK_GE: return "TK_GE";
        case  TK_EQ: return "TK_EQ";
    }

    return "UNKNOWN";
} 