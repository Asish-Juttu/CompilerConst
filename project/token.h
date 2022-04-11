#ifndef _TOKEN_
#define _TOKEN_

#define TOKEN_SIZE 56

enum Token{
    TK_ASSIGNOP, TK_OP, TK_AND, TK_AS, TK_CALL, TK_CL, TK_COLON, TK_COMMA, TK_COMMENT, TK_DEFINETYPE, TK_DIV, TK_DOT, TK_ELSE, TK_END, TK_ENDIF, TK_ENDRECORD, 
    TK_ENDUNION, TK_ENDWHILE, TK_FIELDID, TK_FUNID, TK_GLOBAL, TK_GT, TK_ID, TK_IF, TK_INPUT, TK_INT, TK_LE, TK_LIST, TK_LT, TK_MAIN, TK_MINUS, TK_MUL, TK_NE,
    TK_NOT, TK_NUM, TK_OR, TK_OUTPUT, TK_PARAMETER, TK_PARAMETERS, TK_PLUS, TK_READ, TK_REAL, TK_RECORD, TK_RETURN, TK_RNUM, TK_RUID, TK_SEM, TK_SQL, TK_SQR, 
    TK_THEN, TK_TYPE, TK_UNION, TK_WHILE, TK_WITH, TK_WRITE, EPSILON
};

typedef struct {
    Token token;
    char* lexeme;
    int lineNumber;
} TokenInfo;

typedef struct{
    unsigned int bitMask : TOKEN_SIZE;
} TokenSet;

TokenSet singletonTokenSet(Token t);
TokenSet nullTokenSet();
TokenSet tokenSetUnion(TokenSet t1, TokenSet t2);
#endif