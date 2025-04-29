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

Node *insert_node(Node *head, int value) {
    Node *temp = malloc(sizeof(*temp));
    temp->data = value;
    temp->next = NULL;

    if (head == NULL) {
        //if SLL is empty, then have head -> new node
        head = temp;
    } else {
        Node *p = head;
        while (p->next != NULL) {
            //traverse list until last node is found
            p = p->next;
        }
        p->next = temp;
    }
    return head;
}

Node *delete_node(Node *head, int value) {
    if (head == NULL) {
        fprintf(stderr, "Empty linked list!");
        return NULL;
    }
    if (head->data == value) {
        Node *temp = head;
        head = head->next;
        free(temp);
        return head;
    } else {
        Node *prev = head;
        Node *curr = head->next;
        while (curr != NULL) {
            if (curr->data == value) {
                //update (shuffle) new pointers
                prev->next = curr->next;
                free(curr);
                break;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    
    return head;
}

void print_list(Node *head) {
    Node *curr = head; int i = 1;
    while (curr != NULL) {
        printf("Node %d = %d\n", i, curr->data);
        curr = curr->next;
        i++;
    }
}

bool is_empty(Node *head) {
    return head == NULL;
}

/* test your code functions here */
static void test_node_pointer(void **state) {
    (void) state; //ignore if unused
    int *node_ptr = NULL;
    assert_null(node_ptr);
}

int main(int argc, char *argv[]) {
    // /* test object for assertions on functions */
    // /* calls function of static void type */ 
    // const struct CMUnitTest tests[] = {
    //     cmocka_unit_test(test_node_pointer)
    // };
    // /* test the unit tests! */
    // int test_result = cmocka_run_group_tests(tests, NULL, NULL);
    // if (test_result != 0) {
    //     printf("Some tests failed!\n");
    //     return cmocka_run_group_tests(tests, NULL, NULL);
    // }
    // printf("Success! All tests have passed.");
    // 
    // //random test casing the singly linked list!
    // Node head;
    // head.data = 1;
    // //head.next == ?
    
    Node *head = NULL;
    head = insert_node(head, 100);
    head = insert_node(head, 200);
    head = insert_node(head, 300);
    
    print_list(head);
    //update the list after deleting the node
    printf("------------\n");
    head = delete_node(head, 200);
    print_list(head);


    return EXIT_SUCCESS;
}
