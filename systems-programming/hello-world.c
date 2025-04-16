#include <stdio.h>
#include <unistd.h>

//#define STDOUT_FILENO 1
//#define STDERR_FILENO 2
int main() {
    write(STDOUT_FILENO, "Hello\n", 6);   
    write(STDERR_FILENO, "World\n", 1);   

    printf("Hello World!\n");
    return 0;
}
