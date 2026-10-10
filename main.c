#include <stdio.h>

int add(int a, int b) {};

int subtract(int a, int b) {};

int multiply(int a, int b) {};

int power(int a, int b) {};

int factorial(int n) {};

int sum_range(int a, int b) {};

int main(void) {
    char operation;
    scanf("%c", &operation);

    if (operation == '+') {
        add(1, 2);
    } else if (operation == '-') {
        subtract(1, 2);
    } else if (operation == '*') {
        multiply(1, 2);
    } else if (operation == '^') {
        power(1, 2);
    } else if (operation == '!') {
        factorial(1);
    } else if (operation == 'S') {
        sum_range(1, 2);
    } else {
        printf("Input error: invalid operation!\n");
        return 1;
    };

    return 0;
}
