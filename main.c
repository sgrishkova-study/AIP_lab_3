#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define size_of_a_chunk 4
#define BASE_DIFFERENCE 0.2076
#define BASE 65536

int add(int a, int b) {

};

int subtract(int a, int b) {};

int multiply(int a, int b) {};

int power(int a, int b) {};

int factorial(int n) {};

int sum_range(int a, int b) {};

// я не знаю насколько это надо, по идее нужен какой-то обработчик инпута
// но если предполагается что инпут идеально причесанный то можно забить
void read_input() {};

void convert_string_to_number(char string[], unsigned short number[], unsigned short size_of_number) {
    unsigned long length_of_string = strlen(string);
    unsigned short digit;
    unsigned int temp_calc;
    unsigned short current_position = 0;

    for (unsigned long i = 0; i < length_of_string; i++) {
        digit = string[i] - '0';

        for (unsigned short j = 0; j <= current_position; j++) {
            temp_calc = number[j] * 10u + digit;
            number[j] = temp_calc % BASE;
            digit = temp_calc / BASE;
        }

        if (digit != 0) {
            current_position++;
            number[current_position] = digit;
        }
    }
}

// digits in base 2^16 = [ amount of digits in base 10 * log base 2^16 of 10 ]
unsigned short calculate_array_size(const char string[]) {
    unsigned long length_of_string = strlen(string);
    double calculation = (double) length_of_string * BASE_DIFFERENCE;

    return (unsigned short)(ceil(calculation));
}

int main(void) {
    unsigned short number[calculate_array_size("986513968951")];
    memset(number, 0, sizeof(number));
    convert_string_to_number("986513968951", number,sizeof(number) / sizeof(number[0]));

    printf("yaaay");

    // char operation;
    // scanf("%c", &operation);
    //
    // if (operation == '+') {
    //     read_input();
    //     add(1, 2);
    // } else if (operation == '-') {
    //     read_input();
    //     subtract(1, 2);
    // } else if (operation == '*') {
    //     read_input();
    //     multiply(1, 2);
    // } else if (operation == '^') {
    //     read_input();
    //     power(1, 2);
    // } else if (operation == '!') {
    //     read_input();
    //     factorial(1);
    // } else if (operation == 'S') {
    //     read_input();
    //     sum_range(1, 2);
    // } else {
    //     printf("Input error: invalid operation!\n");
    //     return 1;
    // };

    return 0;
}
