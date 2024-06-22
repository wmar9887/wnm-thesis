#ifndef MINIFLOAT_H
#define MINIFLOAT_H

#include <stdio.h>
#include <string.h>
#include <math.h>

int MF_construct(int s, int e, int m);
void MF_range();
double add_block_minifloats(unsigned char bin1, unsigned char bin2, int exponent_bits, int mantissa_bits);
double minifloat_calc(unsigned char value, int exponent_bits, int mantissa_bits);

#endif // MINIFLOAT_H
