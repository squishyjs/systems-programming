#include "../include/dbg.h"
#include "../include/list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>

#define F       fflush(stdout)

bool VIEWING = true;
bool MAIN_EXIT = false;
const char *task_dir = "./tasks";

typedef enum {
    MENU_VIEW=1,
    MENU_CREATE,
    MENU_EXIT
} MenuOption;

/* signatures */ 
void cli();
void create_new_task();
void view_all_tasks();
void create_directory();

int main(int argc, char *argv[]) {

    cli();
    return EXIT_SUCCESS;
}

void cli() {
    while (VIEWING) { //FIXME: infinite loop!
        printf("\nWelcome to your TODO-list, John!\n");
        printf("You currently have 0 tasks to complete!\n");
        printf("\nWould you like to:\n");
        printf("------------------\n");
        printf("1. View current tasks.\n");
        printf("2. Create a new task.\n");
        printf("3. Exit task menu.\n");
        printf("------------------\n");
        int choice;
        printf("Choice: "); scanf("%d", &choice); F;
        switch (choice) {
            case MENU_VIEW:
                printf("Viewing tasks...\n");
                view_all_tasks();
                VIEWING = MAIN_EXIT;
                break;
            case MENU_CREATE:
                printf("Creating a new task...\n");
                create_new_task();
                VIEWING = MAIN_EXIT;
                break;
            case MENU_EXIT:
                VIEWING = MAIN_EXIT;
                printf("Goodbye!\n");
                break;
            default:
                printf("Please enter [1, 2, or 3]");
                break;
        }
    }
}

void create_directory() {
    //check if directory exists
    struct stat st = {0};
    if (stat(task_dir, &st) == -1) {
        mkdir(task_dir, 0700);
    }
}

void view_all_tasks() {
    create_directory();
    int task_count; task_count = 0;
    //count number of files in "tasks/"
    DIR *dir;
    struct dirent *entry;
    struct stat st;

    dir = opendir(task_dir);
    if (dir == NULL) {
        perror("something went wrong. Directory failed to open.\n");
        EXIT_FAILURE;
    }
    while ((entry = readdir(dir)) != NULL) {
        //build full path
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", task_dir, entry->d_name);

        //skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        //check if path is regular file
        if (stat(path, &st) == 0 && S_ISREG(st.st_mode)) {
            task_count++;
        }
    }
    closedir(dir);
    printf("You currently have %d tasks to complete!\n", task_count);
}

void create_new_task() {
    //TODO: create a new task (fiel tint lkuce)
}

