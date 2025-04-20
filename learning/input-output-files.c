#include "dbg.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DATA 100

typedef enum HairColour {
    BLACK_HAIR,
    BROWN_HAIR,
    BLONDE_HAIR,
    SILVER_HAIR,
    NO_HAIR,
    UNIQUE_HAIR
} HairColour;

typedef enum EyeColour {
    BLUE_EYES,
    GREEN_EYES,
    BROWN_EYES,
    BLACK_EYES,
    UNIQUE_EYES
} EyeColour;

const char *EYE_COLOUR_NAMES[] = {
    "Blue", "Green", "Brown", "Black", "Unique"
};
const char *HAIR_COLOUR_NAMES[] = {
    "Black", "Brown", "Blonde", "Silver", "No hair", "Unique"
};

typedef struct Person {
    int age;
    char first_name[MAX_DATA];
    char last_name[MAX_DATA];
    char hobby[MAX_DATA];
    EyeColour eyes;
    HairColour hair;
    float income;

} Person;

void print_results(Person person) {
    printf("----- RESULTS -----\n");
    printf("First Name: %s\n", person.first_name);
    printf("Last Name: %s\n", person.last_name);
    printf("Age: %d\n", person.age);
    printf("Eyes: %s\n", person.eyes);
    printf("Hair: %s\n", person.hair);
    printf("Income: $%f\n", person.income);
}

int main(int argc, char *argv[]) {
    Person john = {.age = 0};
    char *in = NULL;

    printf("What's your First Name? ");
    in = fgets(john.first_name, MAX_DATA - 1, stdin);
    check(in != NULL, "Failed to read first name.");

    printf("What's your Last Name? ");
    in = fgets(john.last_name, MAX_DATA - 1, stdin);
    check(in != NULL, "Failed to read last name.");
    
    printf("How old are you? ");
    int rc = fscanf(stdin, "%d", &john.age);
    check(rc > 0, "Please enter a number greater than 0!");

    printf("What colour are your eyes:\n");
    for (int i = 0; i <= UNIQUE_EYES; ++i) {
        printf("%d) %s\n", i + 1, EYE_COLOUR_NAMES[i]);
    }
    printf("> ");

    int eyes = -1;
    rc = fscanf(stdin, "%d", &eyes);
    check(rc > 0, "You have to enter a number.");
    john.eyes = eyes - 1;
    check(john.eyes <= UNIQUE_EYES
          && john.eyes >= 0, "Not an eye option."
          );

    printf("What colour is your hair:\n");
    for (int i = 0; i <= UNIQUE_HAIR; ++i) {
        printf("%d) %s\n", i + 1, HAIR_COLOUR_NAMES[i]);
    }
    printf("> ");
    /* write the code to check hair colour, using dbg.h */
    int hair = -1;
    rc = fscanf(stdin, "%d", &hair);
    check(rc > 0, "Please enter a number.");
    john.eyes = hair - 1;
    check(john.hair <= UNIQUE_HAIR && john.hair >= 0, "Not a hair option.");
    /* -----rest of code------ */
    printf("How much do you earn an hour?");
    rc = fscanf(stdin, "%f", &john.income);
    check(rc > 0, "Enter a floating point number.");

    print_results(john);

    return 0;
error:
    return -1;
}
