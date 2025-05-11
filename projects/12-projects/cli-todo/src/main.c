#include "../include/dbg.h"
#include "../include/list.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define F       fflush(stdout)
#define VIEWING true
#define NOT_VIEWING false

typedef enum {
    MENU_VIEW=1,
    MENU_CREATE,
    MENU_EXIT
} MenuOption;

/* signatures */ 
void cli();

int main(int argc, char *argv[]) {

    cli();
    return EXIT_SUCCESS;
}

void cli() {
    while (VIEWING) { //FIXME: infinite loop!
        printf("Welcome to your TODO-list, John!\n");
        printf("You currently have 0 tasks to complete!\n");
        printf("Would you like to:\n");
        printf("Would you like to:\n");
        printf("------------------\n");
        printf("1. View current tasks.\n");
        printf("2. Create a new task.\n");
        printf("3. Exit task menu.");
        printf("------------------\n");
    }
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case MENU_VIEW:
            printf("Viewing tasks...\n");
            break;
        case MENU_CREATE:
            printf("Creating a new task...\n");
            break;
        case MENU_EXIT:
            printf("Goodbye!");
            break;
        default:
            printf("Please enter [1, 2, or 3]");
            break;
    }
}
