#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define F               fflush(stdout)
#define NAME_BUFFER     20
#define MAX_DATA        10

int main(void) {
    char name[NAME_BUFFER];
    char buffer[MAX_DATA];
    char *p;
    int age, ok;

    printf("Enter your name: "); F;

    if (fgets(name, sizeof(name), stdin) == NULL) {
        printf("Error reading name from stdin\n");
        return EXIT_FAILURE;
    }

    p = strchr(name, '\n');
    if (p == NULL)
        *p = '\0';

    fprintf(stdout, "Welcome, %s!\n", name);

    do {
        printf("Enter your age [1-150]: "); F;
        if (fgets(buffer, sizeof(buffer), stdin) != NULL
            && sscanf(buffer, "%d", &age) == 1
            && age >= 1 && age <= 150) {
            ok = true;
        } 
        else {
            ok = false;
            printf("Invalid age! Please enter again...\n");
        }
    } while (!ok);

    printf("Your age now is %d. In 10 years your age will be %d.\n", age, age+10);
    return EXIT_SUCCESS;
}
