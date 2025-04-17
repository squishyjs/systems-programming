#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void) {

    printf("Integer range: \t%d\t%d\n", INT_MIN, INT_MAX);
    printf("Long range: \t%ld\t%ld\n", LONG_MIN, LONG_MAX);
    printf("Float range: \t%e\t%e\n", FLT_MIN, FLT_MAX);
    printf("Double range: \t%e\t%e\n", DBL_MIN, DBL_MAX);
    printf("Long Double range: \t%e\t%e\n", LDBL_MIN, LDBL_MAX);
    printf("Float-Double: \t%e\t%e\n", FLT_EPSILON, DBL_EPSILON);
}
