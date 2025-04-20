#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define STDIN 0     //fd for standard input stream

int main(void) {
    struct timeval tv;
    fd_set readfds;
    int retval;

    tv.tv_sec = 2;
    tv.tv_usec = 500000;

    FD_ZERO(&readfds);
    FD_SET(STDIN, &readfds);

    //don't care about writefds & exceptds
    printf("before select \n");
    select(STDIN+1, &readfds, NULL, NULL, &tv);
    printf("after select \n");

    if (retval == -1) {
        perror("select()");
    } else if (retval > 0) {
        //if (FD_ISSET(STDIN, &readfds)) {
        printf("Data is available now.\n");
    } else {
        printf("No data..timedout.\n");
    }

    // if (FD_ISSET(STDIN, &readfds)) {
    //     printf("A key was pressed!\n");
    // } else {
    //     printf("Timed out.\n");
    // }

    return 0;
}
