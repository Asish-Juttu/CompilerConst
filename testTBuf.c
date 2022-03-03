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

#include "twinBuffer.h"

int isDigit(char ch){
    return ch >= '0' && ch <= '9';
}

int toDigit(char ch){
    return ch - '0';
}

int main(){
    TwinBuffer tb;
    TwinBuffer* tbuf = &tb;

    initTwinBuffer(tbuf, "abc.txt");
    
    char ch;
    nextChar(tbuf, &ch);

    int st = 0;
    int num = 0;
    while(ch != EOF_CHAR){
        printf("(%c)", ch);
        switch(st){
            case 0 : 
                if(isDigit(ch)){
                    printf("0 -> 1\n");
                    int d = toDigit(ch);
                    num = num * 10 + d;
                    st = 1;
                }
                else{
                    printf("0 -> 0\n");
                    printf("%c\n", ch);
                    resetBegin(tbuf, 0);
                }
            break;

            case 1 : 
                if(isDigit(ch)){
                    printf("1 -> 1\n");
                    int d = toDigit(ch);
                    num = num * 10 + d;
                }
                else{
                    printf("1 -> 0\n");
                    printf("%d\n", num);
                    resetBegin(tbuf, 1);

                    st = 0;
                    num = 0;
                }
            break;
        }
        nextChar(tbuf, &ch);
    }

    
    return 0;
}