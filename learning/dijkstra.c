#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define P_INFINITY  9999
#define N_INFINITY  -9999

int main(void) {

    printf("%d\n", P_INFINITY);
    printf("%d\n", N_INFINITY);

    int max_negative = INTMAX_MIN;
    int max_positive = INTMAX_MAX;

    return 0;
}
