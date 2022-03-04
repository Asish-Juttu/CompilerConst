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

#include "lexer.h"
#include "twinBuffer.h"
#include "symbolTable.h"
#include <string.h>
int main(){
    TwinBuffer tb;
    TwinBuffer* tbuf = &tb;

    initTwinBuffer(tbuf, "testcases/t3.txt");
    initSymbolTable(&symbolTable);
    TokenInfo tinf;
    while((tinf = getNextToken(tbuf)).token != EOF_TOKEN){
        if(tinf.token != ERROR_TOKEN){
            if(tinf.token == TK_ID){
                if(strlen(tinf.lexeme) <= 20)
                printf("Line No.  %d \t Lexeme : %s \t Token : %s\n", tinf.lineNumber, tinf.lexeme, tokToStr(tinf.token));
                else
                printf("Line No: %d Error :Variable Identifier is longer than the prescribed length of 20 characters.\n",tinf.lineNumber);
            }
            else if(tinf.token == TK_FUNID){
                if(strlen(tinf.lexeme) <= 30)
                printf("Line No.  %d \t Lexeme : %s \t Token : %s\n", tinf.lineNumber, tinf.lexeme,tokToStr(tinf.token));
                else
                printf("Line No: %d Error :Function Identifier is longer than the prescribed length of 30 characters.\n",tinf.lineNumber);
            }
            else printf("Line No.  %d \t Lexeme : %s \t Token : %s\n", tinf.lineNumber, tinf.lexeme,tokToStr(tinf.token));
        }
        else{
            //printf("Line Number : %d Error : Unknow pattern %s\n", tinf.lineNumber, tinf.lexeme);
            if(strlen(tinf.lexeme) == 1)
            printf("Line No. %d Error : Unknow Symbol <%s>\n", tinf.lineNumber, tinf.lexeme);
            else 
            printf("Line No. %d Error : Unknow Pattern <%s>\n", tinf.lineNumber, tinf.lexeme);
        }
    }
    return 1;
}