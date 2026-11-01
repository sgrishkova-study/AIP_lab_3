#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define size_of_a_chunk 4
#define base_difference 0.2076

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
    unsigned short string_length = strlen(string);
    unsigned short limb_number = size_of_number - 1;
    unsigned short step = string_length % size_of_a_chunk;

    if (step == 0) { step = size_of_a_chunk; };

    for (unsigned short i = 0; i < string_length; i++) {
        if (step == 0) { step = size_of_a_chunk; limb_number -= 1; };

        unsigned short digit = string[i] - '0';
        unsigned short multiplier = 1;

        for (unsigned short j = 1; j < step; j++) {
            multiplier *= 10;
        }

        number[limb_number] += digit * multiplier;
        step -= 1;
    }
}

// digits in base 2^16 = [ amount of digits in base 10 * log base 2^16 of 10 ]
unsigned short calculate_array_size(const char string[]) {
    int length_of_string = strlen(string);
    double calculation = (double) length_of_string * base_difference;
    return (unsigned short)(ceil(calculation));
}

int main(void) {

    printf("%u", calculate_array_size("123456666666"));
    // unsigned short number[calculate_array_size("123456789")];
    // memset(number, 0, sizeof(number));
    // convert_string_to_number("123456789", number,sizeof(number) / sizeof(number[0]));

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
