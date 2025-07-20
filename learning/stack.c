#include "dbg.h"

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_DATA   100

typedef struct Stack {
    int *data;
    int top_element;
    size_t capacity;
    
    bool (*empty)(struct Stack *self);
    size_t (*size)(struct Stack *self);
    int (*top)(struct Stack *self);
    void (*push)(struct Stack *self, int element);
    void (*pop)(struct Stack *self);
} Stack;

void push_element(Stack *stack, int element) {
    if (stack->top_element < (int)(stack->capacity - 1)) {
        stack->data[++(stack->top_element)] = element;
        printf("Pushed %d to top of stack.\n", element);
    } else {
        fprintf(stderr, "error: stack overflow.\n");
    }
}

void pop_element(Stack *stack) {
    // stack->data[(stack->top_element)--];
    // printf("Removed top element.\n");
    if (stack->top_element >= 0) {
        printf("Removed top element (%d) from stack.\n", stack->top_element);
        stack->top_element--;
    } else {
        fprintf(stderr, "error: stack underflow.\n");
    }
}

size_t get_stack_size(Stack *stack) {
    // return stack->stack_size;
    return (size_t)(stack->top_element + 1);
}

int top_element(Stack *stack) {
    if (stack->top_element >= 0) {
        return stack->data[stack->top_element];
    } else {
        fprintf(stderr, "error: empty stack.\n");
        // printf("error: stack is empty.\n");
        goto error;
    }
error:
    return -1;
}

bool is_empty(Stack *stack) {
    return stack->top_element == -1;
}

Stack *create_stack(int capacity) {
    Stack *my_stack         = malloc(sizeof(Stack));
    if (!my_stack) {
        fprintf(stderr, "errro: malloc (stack) failed.\n");
        return NULL;
    }
    //members
    my_stack->data          = malloc(sizeof(int) * capacity);
    if (!my_stack->data) {
        fprintf(stderr, "error: malloc (data) failed.\n");
        free(my_stack);
        return NULL;
    }
    my_stack->capacity      = capacity;
    my_stack->top_element   = -1;

    //methods
    my_stack->push = &push_element;
    my_stack->pop = &pop_element;
    my_stack->size = &get_stack_size;
    my_stack->top = &top_element;
    my_stack->empty = &is_empty;

    return my_stack;
}

int main(void) {
    Stack *my_new_stack = create_stack(100);
    check_mem(my_new_stack);
    if (my_new_stack->empty(my_new_stack)) {
        fprintf(stderr, "error: stack empty.\n");
    }
    my_new_stack->push(my_new_stack, 1);
    my_new_stack->push(my_new_stack, 5);
    my_new_stack->push(my_new_stack, 20);

    my_new_stack->top(my_new_stack); /* output: 20 */
    my_new_stack->pop(my_new_stack);
    my_new_stack->top(my_new_stack); /* output: 5 */
    if (!my_new_stack->empty(my_new_stack)) {
        printf("Stack IS NOT empty!\n");
    }

    // free heap memory
    free(my_new_stack->data);
    free(my_new_stack);
error:
    return -1;
}
