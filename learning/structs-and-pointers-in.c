#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Person {
    char *name;
    int age;
    float height;
    int weight;
};

struct Person *create_person(char *name, int age, float height, int weight) {
    /* create a person that has a name, age, height, and weight */
    struct Person *person = malloc(sizeof(struct Person));
    assert(person != NULL);
    //initialise variables of the person
    person->name = strdup(name); //should be strdup(name)..but why??
    person->age = age;
    person->height = height;
    person->weight = weight;

    return person;
}

void destroy_person(struct Person *person) {
    assert(person != NULL);

    free(person->name);
    free(person);
}

void print_person(struct Person *person) {
    printf("Name is: %s\n", person->name);
    printf("Age is: %d\n", person->age);
    printf("Height is: %f\n", person->height);
    printf("Weight is: %d\n", person->weight);
}

int main(void) {

    struct Person *john = create_person("John Sciuto", 20, 6.2, 85);
    
    printf("%s is at memory location: %p\n", john->name, john);
    print_person(john);
    
    //free memory
    destroy_person(john);
    // destroy_person(NULL);
    return 0;
}
