#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void) {

    int count;
    mode_t mode = S_IRUSR | S_IWUSR;
    int fileDescriptor = open("output.txt", O_CREAT | O_TRUNC | O_RDWR, mode);
    write(fileDescriptor, "Great!\n", 7);
    close(fileDescriptor);

    for (count = 5; count; count--) {
        write(STDOUT_FILENO, "Hello\n", 6);
        write(STDERR_FILENO, ".", 1);
    }

    return 0;
}
