#include "twinBuffer.h"
#include "lexer.h"
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

TokenInfo getNextToken(TwinBuffer* tbuf){

    char ch;
    nextChar(tbuf, &ch);
    while(1){
        if(ch==' '){
            nextChar(tbuf, &ch);
        }
        else if(ch=='\n'){
            line++;
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
                    resetBegin(tbuf,0);
                    while(ch != '\n'){
                        nextChar(tbuf, &ch);
                        resetBegin(tbuf, 0);
                    }
                    return (TokenInfo) {TK_COMMENT,NULL,line};
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
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_SQL,NULL,line};
                }
                else if(ch == ']'){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_SQR,NULL,line};
                }
                else if(ch==','){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_COMMA,NULL,line};
                }
                else if(ch==';'){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_SEM,NULL,line};
                }
                else if(ch==':'){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_COLON,NULL,line};
                }
                else if(ch=='.'){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_DOT,NULL,line};
                }
                else if(ch==')'){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_CL,NULL,line};
                }
                else if(ch=='('){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_OP,NULL,line};
                }
                else if(ch=='+'){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_PLUS,NULL,line};
                }
                else if(ch=='-'){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_MINUS,NULL,line};
                }
                else if(ch=='*'){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_MUL,NULL,line};
                }
                else if(ch=='/'){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_DIV,NULL,line};
                }
                else if(ch=='&'){
                    st = 51;
                }
                else if(ch=='@'){
                    st = 54;
                }
                else if(ch=='~'){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_NOT,NULL,line};
                }
                else if(ch=='!'){
                    st = 58;
                }
                else{
                    //ERROR
                }
            break;

            case 1:
                if(ch == '-'){
                    st = 2;
                }
                else if(ch == '='){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_LE,NULL,line};
                }
                else {
                    resetBegin(tbuf,1);
                    return (TokenInfo) {TK_LT,NULL,line};
                }
            break;

            case 2 :
                if(ch == '-'){
                    st = 3;
                }
                else{
                    // ERROR
                }
            break;

            case 3:
                if(ch == '-'){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_ASSIGNOP,NULL,line};
                }
                else{
                    // ERROR
                }
            break;

            case 8:
                if(isFieldID_2(ch)){
                }
                else{
                    resetBegin(tbuf,1);
                    return (TokenInfo) {TK_FIELDID,NULL,line}; // Lexeme and look-up table
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
                    resetBegin(tbuf,1);
                    return (TokenInfo) {TK_FIELDID,NULL,line}; // Lexeme and look-up table
                }
            break;

            case 11:
                if(isID(ch)){
                }
                else if(num_1(ch)){
                    st = 12;
                }
                else {
                    resetBegin(tbuf,1);
                    return (TokenInfo) {TK_FIELDID,NULL,line}; // Lexeme and look-up table
                }
            break;

            case 12:
                if(num_1(ch)){
                }
                else {
                    resetBegin(tbuf,1);
                    return (TokenInfo) {TK_FIELDID,NULL,line}; // Lexeme and look-up table
                }
            break;

            case 15:
                if(num_2(ch)){
                }
                else if(ch == '.'){
                    st = 17;
                }
                else{
                    resetBegin(tbuf,1);
                    return (TokenInfo) {TK_NUM,NULL,line}; // Lexeme 
                }
            break;

            case 17:
                if(num_2(ch)){
                    st = 18;
                }
                else{
                    resetBegin(tbuf,2);
                    return (TokenInfo) {TK_NUM,NULL,line}; // Lexeme 
                }
            break;

            case 18:
                if(num_2(ch)){
                    st = 19;
                }
                else{
                    resetBegin(tbuf,3);
                    return (TokenInfo) {TK_NUM,NULL,line}; // Lexeme 
                }
            break;

            case 19:
                if(ch=='E'){
                    st = 23;
                }
                else{
                    resetBegin(tbuf,1);
                    return (TokenInfo) {TK_RNUM,NULL,line}; // Lexeme 
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
                    //ERROR
                }
            break;

            case 24:
                if(num_2(ch)){
                    st = 25;
                }
                else{
                    //ERROR
                }
            break;

            case 25:
                if(num_2(ch)){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_RNUM,NULL,line}; // Lexeme 
                }
                else{
                    //ERROR
                }
            break;

            case 27:
                if(ch=='='){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_GE,NULL,line};
                }
                else{
                    resetBegin(tbuf,1);
                    return (TokenInfo) {TK_GT,NULL,line};
                }
            break;

            case 30:
                if(isFunID(ch)){
                    st = 31;
                }
                else{
                    //ERROR
                }
            break;

            case 31:
                if(isFunID(ch)){
                }
                else if(num_2(ch)){
                    st = 32;
                }
                else{
                    resetBegin(tbuf,1);
                    return (TokenInfo) {TK_ASSIGNOP,NULL,line}; // Lexeme and look-up table
                }
            break;

            case 32:
                if(num_2(ch)){
                }
                else{
                    resetBegin(tbuf,1);
                    return (TokenInfo) {TK_ASSIGNOP,NULL,line}; // Lexeme and look-up table
                }
            break;

            case 34:
                if(ch=='='){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_EQ,NULL,line};
                }
                else {
                    //ERROR
                }
            break;

            case 36:
                if(isFieldID_2(ch)){
                    st = 37;
                }
                else {
                    //ERROR
                }
            break;

            case 37:
                if(isFieldID_2(ch)){
                }
                else {
                    resetBegin(tbuf,1);
                    return (TokenInfo) {TK_RUID,NULL,line}; // Lexeme
                }
            break;

            case 51:
                if(ch=='&'){
                    st = 52;
                }
                else{
                    //ERROR
                }
            break;

            case 52:
                if(ch=='&'){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_AND,NULL,line}; 
                }
                else{
                    //ERROR
                }
            break;

            case 54:
                if(ch=='@'){
                    st = 55;
                }
                else{
                    //ERROR
                }
            break;

            case 55:
                if(ch=='@'){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_OR,NULL,line}; 
                }
                else{
                    //ERROR
                }
            break;

            case 58:
                if(ch=='='){
                    resetBegin(tbuf,0);
                    return (TokenInfo) {TK_NE,NULL,line}; 
                }
                else{
                    //ERROR
                }
            break;
        }
        nextChar(tbuf, &ch);
    }

    
    return (TokenInfo){EPSILON,NULL,line};
}