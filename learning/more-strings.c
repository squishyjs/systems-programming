#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    
void different_ways_to_modify_string() {
    /* heap allocated string (always check for NULL and call free() later) */
    char *str = malloc(6);
    strcpy(str, "Hello");
    
    /* safest way to initialise stirng */
    const char *safe_string = "Hello";
    for (int i = 0; safe_string[i] != '\0'; ++i) {
        printf("Character %d: %c\n", i, safe_string[i]);
    }
    
    /* character arrays with literals (modifiable) */ 
    char string_array[] = "Hi there!";

    free(str);
}

void strings_program() {
    char s[20];

    strcpy(s, "strings");

    printf("s = %s\n", s);
    printf("s[3] = %c\n", s[3]);
    printf("s[7] = %c\n", s[7]);
    printf("strlen(s) = %lu\n", (unsigned long)strlen(s));

    strcat(s, " program");
    printf("s now = %s\n", s);
}

int main(void) {
    const char *myString = "John Sciuto";
    printf("%s\n", myString);

    size_t myStringLength = strlen(myString);
    printf("Length of string is: %zu\n", myStringLength);
    //call program
    different_ways_to_modify_string();
    return 0;
}
