#include <stdio.h>
#include <stdlib.h>

void fun(int n) {
    while (n != 1) {
        printf("%d ", n);
        if (n % 2) 
            n = 3 * n + 1;
        else 
            n /= 2;
    }
    printf("%d\n", n);
}

int main(int argc, char *argv[]) {
    if (argc == 1) fun(100);
    else {
        for (int i = 1; i < argc; i++) {
            int n = atoi(argv[i]);
            fun(n);
        }
    }
    
    return 0;
}
