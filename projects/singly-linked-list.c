#include "unitypes.h"
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

/* A node stores a value (data) and its next node */
typedef struct Node {
    struct Node *next;
    int data;
} Node;

void insert_node(Node *head, int value) {

}

void delete_node(Node *head, Node *target_node) {

}

bool is_empty(Node *head) {
    return head->next == NULL;
}




static void test_node_pointer(void **state) {
    (void) state; //ignore if unused

    int *node_ptr = NULL;
    assert_null(node_ptr);
}

int main(int argc, char *argv[]) {
    /* test object for assertions on functions */
    /* calls function of static void type */ 
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_node_pointer)
    };

    int test_result = cmocka_run_group_tests(tests, NULL, NULL);
    if (test_result != 0) {
        printf("Some tests failed!\n");
        return cmocka_run_group_tests(tests, NULL, NULL);
    }
    printf("Success! All tests have passed.");
    return EXIT_SUCCESS;
}
