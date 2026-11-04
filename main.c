#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define BASE_DIFFERENCE 0.20763 // Log base 2^16 to 10
#define BASE_REVERSE_DIFFERENCE 4.8165 // Log base 10 to 2^16
#define BASE 65536 // 2^16; amount of all possible values short can store
#define MAX_INPUT_SIZE 208
#define MAX_RESULT_SIZE 416

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

static void increment(unsigned short number[], unsigned short size) {
    unsigned int carry = 1;

    for (unsigned short i = 0; i < size && carry; i++) {
        unsigned int temp = number[i] + carry;

        number[i] = temp % BASE;
        carry = temp / BASE;
    }
}

static void multiply(const unsigned short number_1[], const unsigned short number_2[], unsigned short result[], const unsigned short size_of_number_1, const unsigned short size_of_number_2, const unsigned short size_of_result) {
    for (unsigned short i = 0; i < size_of_number_2; i++) {
        unsigned long carry = 0;

        for (unsigned short j = 0; j < size_of_number_1; j++) {
            unsigned long temp = (unsigned long)result[i + j] + (unsigned long)number_1[j] * (unsigned long)number_2[i] + carry;
            result[i + j] = (unsigned short)(temp % BASE);
            carry = temp / BASE;
        }

        unsigned short k = i + size_of_number_1;

        while (carry > 0 && k < size_of_result) {
            unsigned long temp = (unsigned long)(result[k] + carry);
            result[k] = (unsigned short)(temp % (BASE));
            carry = temp / BASE;
            k++;
        }
    }
}

static unsigned short get_actual_size(const unsigned short number[], unsigned short size) {
    while (size > 1 && number[size - 1] == 0) {
        size--;
    }

    return size;
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

static unsigned short divide_by_2(unsigned short number[], unsigned short size) {
    unsigned int carry = 0;
    for (int i = size - 1; i >= 0; i--) {
        unsigned int temp = carry * BASE + number[i];

        number[i] = temp / 2;
        carry = temp % 2;
    }

    return (unsigned short) carry;
}

static void factorial(const unsigned short number[], unsigned short result[], unsigned short size_of_number, unsigned short size_of_result) {
    result[0] = 1;

    unsigned short multiplier[size_of_number];
    memset(multiplier, 0, sizeof(multiplier));
    multiplier[0] = 2;

    unsigned short multiplier_size = get_actual_size(multiplier, size_of_number);

    while (compare(multiplier, number, multiplier_size, size_of_number) <= 0) {
        unsigned short temp[size_of_result];
        memset(temp, 0, sizeof(temp));

        multiply(result,multiplier, temp, size_of_result,multiplier_size, size_of_result);
        memcpy(result, temp, size_of_result * sizeof(unsigned short));
        increment(multiplier, size_of_number);

        multiplier_size = get_actual_size(multiplier, size_of_number);
    }
}

// exponentiation by squaring
static void power(const unsigned short number[], unsigned short exponent[], unsigned short result[], unsigned short size_of_number, unsigned short size_of_exponent, unsigned short size_of_result) {
    result[0] = 1;

    unsigned short base[size_of_result];
    memset(base, 0, sizeof(base));
    memcpy(base,number,size_of_number * sizeof(unsigned short));

    unsigned short exp[size_of_exponent];
    memcpy(exp, exponent,size_of_exponent * sizeof(unsigned short));

    unsigned short base_size = get_actual_size(base, size_of_result);
    unsigned short exp_size = get_actual_size(exp, size_of_exponent);
    unsigned short result_size = 1;

    while (!(exp_size == 1 && exp[0] == 0)) {
        unsigned short remainder = divide_by_2(exp, exp_size);
        exp_size = get_actual_size(exp, exp_size);

        if (remainder == 1) {
            unsigned short temp[size_of_result];
            memset(temp, 0, sizeof(temp));
            multiply(result, base, temp, size_of_result, base_size, size_of_result);

            memcpy(result, temp, sizeof(temp));

            result_size = get_actual_size(result, size_of_result);
        }

        if (exp_size == 1 && exp[0] == 0) {
            break;
        }

        unsigned short temp[size_of_result];
        memset(temp, 0, sizeof(temp));

        multiply(base, base, temp, base_size, base_size, size_of_result);
        memcpy(base, temp, sizeof(temp));

        base_size = get_actual_size(base, size_of_result);
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

static int get_input(char **string, bool *minus) {
    size_t capacity = 0;
    ssize_t length = getline(string, &capacity, stdin);

    if (length < 0) {
        return -1;
    }

    if (length > 0 && (*string)[length - 1] == '\n') {
        (*string)[--length] = '\0';
    }

    if (length > 0 && (*string)[0] == '-') {
        *minus = true;
        memmove(*string, *string + 1, (size_t)length);
        length--;
    }

    return length;
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

static void print_result(const unsigned short result[], unsigned short size_of_result, bool negative) {
    char output[calculate_string_size(size_of_result) + 1];
    convert_number_to_string(result, size_of_result, output);
    printf("Result=%s%s", negative ? "-" : "", output);
}

static bool decrement(unsigned short number[], unsigned short size) {
    for (unsigned short i = 0; i < size; i++) {
        if (number[i] > 0) {
            number[i]--;
            return true;
        }
        number[i] = BASE - 1;
    }
    return false;
}

static void sum_to_n(const unsigned short number[], unsigned short size_of_number, unsigned short result[],unsigned short size_of_result) {
    unsigned short next[size_of_number + 1];
    unsigned short product[size_of_result];

    memset(next, 0, sizeof(next));
    memset(product, 0, sizeof(product));

    memcpy(next, number, size_of_number * sizeof(unsigned short));
    increment(next, size_of_number + 1);

    unsigned short next_size = get_actual_size(next, size_of_number + 1);

    multiply(number,next, product,size_of_number,next_size,size_of_result);
    divide_by_2(product, size_of_result);

    memcpy(result,product,size_of_result * sizeof(unsigned short));
}

static int sum_range(const unsigned short a[], const unsigned short b[], unsigned short result[], unsigned short size_of_a, unsigned short size_of_b, unsigned short size_of_result) {
    if (compare(a, b, size_of_a, size_of_b) > 0) {
        return -1;
    }

    unsigned short sum_b[size_of_result];
    unsigned short a_minus_one[size_of_a];
    unsigned short sum_before_a[size_of_result];

    memset(sum_b, 0, sizeof(sum_b));
    memset(a_minus_one, 0, sizeof(a_minus_one));
    memset(sum_before_a, 0, sizeof(sum_before_a));

    sum_to_n(b, size_of_b, sum_b, size_of_result);

    memcpy(a_minus_one, a,size_of_a * sizeof(unsigned short));

    if (!decrement(a_minus_one, size_of_a)) {
        return -1;
    }

    sum_to_n(a_minus_one,size_of_a, sum_before_a, size_of_result);
    subtract(sum_b,sum_before_a, result, size_of_result,size_of_result,size_of_result);

    return 0;
}

int main(void) {
    char operation;
    if (scanf("%c\n", &operation) != 1) {
        printf("Unknown operation.");
        return -1;
    }

    if (strchr("+-*^S!", operation) == NULL) {
        printf("Unknown operation.");
        return -1;
    }

    bool minus_1 = false;

    char* string_1 = NULL;
    int length_of_number_1 = get_input(&string_1, &minus_1);

    if (length_of_number_1 <= 0) {
        printf("Invalid number.");
        return -1;
    }

    if (operation == '!') {
        if (minus_1) {
            printf("Factorial of negative numbers is not supported.");
            return -1;
        }

        unsigned short number_1[calculate_array_size(string_1)];
        memset(number_1, 0, sizeof(number_1));

        unsigned short size_of_number_1 = sizeof(number_1) / sizeof(number_1[0]);

        if (convert_string_to_number(string_1, number_1, size_of_number_1) == -1) { return -1; }

        unsigned short result[MAX_RESULT_SIZE] = {0};

        factorial(number_1, result,size_of_number_1,MAX_RESULT_SIZE);
        unsigned short size_of_result = get_actual_size(result, MAX_RESULT_SIZE);

        print_result(result, size_of_result, false);
        return 0;
    }

    bool minus_2 = false;
    bool negative_output = false;
    char* string_2 = NULL;
    int length_of_number_2 = get_input(&string_2, &minus_2);

    if (length_of_number_2 <= 0) {
        printf("Invalid number.");
        return -1;
    }

    int sign_1 = minus_1 ? -1 : 1;
    int sign_2 = minus_2 ? -1 : 1;

    if (operation == 'S') {
        if (minus_1 || minus_2) {
            printf("Negative numbers are not supported for S.");
            return -1;
        }
    } else if (operation == '*') {
        negative_output = (sign_1 * sign_2 < 0);
    } else if (operation == '^') {
        if (minus_2) {
            printf("Negative exponents are not supported.");
            return -1;
        }
        negative_output = minus_1 && ((string_2[length_of_number_2 - 1] - '0') % 2 != 0);
    } else {
        int operation_sign = operation == '+' ? 1 : -1;
        int effective_sign_2 = sign_2 * operation_sign;

        if (sign_1 == effective_sign_2) {
            operation = '+';
        } else {
            operation = '-';
        }

        negative_output = sign_1 == -1;
    }

    if ((length_of_number_1 > 0) && (operation == '!' || length_of_number_2 > 0)) {
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
                case '^': {
                    size_of_result = MAX_RESULT_SIZE;
                    unsigned short result[size_of_result];
                    memset(result, 0, sizeof(result));

                    power(number_1, number_2, result, size_of_number_1, size_of_number_2, size_of_result);

                    size_of_result = get_actual_size(result, size_of_result);

                    print_result(result, size_of_result, negative_output);
                }
                break;
                case 'S': {
                    size_of_result = MAX_RESULT_SIZE;

                    unsigned short result[MAX_RESULT_SIZE];
                    memset(result, 0, sizeof(result));

                    if (sum_range(number_1,number_2,result,size_of_number_1,size_of_number_2, size_of_result) == -1) {
                        printf("Invalid range.");
                        return -1;
                    }

                    size_of_result = get_actual_size(result, size_of_result);
                    print_result(result, size_of_result, false);
                }
                break;
            }
        } else {
            printf("Invalid number.");
            return -1;
        }

    return 0;
};