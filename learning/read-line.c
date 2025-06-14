#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define INIT_BUF            64
char *read_line(void); // function signature 

int main(void) {
    char *test_input;

    if ((test_input = read_line())) { // get the line to be read
        puts(test_input);
        free(test_input);
    }

    // free(test_input);
    return EXIT_SUCCESS;
}

char *read_line(void) {              /* stdin version */
    const size_t INIT = 64;        /* start with 64 bytes */
    size_t cap = INIT, len = 0;
    char *buf = malloc(cap);
    if (!buf) return NULL;

    int ch;
    while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {
        if (len + 1 >= cap) {      /* +1 for final '\0' */
            cap *= 2;
            char *tmp = realloc(buf, cap);
            if (!tmp) { free(buf); return NULL; }
            buf = tmp;
        }
        buf[len++] = (char)ch;
    }

    if (ch == EOF && len == 0) {   /* nothing read & EOF → caller sees NULL */
        free(buf);
        return NULL;
    }
    if (len && buf[len-1] == '\r') /* strip Windows CR */
        len--;

    buf[len] = '\0';
    return buf;                    /* caller frees */
}
