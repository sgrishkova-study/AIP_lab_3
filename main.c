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
    char operation;
    scanf("%c\n", &operation);

    char * number_1 = NULL;
    size_t size_num1 = 0;
    unsigned int length_of_number_1 = getline(&number_1, &size_num1, stdin);
    number_1[length_of_number_1 - 1] = '\0';

    unsigned short number[calculate_array_size(number_1)];
    memset(number, 0, sizeof(number));
    convert_string_to_number(number_1, number,sizeof(number) / sizeof(number[0]));

    //

    char * number_2 = NULL;
    size_t size_num2 = 0;
    unsigned int length_of_number_2 = getline(&number_2, &size_num2, stdin);
    number_2 [length_of_number_2 - 1] = '\0';

    unsigned short number2[calculate_array_size(number_2)];
    memset(number2, 0, sizeof(number));
    convert_string_to_number(number_2, number2,sizeof(number) / sizeof(number[0]));


    printf("%s\n", number_1);
    return 0;
}
