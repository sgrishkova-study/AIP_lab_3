#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define BASE_DIFFERENCE 0.2076 // Log base 2^16 to 10
#define BASE_REVERSE_DIFFERENCE 4.81648 // Log base 10 to 2^16
#define BASE 65536 // 2^16; amount of all possible values short can store

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

static void add(const unsigned short number_1[], const unsigned short number_2[], unsigned short result[], const unsigned short size_of_number_1, const unsigned short size_of_number_2, const unsigned short size_of_result) {
    unsigned int carry = 0;

    for (unsigned short i = 0; i < size_of_result; i++) {
        unsigned int temp = carry;

        if (i < size_of_number_1) {
            temp += number_1[i];
        }
        if (i < size_of_number_2) {
            temp += number_2[i];
        }

        result[i] = temp % BASE;
        carry = temp / BASE;
    }
};

static void subtract(const unsigned short number_1[], const unsigned short number_2[], unsigned short result[],const unsigned short size_of_number_1, const unsigned short size_of_number_2, const unsigned short size_of_result) {
    short carry = 0;

    for (unsigned short i = 0; i < size_of_result; i++) {
        int temp = number_1[i] - carry;

        if (i < size_of_number_2) {
            temp -= number_2[i];
        }

        if (temp < 0) {
            temp += BASE;
            carry = 1;
        } else {
            carry = 0;
        }

        result[i] = (unsigned short) temp;
    }
};

static void multiply(const unsigned short number_1[], const unsigned short number_2[], unsigned short result[], const unsigned short size_of_number_1, const unsigned short size_of_number_2, const unsigned short size_of_result) {
    for (unsigned short i = 0; i < size_of_number_2; i++) {
        unsigned long carry = 0;

        for (unsigned short j = 0; j < size_of_number_1; j++) {
            unsigned long temp = result[i + j] + number_1[j] * number_2[i] + carry;
            result[i + j] = temp % BASE;
            carry = temp / BASE;
        }

        unsigned short k = i + size_of_number_1;

        while (carry > 0 && k < size_of_result) {
            unsigned long temp = result[k] + carry;
            result[k] = temp % BASE;
            carry = temp / BASE;
            k++;
        }
    }
}

static int convert_string_to_number(const char string[], unsigned short number[], unsigned short size_of_number) {
    unsigned long length_of_string = strlen(string);
    unsigned short current_position = 0;

    for (unsigned long i = 0; i < length_of_string; i++) {
        unsigned short digit = string[i] - '0';

        if (string[i] < '0' || string[i] > '9') {
            printf("Invalid input string.");
            return -1;
        }

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

static int get_input(char* string[], bool* minus) {
    size_t size_string_1 = 0;
    int length_of_string = 0;
    length_of_string = getline(string, &size_string_1, stdin);

    if (length_of_string > 0) {
        length_of_string = length_of_string - 1;
    } else {
        return -1;
    }

    if (*string[0] == '-') {
        *minus = !*minus;
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

static short compare(const unsigned short number_1[], const unsigned short number_2[], unsigned short size_1, unsigned short size_2
) {
    while (size_1 > 0 && number_1[size_1 - 1] == 0) {
        size_1--;
    }

    while (size_2 > 0 && number_2[size_2 - 1] == 0) {
        size_2--;
    }

    if (size_1 > size_2) return 1;
    if (size_1 < size_2) return -1;

    for (int i = size_1 - 1; i >= 0; i--) {
        if (number_1[i] > number_2[i]) return 1;
        if (number_1[i] < number_2[i]) return -1;
    }

    return 0;
}

static void print_result(
    const unsigned short result[],
    unsigned short size_of_result,
    bool negative
) {
    char output[calculate_string_size(size_of_result) + 1];

    convert_number_to_string(result, size_of_result, output);

    printf("Result=%s%s\n", negative ? "-" : "", output);
}

int main(void) {
    char operation;
    scanf("%c\n", &operation);

    if (strchr("+-*^S!", operation) != NULL) {
        bool minus_1 = false;
        bool minus_2 = false;
        bool negative_output = false;

        char* string_1 = NULL;
        int length_of_number_1 = get_input(&string_1, &minus_1);
        if (minus_1) {
            memmove(string_1, string_1 + 1, strlen(string_1));
            length_of_number_1 -= 1;
        }

        char* string_2 = NULL;
        int length_of_number_2 = get_input(&string_2, &minus_2);
        if (minus_2) {
            memmove(string_2, string_2 + 1, strlen(string_2));
            length_of_number_2 -= 1;
        }

        int sign_1 = minus_1 ? -1 : 1;
        int sign_2 = minus_2 ? -1 : 1;

        if (operation == '*') {
            negative_output = (sign_1 * sign_2 < 0);
        } else {
            int operation_sign = (operation == '+') ? 1 : -1;
            int effective_sign_2 = sign_2 * operation_sign;

            if (sign_1 == effective_sign_2) {
                operation = '+';
            } else {
                operation = '-';
            }

            negative_output = (sign_1 == -1);
        }

        if ((length_of_number_1 > 0) && (length_of_number_2 > 0)) {
            string_1[length_of_number_1] = '\0';
            unsigned short number_1[calculate_array_size(string_1)];
            memset(number_1, 0, sizeof(number_1));
            const unsigned short size_of_number_1 = sizeof(number_1) / sizeof(number_1[0]);
            if (convert_string_to_number(string_1, number_1, size_of_number_1) == - 1) {
                return -1;
            };

            string_2[length_of_number_2] = '\0';
            unsigned short number_2[calculate_array_size(string_2)];
            memset(number_2, 0, sizeof(number_2));
            const unsigned short size_of_number_2 = sizeof(number_2) / sizeof(number_2[0]);

            if (convert_string_to_number(string_2, number_2, size_of_number_2) == -1) {
                return -1;
            }

            unsigned short size_of_result = 0;

            switch (operation) {
                case '+': {
                    size_of_result = max(size_of_number_1, size_of_number_2) + 1;
                    unsigned short result[size_of_result];
                    memset(result, 0, sizeof(result));

                    add(number_1, number_2, result, size_of_number_1, size_of_number_2, size_of_result);

                    char output[calculate_string_size(size_of_result) + 1];
                    convert_number_to_string(result, size_of_result, output);

                    print_result(result, size_of_result, negative_output);

                }
                break;
                case '-': {
                    size_of_result = max(size_of_number_1, size_of_number_2);
                    unsigned short result[size_of_result];
                    memset(result, 0, sizeof(result));

                    short check = compare(number_1, number_2, size_of_number_1, size_of_number_2);

                    if (check == 1) {
                        subtract(number_1, number_2, result, size_of_number_1, size_of_number_2, size_of_result);
                    } else if (check == -1) {
                        subtract(number_2, number_1, result, size_of_number_2, size_of_number_1, size_of_result);
                        negative_output = !negative_output;
                    } else {
                        printf("Result=0\n");
                    }

                    char output[calculate_string_size(size_of_result) + 1];
                    convert_number_to_string(result, size_of_result, output);

                    print_result(result, size_of_result, negative_output);
                }
                break;
                case '*': {
                    size_of_result = size_of_number_1 + size_of_number_2;
                    unsigned short result[size_of_result];
                    memset(result, 0, sizeof(result));
                    short check = compare(number_1, number_2, size_of_number_1, size_of_number_2);
                    if (check == 1) {
                        multiply(number_1, number_2, result, size_of_number_1, size_of_number_2, size_of_result);
                    } else {
                        multiply(number_2, number_1, result, size_of_number_2, size_of_number_1, size_of_result);
                    }

                    char output[calculate_string_size(size_of_result) + 1];
                    convert_number_to_string(result, size_of_result, output);

                    print_result(result, size_of_result, negative_output);
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
};