#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "_string.h"

int main(int argc, char *argv[]) {
    FILE *file_ptr;
    
    /* open file (creates if !exist) */
    file_ptr = fopen("hello_world.txt", "w");
    
    /* write some text to the file */
    fprintf(file_ptr, "Hello, World!");

    /* close file */
    fclose(file_ptr);

    return EXIT_SUCCESS;
}
