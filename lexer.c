#include "twinBuffer.h"
#include "lexer.h"
#include "symbolTable.h"
#include <stdlib.h>
#include <string.h>


int isDigit(char ch){
    return ch >= '0' && ch <= '9';
}

int toDigit(char ch){
    return ch - '0';
}

int isFieldID_1(char ch){
    return (ch=='a' || (ch >= 'e' && ch <= 'z'));
}

int isFieldID_2(char ch){
    return (ch >= 'a' && ch <= 'z');
}

int isFunID(char ch){
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

int isID(char ch){
    return (ch >= 'b' && ch <= 'd');
}

int num_1(char ch){
    return (ch >= '2' && ch <= '7');
}

int num_2(char ch){
    return (ch >= '0' && ch <= '9');
}

int line=1;

TokenInfo keyWordOrFieldId(char* lexeme, int lineNum){
    SymbolVal* symInfo = find(&symbolTable, lexeme);

    Token token = TK_FIELDID;

    if(symInfo == NULL){
        insert(&symbolTable, (KeyVal){lexeme, {lexeme, TK_FIELDID, NONE}});
        token = TK_FIELDID;
    }

    else if(!(symInfo->token == TK_FIELDID)){
        token = symInfo->token;
    }
    return (TokenInfo) {token, lexeme, lineNum}; // Lexeme and look-up table
}

TokenInfo keyWordOrFunId(char* lexeme, int lineNum){
    
    SymbolVal* symInfo = find(&symbolTable, lexeme);

    Token token = TK_FUNID;

    if(symInfo == NULL){
        insert(&symbolTable, (KeyVal){lexeme, {lexeme, TK_FUNID, NONE}});
        token = TK_FUNID;
    }

    else if(!(symInfo->token == TK_FUNID)){
        token = symInfo->token;
    }
    return (TokenInfo) {token, lexeme, lineNum}; // Lexeme and look-up table
}

TokenInfo getId(char* lexeme, int lineNum){
    insert(&symbolTable, (KeyVal){lexeme, {lexeme, TK_FUNID, NONE}});
    return (TokenInfo) {TK_ID, lexeme, lineNum};
}

void lexicalError(TwinBuffer* tbuf){
    char* lexeme;
    resetBegin(tbuf, 0, &lexeme);
    printf("Unknown pattern %s", lexeme);
    free(lexeme);
}
TokenInfo getNextToken(TwinBuffer* tbuf){

    char ch;
    nextChar(tbuf, &ch);
    while(1){
        if(ch==' ' || ch == '\t'){
            resetBegin(tbuf, 0, NULL);
            nextChar(tbuf, &ch);
        }
        else if(ch=='\n'){
            line++;
            resetBegin(tbuf, 0, NULL);
            nextChar(tbuf, &ch);
        }
        else break;
    }

    int st = 0;
    int num = 0;
    while(ch != EOF_CHAR){
        //LOG("(%c)", ch);
        switch(st){
            case 0: 
                if(ch=='<'){
                    st = 1;
                }
                else if(ch=='%'){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    while(ch != '\n'){
                        nextChar(tbuf, &ch);
                        char* lexeme;
                        resetBegin(tbuf,0,NULL);
                    }
                    
                    if(ch=='\n'){
                        line++;
                        resetBegin(tbuf, 0, NULL);
                        //nextChar(tbuf, &ch);
                    }
                   
                }
                else if(ch=='\n'){
                    line++;
                    resetBegin(tbuf,0,NULL);
                }
                else if(ch == '\t'){
                    resetBegin(tbuf,0,NULL);
                }
                else if(isFieldID_1(ch)){
                    st = 8;
                }
                else if(isID(ch)){
                    st = 10;
                }
                else if(num_2(ch)){
                    st = 15;
                }
                else if(ch=='>'){
                    st = 27;
                }
                else if(ch=='_'){
                    st = 30;
                }
                else if(ch=='='){
                    st = 34;
                }
                else if(ch=='#'){
                    st = 36;
                }
                else if(ch == '['){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_SQL,lexeme,line};
                }
                else if(ch == ']'){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_SQR,lexeme,line};
                }
                else if(ch==','){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_COMMA,lexeme,line};
                }
                else if(ch==';'){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_SEM,lexeme,line};
                }
                else if(ch==':'){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_COLON,lexeme,line};
                }
                else if(ch=='.'){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_DOT,lexeme,line};
                }
                else if(ch==')'){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_CL,lexeme,line};
                }
                else if(ch=='('){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_OP,lexeme,line};
                }
                else if(ch=='+'){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_PLUS,lexeme,line};
                }
                else if(ch=='-'){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_MINUS,lexeme,line};
                }
                else if(ch=='*'){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_MUL,lexeme,line};
                }
                else if(ch=='/'){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_DIV,lexeme,line};
                }
                else if(ch=='&'){
                    st = 51;
                }
                else if(ch=='@'){
                    st = 54;
                }
                else if(ch=='~'){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_NOT,lexeme,line};
                }
                else if(ch=='!'){
                    st = 58;
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN,lexeme,line};
                }
            break;

            case 1:
                if(ch == '-'){
                    st = 2;
                }
                else if(ch == '='){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_LE,lexeme,line};
                }
                else {
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {TK_LT,lexeme,line};
                }
            break;

            case 2 :
                if(ch == '-'){
                    st = 3;
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN,lexeme,line};
                }
            break;

            case 3:
                if(ch == '-'){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_ASSIGNOP,lexeme,line};
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN,lexeme,line};
                }
            break;

            case 8:
                if(isFieldID_2(ch)){
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    TokenInfo tinf = keyWordOrFieldId(lexeme, line);
                    return tinf;
                }
            break;

            case 10:
                if(isFieldID_2(ch)){
                    st = 8;
                }
                else if(num_1(ch)){
                    st = 11;
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    TokenInfo tinf = keyWordOrFieldId(lexeme, line);
                    return tinf;
                }
            break;

            case 11:
                if(isID(ch)){
                }
                else if(num_1(ch)){
                    st = 12;
                }
                else {
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    TokenInfo tinf = getId(lexeme, line);
                    
                    return tinf;
                }
            break;

            case 12:
                if(num_1(ch)){
                }
                else {
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return getId(lexeme, line);
                }
            break;

            case 15:
                if(num_2(ch)){
                }
                else if(ch == '.'){
                    st = 17;
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {TK_NUM, lexeme, line}; // Lexeme 
                }
            break;

            case 17:
                if(num_2(ch)){
                    st = 18;
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,2,&lexeme);
                    return (TokenInfo) {TK_NUM, lexeme, line}; // Lexeme 
                }
            break;

            case 18:
                if(num_2(ch)){
                    st = 19;
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN, lexeme, line}; // Lexeme 
                }
            break;

            case 19:
                if(ch=='E'){
                    st = 23;
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {TK_RNUM, lexeme, line}; // Lexeme 
                }
            break;

            case 23:
                if(num_2(ch)){
                    st = 25;
                }
                else if(ch=='+' || ch=='-'){
                    st = 24;
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN,lexeme,line};
                }
            break;

            case 24:
                if(num_2(ch)){
                    st = 25;
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN,lexeme,line};
                }
            break;

            case 25:
                if(num_2(ch)){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_RNUM, lexeme, line}; // Lexeme 
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN,lexeme,line};
                }
            break;

            case 27:
                if(ch=='='){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_GE,lexeme,line};
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {TK_GT,lexeme,line};
                }
            break;

            case 30:
                if(isFunID(ch)){
                    st = 31;
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN,lexeme,line};
                }
            break;

            case 31:
                if(isFunID(ch)){
                }
                else if(num_2(ch)){
                    st = 32;
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    TokenInfo tinf = keyWordOrFunId(lexeme, line);
                    return tinf;// Lexeme and look-up table
                }
            break;

            case 32:
                if(num_2(ch)){
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    TokenInfo tinf = keyWordOrFunId(lexeme, line);

                    return tinf;// Lexeme and look-up table
                }
            break;

            case 34:
                if(ch=='='){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_EQ,lexeme,line};
                }
                else {
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN,lexeme,line};
                }
            break;

            case 36:
                if(isFieldID_2(ch)){
                    st = 37;
                }
                else {
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN,lexeme,line};
                }
            break;

            case 37:
                if(isFieldID_2(ch)){
                }
                else {
                    char* lexeme ;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {TK_RUID, lexeme, line}; // Lexeme
                }
            break;

            case 51:
                if(ch=='&'){
                    st = 52;
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN,lexeme,line};
                }
            break;

            case 52:
                if(ch=='&'){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_AND,lexeme,line}; 
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN,lexeme,line};
                }
            break;

            case 54:
                if(ch=='@'){
                    st = 55;
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN,lexeme,line};
                }
            break;

            case 55:
                if(ch=='@'){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_OR,lexeme,line}; 
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN,lexeme,line};
                }
            break;

            case 58:
                if(ch=='='){
                    char* lexeme;
                    resetBegin(tbuf,0,&lexeme);
                    return (TokenInfo) {TK_NE,lexeme,line}; 
                }
                else{
                    char* lexeme;
                    resetBegin(tbuf,1,&lexeme);
                    return (TokenInfo) {ERROR_TOKEN,lexeme,line};
                }
            break;
        }
        nextChar(tbuf, &ch);
    }

    
    return (TokenInfo){EPSILON,NULL,line};
}

void insertTinf(TokenInfoArray* array, TokenInfo tinf){
    if(array->size == array->capacity){
        array->capacity = 1.4 * array->capacity + 1;
        array->tokenInf = realloc(array->tokenInf, array->capacity * sizeof(TokenInfo));
    }

    array->tokenInf[array->size++] = tinf;
}

TokenInfoArray tokenize(char* file){
    TokenInfoArray tArray;
    tArray.size = 0;
    tArray.capacity = 10;
    tArray.tokenInf = malloc(tArray.capacity * sizeof(TokenInfo));

    TwinBuffer tb;
    TwinBuffer* tbuf = &tb;
    initTwinBuffer(tbuf, file);
    initSymbolTable(&symbolTable);
    TokenInfo tinf;
    while((tinf = getNextToken(tbuf)).token != EPSILON){
        if(tinf.token != ERROR_TOKEN){
            // if(tinf.token == TK_ID){
            //     if(strlen(tinf.lexeme) <= 20)
            //         printf("Line No.  %d \t Lexeme : %s \t Token : %s\n", tinf.lineNumber, tinf.lexeme, tokToStr(tinf.token));
            //     else
            //         printf("Line No: %d Error :Variable Identifier is longer than the prescribed length of 20 characters.\n",tinf.lineNumber);
            // }
            // else if(tinf.token == TK_FUNID){
            //     if(strlen(tinf.lexeme) <= 30)
            //         printf("Line No.  %d \t Lexeme : %s \t Token : %s\n", tinf.lineNumber, tinf.lexeme,tokToStr(tinf.token));
            //     else
            //         printf("Line No: %d Error :Function Identifier is longer than the prescribed length of 30 characters.\n",tinf.lineNumber);
            // }
            // else printf("Line No.  %d \t Lexeme : %s \t Token : %s\n", tinf.lineNumber, tinf.lexeme,tokToStr(tinf.token));
            insertTinf(&tArray, tinf);
        }
        else{
        //printf("Line Number : %d Error : Unknow pattern %s\n", tinf.lineNumber, tinf.lexeme);
        if(strlen(tinf.lexeme) == 1)
            printf("Line No. %d Error : Unknow Symbol <%s>\n", tinf.lineNumber, tinf.lexeme);
        else 
            printf("Line No. %d Error : Unknow Pattern <%s>\n", tinf.lineNumber, tinf.lexeme);
        }
    }
    
    return tArray;
}