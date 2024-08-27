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

int main() {
    int n;

    while(1) {  // Infinite loop to continuously accept input
        // printf("Enter the value of n (enter 1 to exit): ");
        scanf("%d", &n);
        fun(n);
        if(n == 1) break;  // Exit condition if the user enters 1
    }
    
    return 0;
}
