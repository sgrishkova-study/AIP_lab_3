#include <stdio.h>
#include <string.h>
#include <math.h>

#define BASE_DIFFERENCE 0.2076
#define BASE 65536

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))


void add(const unsigned short number_1[], const unsigned short number_2[], unsigned short result[],
    unsigned short size_of_number_1, unsigned short size_of_number_2, unsigned short size_of_result) {
    unsigned int temp;

    for (unsigned short i = 0; i < size_of_result; i++) {
        temp = result[i];

        if (i < size_of_number_1) {
            temp += number_1[i];
        }

        if (i < size_of_number_2) {
            temp += number_2[i];
        }

        unsigned short carry = temp / BASE;
        result[i] = temp % BASE;
        result[i + 1] = carry;
    }
};

unsigned short subtract(const unsigned short number_1[], const unsigned short number_2[], unsigned short result[],
    unsigned short size_of_number_1, unsigned short size_of_number_2, unsigned short size_of_result) {
    unsigned short minus = 0;
    short carry = 0;
    int temp;

    for (unsigned short i = 0; i < size_of_result; i++) {
        temp = carry + number_1[i];
        if (i < size_of_number_2) {
            temp -= number_2[i];
        }

        if (temp < 0) {
            if (i + 1 == size_of_result) {
                temp = -temp;
                minus = 1;
            }

            carry = -1;
            temp += BASE;
        } else {
            carry = 0;
        }

        result[i] = temp;
    }
    return minus;
};

int multiply(int a, int b) {};

int power(int a, int b) {};

int factorial(int n) {};

// арифметическая прогрессия
int sum_range(int a, int b) {};

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

// Digits in base 2^16 = [ Amount of digits in base 10 * log base 2^16 of 10 ]
unsigned short calculate_array_size(const char string[]) {
    unsigned long length_of_string = strlen(string);
    double calculation = (double) length_of_string * BASE_DIFFERENCE;
    return (unsigned short)(ceil(calculation));
}

int get_input(char* string[]) {
    size_t size_string_1 = 0;
    int length_of_string = 0;
    length_of_string = getline(string, &size_string_1, stdin);

    if (length_of_string > 0) {
        length_of_string = length_of_string - 1;
    }

    return length_of_string;
}

void convert_array_to_string(const unsigned short [], char string[]) {

}

int main(void) {
    char operation;
    scanf("%c\n", &operation);

    if (strchr("+-*^S!", operation) != NULL) {
        char* string_1 = NULL;
        int length_of_number_1 = get_input(&string_1);

        char* string_2 = NULL;
        int length_of_number_2 = get_input(&string_2);

        if ((length_of_number_1 > 0) && (length_of_number_2 > 0)) {
            string_1[length_of_number_1] = '\0';
            unsigned short number_1[calculate_array_size(string_1)];
            memset(number_1, 0, sizeof(number_1));
            unsigned short size_of_number_1 = sizeof(number_1) / sizeof(number_1[0]);
            convert_string_to_number(string_1, number_1, size_of_number_1);

            string_2[length_of_number_2] = '\0';
            unsigned short number_2[calculate_array_size(string_2)];
            memset(number_2, 0, sizeof(number_2));
            unsigned short size_of_number_2 = sizeof(number_2) / sizeof(number_2[0]);
            convert_string_to_number(string_2, number_2, size_of_number_2);

            unsigned short size_of_result = 0;

            switch (operation) {
                case '+': {
                    size_of_result = max(size_of_number_1, size_of_number_2) + 1;
                    unsigned short result[size_of_result];
                    memset(result, 0, sizeof(result));
                    add(number_1, number_2, result, size_of_number_1, size_of_number_2, size_of_result);
                }
                break;
                case '-': {
                    size_of_result = max(size_of_number_1, size_of_number_2);
                    unsigned short result[size_of_result];
                    memset(result, 0, sizeof(result));

                    unsigned short minus = 0;

                    if (size_of_number_1 > size_of_number_2) {
                        subtract(number_1, number_2, result, size_of_number_1, size_of_number_2, size_of_result);
                    } else {
                        minus = subtract(number_2, number_1, result, size_of_number_2, size_of_number_1, size_of_result);
                        if (size_of_number_1 < size_of_number_2) { minus = 1;}
                    }
                    printf("f");
                }
                break;
            }


            printf("yaay");
        } else {
            printf("Invalid number.");
            return -1;
        }

    } else {
        printf("Unknown operation.");
        return -1;
    }

    return 0;
}
