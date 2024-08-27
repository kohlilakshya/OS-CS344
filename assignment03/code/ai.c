#include<stdio.h>
#include<stdlib.h>

void fun(int n) {
    while(n != 1) {
        printf("%d ", n);
        if(n % 2) n = 3 * n + 1;
        else n /= 2;
    }
    printf("%d\n", n);
}

int main(int argc, char *argv[]) {
    int n = 100;
    if(argc == 2) n = atoi(argv[1]);
    fun(n);
    return 0;
}
