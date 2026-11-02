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
    char operation;
    scanf("%c\n", &operation);

    char * string_1 = NULL;
    size_t size_string_1 = 0;
    unsigned int length_of_number_1 = getline(&string_1, &size_string_1, stdin);
    string_1[length_of_number_1 - 1] = '\0';

    unsigned short number_1[calculate_array_size(string_1)];
    memset(number_1, 0, sizeof(number_1));
    convert_string_to_number(string_1, number_1,sizeof(number_1) / sizeof(number_1[0]));

    //

    char * string_2 = NULL;
    size_t size_string2 = 0;
    unsigned int length_of_number_2 = getline(&string_2, &size_string2, stdin);
    string_2 [length_of_number_2 - 1] = '\0';

    unsigned short number_2[calculate_array_size(string_2)];
    memset(number_2, 0, sizeof(number_2));
    convert_string_to_number(string_2, number_2,sizeof(number_2) / sizeof(number_2[0]));


    printf("%s\n", string_1);
    return 0;
}
