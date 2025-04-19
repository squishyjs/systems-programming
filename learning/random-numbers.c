#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    /* Program outputs either yes or no */
    int yes_or_no;

    srand((unsigned)time(NULL));
    yes_or_no = rand() % 2;

    if (yes_or_no == 0) {
        printf("No\n");
    } else {
        printf("Yes\n");
    }
    return 0;
}
