#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void swap_integers(int a, int b);
void swap_integer_with_pointers(int *a, int *b);

int main(void) {

    int arr[] = {1, 2, 3, 4, 5, 6, 7, 11, 13, 15, 17, 19};
    
    /* define a pointer */
    int *ptr;
    printf("arr[4] = %d\n", arr[4]);  //output: 5

    printf("the address of the array = %p\n", (void *)arr);
    /* printf format specifier for pointer is: %p */

    printf("the address of element no. 0 of the array = %p\n", (void *)&arr[0]);
    printf("the address of element no. 4 of the array = %p\n", (void *)&arr[4]);

    printf("the value of the pointer = %p\n", (void *)ptr);
    /* dereferencing pointer ptr to access its value, as oppsoed to address ptr is -> to */
    printf("the value of the pointer points at = %d\n", *ptr);
    
    /* careful with pointers: */
    /*
     * int *p;                  //p itself and what points to may be modified
     * const int *p             //p itself may be modified; what p points to may not
     * int *const p;            //p itself may not be modified; what p points to may
     * const int *const p;      //p itself nor what it points to may be modified
    */



    return 0;
}

void swap_integers(int a, int b) {
    int temp;
    temp = a;
    a = b;
    b = temp;
}

void swap_integer_with_pointers(int *a, int *b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

