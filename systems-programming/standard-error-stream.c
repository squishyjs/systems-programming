#include <unistd.h>

int main(void) {
    for (int len = 7; len > 0; --len) {
        write(STDOUT_FILENO, "I think", len);
        write(STDOUT_FILENO, "\n", 6);
    }
    return 1023;
}
