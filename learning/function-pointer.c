/* fp.c */
#include <stdio.h>
#include "dbg.h"

#define F fflush(stdout)
#define ADDITION 1
#define SUBTRACTION 2
#define DIVISION 3
#define MULTIPLICATION 4

void addition(int *target, int a, int b) {
    *target = a + b;
    return;
}

void subtraction(int *target, int a, int b) {
    *target = a - b;
    return;
}

void division(int *target, int a, int b) {
    *target = a / b;
    return;
}

void mutliplication(int *target, int a, int b) {
    *target = a * b;
    return;
}

int main() {
    int x, y, op, result;       //op=operation
    void (*func_ptr)(int*, int, int);

    printf("Press 1 for addition, 2 for subtraction, 3 for division, 4 for multiplication, and 0 to quit.\n");
    scanf("%d", &op);

    switch (op) {
        case ADDITION:
            /* addition */
            func_ptr = addition;
            break;
        case SUBTRACTION:
            func_ptr = subtraction;
            break;
        case DIVISION:
            func_ptr = division;
            break;
        case MULTIPLICATION:
            func_ptr = mutliplication;
            break;
        default:
            return 0;
    }

    printf("Number 1: "); F;
    scanf("%d", &x);
    printf("Number 2: "); F;
    scanf("%d", &y);

    (func_ptr)(&result, x, y);
    printf("\nResult: %d\n", result);
    check_mem(func_ptr);

error:
    return -1;

    return 0;
}
