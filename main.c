#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define BASE_DIFFERENCE 0.2076 // Log base 2^16 to 10
#define BASE_REVERSE_DIFFERENCE 4.81648 // Log base 10 to 2^16
#define MAX_POSSIBLE_LENGTH 11 // Consider length of (65536 * 65535) + 1 for null terminator
#define BASE 65536 // 2^16; amount of all possible values short can store

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

static void add(const unsigned short number_1[], const unsigned short number_2[], unsigned short result[], const unsigned short size_of_number_1, const unsigned short size_of_number_2, const unsigned short size_of_result) {
    for (unsigned short i = 0; i < size_of_result; i++) {
        unsigned int temp = result[i];

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

static unsigned short subtract(const unsigned short number_1[], const unsigned short number_2[], unsigned short result[], const unsigned short size_of_number_1, const unsigned short size_of_number_2, const unsigned short size_of_result) {
    unsigned short minus = 0;
    short carry = 0;

    for (unsigned short i = 0; i < size_of_result; i++) {
        int temp = carry + number_1[i];
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

static int convert_string_to_number(char string[], unsigned short number[], unsigned short size_of_number) {
    unsigned long length_of_string = strlen(string);
    unsigned short current_position = 0;

    for (unsigned long i = 0; i < length_of_string; i++) {
        unsigned short digit = string[i] - '0';

        for (unsigned short j = 0; j <= current_position; j++) {
            unsigned int temp_calc = number[j] * 10u + digit;
            number[j] = temp_calc % BASE;
            digit = temp_calc / BASE;
        }

        if (digit != 0) {
            current_position++;
            if (current_position < size_of_number) {
                number[current_position] = digit;
            } else {
                printf("Exceeded maximum number of digits.");
                return -1;
            }
        }
    }

    return 0;
}

static unsigned short calculate_array_size(const char string[]) {
    // Digits in base 2^16 = [ Amount of digits in base 10 * log base 2^16 of 10 ]
    unsigned long length_of_string = strlen(string);
    double calculation = (double) length_of_string * BASE_DIFFERENCE;
    return (unsigned short)(ceil(calculation));
}

static unsigned int calculate_string_size(const unsigned short size_of_number) {
    double calculation = (double) size_of_number * BASE_REVERSE_DIFFERENCE;
    return (unsigned int)(ceil(calculation));
}

static int get_input(char* string[]) {
    size_t size_string_1 = 0;
    int length_of_string = 0;
    length_of_string = getline(string, &size_string_1, stdin);

    if (length_of_string > 0) {
        length_of_string = length_of_string - 1;
    }

    return length_of_string;
}

static void convert_number_to_string(const unsigned short number[], unsigned short size_of_number, char string[]) {
    unsigned short size_of_temp = calculate_string_size(size_of_number);
    unsigned short temp[size_of_temp];
    unsigned short count = 1;
    memset(temp, 0, sizeof(temp));

    for (int i = size_of_number - 1; i > -1; i--) {
        unsigned long carry = number[i];

        for (unsigned short j = 0; j < count; j++) {
            unsigned long temp_value = temp[j] * BASE + carry;
            temp[j] = temp_value % 10;
            carry = temp_value / 10;
        }

        while (carry != 0) {
            temp[count] = (carry % 10);
            carry = carry / 10;
            count++;
        }
    }

    for (unsigned short i = 0; i < count; i++) {
        string[i] = (char)('0' + temp[count - i - 1]);
    }

    string[count] = '\0';
}

int main(void) {
    char operation;
    scanf("%c\n", &operation);

    if (strchr("+-*^S!", operation) != NULL) {
        char* string_1 = NULL;
        const int length_of_number_1 = get_input(&string_1);

        char* string_2 = NULL;
        const int length_of_number_2 = get_input(&string_2);

        if ((length_of_number_1 > 0) && (length_of_number_2 > 0)) {
            string_1[length_of_number_1] = '\0';
            unsigned short number_1[calculate_array_size(string_1)];
            memset(number_1, 0, sizeof(number_1));
            const unsigned short size_of_number_1 = sizeof(number_1) / sizeof(number_1[0]);
            convert_string_to_number(string_1, number_1, size_of_number_1);

            string_2[length_of_number_2] = '\0';
            unsigned short number_2[calculate_array_size(string_2)];
            memset(number_2, 0, sizeof(number_2));
            const unsigned short size_of_number_2 = sizeof(number_2) / sizeof(number_2[0]);
            convert_string_to_number(string_2, number_2, size_of_number_2);

            unsigned short size_of_result = 0;

            switch (operation) {
                case '+': {
                    size_of_result = max(size_of_number_1, size_of_number_2) + 1;
                    unsigned short result[size_of_result];
                    memset(result, 0, sizeof(result));

                    add(number_1, number_2, result, size_of_number_1, size_of_number_2, size_of_result);

                    char output[calculate_string_size(size_of_number_1) + 1];
                    convert_number_to_string(result, size_of_result, output);
                    printf("Result=%s\n", output);

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

                }
                break;
            }

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
