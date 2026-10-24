#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int add(int a, int b) {};

int subtract(int a, int b) {};

int multiply(int a, int b) {};

int power(int a, int b) {};

int factorial(int n) {};

int sum_range(int a, int b) {};

// я не знаю насколько это надо, по идее нужен какой-то обработчик инпута
// но если предполагается что инпут идеально причесанный то можно забить
void read_input() {};

void convert_string_to_number(char string[], unsigned short number[]) {
    int string_length = strlen(string);

    for (int i = 0, j = string_length - 1; i < string_length; i++, j--) {
        number[j] = string[i] - '0';
    }
};

unsigned short calculate_array_size(char string[], short size_of_a_chunk) {
    short lenght_of_string = strlen(string);
    return (lenght_of_string / size_of_a_chunk) + (lenght_of_string % size_of_a_chunk != 0);
}

int main(void) {
    unsigned short number[calculate_array_size("11123456789", 4)];
    convert_string_to_number("123456789", number);

    printf("%d\n", calculate_array_size("1111111111111111111111111111111123456789", 4));

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
