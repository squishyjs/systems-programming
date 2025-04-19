#include <stdio.h>

typedef enum computer_parts {
    CPU,
    RAM,
    GPU,
    PSU,
    MOTHERBOARD,
    CABLES,
    CASE,
    COOLER
} Computer_specifier;

//alias "enum colours" to -> colour_type
typedef enum colours colour_type;
enum colours {
    RED, 
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    INDIGO,
    VIOLET,
    NUMBER_OF_COLOURS
};

int main(void) {
    colour_type sky, forest;

    printf("There are %d colours in the enum\n", NUMBER_OF_COLOURS);
    
    sky = BLUE;
    forest = GREEN;

    printf("sky = %d\n", (int)sky);
    printf("forest = %d\n", (int)forest);

}
