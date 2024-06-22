#include "minifloat.h"

int MF_construct(int s, int e, int m) {
    int value = 0;
    // Sign = s
    // Exponent = e
    // Mantissa = m

    value |= (s << 7);
    value |= (e << 3);
    value |= m;
    printf("MiniFLoat Value: %d\n", value);
    return value;
}

double minifloat_calc(unsigned char value, int exponent_bits, int mantissa_bits) {
    // Extracting the sign, exponent, and mantissa from the value
    int sign = (value >> 7) & 1;
    int exp = (value & 0b01111000) >> 3;
    int mantissa = value & 0b00000111;

    double result;
    double sum;

    // Handle denormal numbers (exponent is zero)
    if (exp == 0) {
        sum = mantissa * pow(2, -mantissa_bits);
        result = pow(-1, sign) * pow(2, -6) * sum;
    } else {
        sum = 1 + mantissa * pow(2, -mantissa_bits);
        result = pow(-1, sign) * pow(2, exp - 7) * sum;
    }

    return result;
}

void MF_range() {
    int sign;
    int exponent;
    int mantissa;
    float result = 0;
    float sum = 0;


    for (int i = 0b0; i < 0b100000000; i++) {
        printf("\n");
        sign = (i >> 7) & 1;
        //print("Sign", bin(sign)[2:].zfill(1))
    
        exponent = (i & 0b01111000) >> 3;
        //print("Exponent:", bin(exponent)[2:].zfill(4))

        mantissa = (i & 0b00000111);
        //print("Mantissa:", bin(mantissa)[2:].zfill(3))

        if (exponent == 0b0) {
            sum = 0 + mantissa * pow(2, -3);
            result = pow(-1, sign) * pow(2, -6) * sum;
        }

        else {
            sum = 1 + mantissa * pow(2, -3);
            result = pow(-1, sign) * pow(2, exponent - 7) * sum;
        }
        //print("Value:", bin(value)[2:].zfill(8));
        printf("%f\n", result);
    }
}

double add_block_minifloats(unsigned char bin1, unsigned char bin2, int exponent_bits, int mantissa_bits) {
    // Information Extraction
    int sign1 = (bin1 >> 7) & 1;
    int exp1 = (bin1 & 0b01111000) >> 3;
    int mantissa1 = bin1 & 0b00000111;

    int sign2 = (bin2 >> 7) & 1;
    int exp2 = (bin2 & 0b01111000) >> 3;
    int mantissa2 = bin2 & 0b00000111;

    // Constants
    int bias = (1 << (exponent_bits - 1)) - 1;

    // Handle denormal numbers (exponent is zero)
    if (exp1 == 0) {
        mantissa1 <<= 1;  // Shift mantissa left
    } else {
        mantissa1 = (1 << mantissa_bits) | mantissa1;  // Add implicit leading 1
    }

    if (exp2 == 0) {
        mantissa2 <<= 1;  // Shift mantissa left
    } else {
        mantissa2 = (1 << mantissa_bits) | mantissa2;  // Add implicit leading 1
    }

    // Align exponents
    int exp_result;
    if (exp1 > exp2) {
        mantissa2 >>= (exp1 - exp2);
        exp_result = exp1;
    } else if (exp2 > exp1) {
        mantissa1 >>= (exp2 - exp1);
        exp_result = exp2;
    } else {
        exp_result = exp1;
    }

    // Add or subtract mantissas based on sign
    int mantissa_result;
    int sign_result;
    if (sign1 == sign2) {
        mantissa_result = mantissa1 + mantissa2;
        sign_result = sign1;
    } else {
        if (mantissa1 >= mantissa2) {
            mantissa_result = mantissa1 - mantissa2;
            sign_result = sign1;
        } else {
            mantissa_result = mantissa2 - mantissa1;
            sign_result = sign2;
        }
    }

    // Normalize result
    if (mantissa_result & (1 << (mantissa_bits + 1))) {
        mantissa_result >>= 1;
        exp_result += 1;
    }

    // Handle overflow
    if (exp_result >= (1 << exponent_bits) - 1) {
        exp_result = (1 << exponent_bits) - 1;
        mantissa_result = 0;
    } else {
        while (mantissa_result && !(mantissa_result & (1 << mantissa_bits))) {
            mantissa_result <<= 1;
            exp_result -= 1;
        }
    }

    // Remove implicit leading 1 for normal numbers
    if (exp_result > 0) {
        mantissa_result &= ~(1 << mantissa_bits);
    } else {
        mantissa_result >>= 1;  // Adjust for denormal numbers
    }

    // Calculate final value as a decimal
    float final_result;
    if (exp_result == 0) {
        float sum = 0 + mantissa_result * pow(2, -mantissa_bits);
        final_result = pow(-1, sign_result) * pow(2, 1 - bias) * sum;
    } else {
        float sum = 1 + mantissa_result * pow(2, -mantissa_bits);
        final_result = pow(-1, sign_result) * pow(2, exp_result - bias) * sum;
    }
    return final_result;
}