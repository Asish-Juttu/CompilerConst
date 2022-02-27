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