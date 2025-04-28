#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistdio.h>
#include <stdbool.h>
/* CMocka header files */
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

/* Function you want to test */
int add(int a, int b) {
    return a + b;
}

/* Test function */
static void test_addition(void **state) {
    (void) state; /* unused */

    assert_int_equal(add(2, 3), 5);
    assert_int_equal(add(-1, 1), 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_addition),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
