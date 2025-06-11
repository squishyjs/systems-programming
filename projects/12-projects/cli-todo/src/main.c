#include "../include/dbg.h"
#include "../include/list.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>

#define F               fflush(stdout)
bool VIEWING =          true;
bool MAIN_EXIT =        false;
const char *task_dir =  "./tasks";

typedef enum {
    MENU_VIEW=1,
    MENU_CREATE,
    MENU_DELETE,
    MENU_DEL_ALL,
    MENU_EXIT
} MenuOption;

/* signatures */ 
void cli();
void view_all_tasks();
void create_new_task();
void delete_task();
void delete_all_tasks();
void create_directory();
void clear_input_buffer();
bool task_dir_exists();

int main(int argc, char *argv[]) {
    cli();
    return EXIT_SUCCESS;
}

void cli() {
    while (VIEWING) {
        int task_count = 0;
        
        // Get task count for display
        DIR *dir;
        struct dirent *entry;
        struct stat st;

        dir = opendir(task_dir);
        if (dir != NULL) {
            while ((entry = readdir(dir)) != NULL) {
                char path[1024];
                snprintf(path, sizeof(path), "%s/%s", task_dir, entry->d_name);

                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                    continue;
                }
                
                if (stat(path, &st) == 0 && S_ISREG(st.st_mode)) {
                    task_count++;
                }
            }
            closedir(dir);
        }
        
        printf("\nWelcome to your TODO-list, John!\n");
        printf("You currently have %d tasks to complete!\n", task_count);
        printf("\nWould you like to:\n");
        printf("------------------\n");
        printf("1. View current tasks.\n");
        printf("2. Create a new task.\n");
        printf("3. Delete a task.\n");
        printf("4. Remove all tasks\n");
        printf("5. Exit task menu.\n");
        printf("------------------\n");
        
        int choice;
        printf("Choice: "); 
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer(); F;
        switch (choice) {
            case MENU_VIEW:
                printf("Viewing tasks...\n");
                view_all_tasks();
                break;
            case MENU_CREATE:
                printf("Creating a new task...\n");
                create_new_task();
                break;
            case MENU_DELETE:
                printf("Deleting a task...\n");
                delete_task();
                break;
            case MENU_DEL_ALL:
                printf("All tasks have been deleted!.");
                delete_all_tasks();
                break;
            case MENU_EXIT:
                VIEWING = false;
                printf("Goodbye!\n");
                break;
            default:
                printf("Please enter [1, 2, 3, or 4]\n");
                break;
        }
    }
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void delete_task() {
    create_directory();
    char filename[256];
    printf("Enter the title of the task to delete: "); F;
    clear_input_buffer(); // This was redundant, removed
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s/%s.txt", task_dir, filename);
    int rc = remove(filepath);
    if (rc == 0) {
        printf("Task '%s' deleted successfully.\n", filename);
    } else {
        printf("Failed to delete task: %s\n", filename);
    }
}

bool task_dir_exists() {
    DIR *task_directory = opendir("./tasks");
    if (task_directory) {
        closedir(task_directory);
        return true;
    } else if (ENOENT == errno) {
        return false;
    } else {
        printf("opendir() fialed for some other reason.\n");
        return false;
    }
}

void delete_all_tasks() {
    if (task_dir_exists()) {
        //TODO
    } else {
        printf("No tasks to delete!.\n");
    }
}

void create_directory() {
    //check if directory exists
    struct stat st = {0};
    if (stat(task_dir, &st) == -1) {
        mkdir(task_dir, 0700);
        printf("Created tasks directory.\n");
    }
}

void view_all_tasks() {
    create_directory();
    int task_count = 0;
    
    DIR *dir;
    struct dirent *entry;
    struct stat st;

    dir = opendir(task_dir);
    if (dir == NULL) {
        printf("Directory failed to open.\n");
        return;
    }
    
    printf("\n===== YOUR TASKS =====\n");
    
    while ((entry = readdir(dir)) != NULL) {
        // Build full path
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", task_dir, entry->d_name);

        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Check if path is regular file and ends with .txt
        if (stat(path, &st) == 0 && S_ISREG(st.st_mode)) {
            char *ext = strrchr(entry->d_name, '.');
            if (ext && strcmp(ext, ".txt") == 0) {
                // display task title (w/o .txt)
                char title[256];
                strncpy(title, entry->d_name, strlen(entry->d_name) - 4); // Remove .txt
                title[strlen(entry->d_name) - 4] = '\0';
                
                printf("%d. %s\n", ++task_count, title);
                
                // read and display task status
                FILE *fp = fopen(path, "r");
                if (fp) {
                    char line[1024];
                    while (fgets(line, sizeof(line), fp)) {
                        if (strncmp(line, "Status:", 7) == 0) {
                            printf("   %s", line);
                            break;
                        }
                    }
                    fclose(fp);
                }
            }
        }
    }
    
    closedir(dir);
    if (task_count == 0) {
        printf("No tasks found.\n");
    }
    printf("======================\n");
    printf("You currently have %d tasks to complete!\n", task_count);
}

void create_new_task() {
    create_directory();

    char title[512];
    char description[1024];

    // title
    printf("Enter task title: ");
    F;
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;                    // remove trailing newline

    // description
    printf("Enter task description: ");
    F;
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = 0;        // remove trailing newline

    // build file path
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s/%s.txt", task_dir, title);

    // write to file
    FILE *fp = fopen(filepath, "w");
    if (!fp) {
        perror("Failed to create the task file");
        return;
    }

    fprintf(fp, "Title: %s\n", title);
    fprintf(fp, "Description: %s\n", description);
    fprintf(fp, "Status: Pending\n");

    fclose(fp);
    printf("Task '%s' has been created successfully!\n", title);
}
