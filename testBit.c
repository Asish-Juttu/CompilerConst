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

#include <stdio.h>
typedef struct{
    unsigned long long a : 100;
} A;

int main(){
    A a;
    for(int i = 0; i < 100; i++){
        a.a = a.a | 1 << i;
    }

    for(int i = 0; i < 100; i++){
        printf("%d", (a.a >> i) & 1);
    }

    printf("\n%llu",a.a);
    return 0;
}