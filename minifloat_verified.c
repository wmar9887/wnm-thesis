#include "minifloat.h"

int main() {
    double value = add_block_minifloats(0b01101010, 0b01100101, 4, 3);
    double v2 = minifloat_calc(0b01010110);
    printf("%f\n", value);
    return 0;
}


