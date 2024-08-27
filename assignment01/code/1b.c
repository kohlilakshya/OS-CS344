#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

int main() {
    char *value = getenv("n");
    if (!value) {
        fun(100);
    } else {
        int len = strlen(value);
        // printf("len = %d\n", len);

        int num = 0;
        int j = 1;
        for (int i = len; i > 0; i--) {
            if (value[i - 1] != ' ') {
                char temp[2];
                temp[1] = '\0';
                temp[0] = value[i - 1];
                num += atoi(temp) * j;
                j *= 10;
            } else {
                fun(num);
                j = 1;
                num = 0;
            }
        }
        fun(num);
    }
    return 0;
}
