#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define F                   fflush(stdout)
#define MAX_NAME_LEN        100
#define MAX_STUDENTS        1000
#define NUM_SUBJECTS        5

typedef struct Student {
    char *name;
    int studentID;
    float grades[NUM_SUBJECTS];
    float average;
} Student;

typedef struct GradeBook {
    Student students[MAX_STUDENTS];
    int count;
    void (*student_info)(Student);
} GradeBook;

void print_student(Student student) {
    //FIXME: function not calculating the grades correctly
    printf("\nStudent name: %s\n", student.name);
    printf("Student id: %d\n", student.studentID);

    printf("Grades: ");
    for (int i = 0; i < NUM_SUBJECTS; ++i) {
        if (i == NUM_SUBJECTS - 1) {
            printf("%.2f\n", student.grades[i]); F;
            continue;
        }
        printf("%.2f, ", student.grades[i]);
    }

    printf("Grade Average: %.2f\n", student.average);
}

Student new_student() {
    Student new_student;
    char student_name[MAX_NAME_LEN];

    // name input
    printf("Enter student name: ");
    scanf("%99s", student_name);    // no spaces (full names)

    new_student.name = strdup(student_name);

    // student ID input
    printf("Enter student id: ");
    if (scanf("%d", &new_student.studentID) != 1) {
        fprintf(stderr, "Invalid studnet ID input. Leaving function.\n");
        exit(EXIT_FAILURE);
    }

    // grade input
    float average = 0;
    printf("Entering Grades...\n");
    for (int i = 0; i < NUM_SUBJECTS; ++i) {
        float grade;
        printf("Grade %d: ", i + 1);
        if (scanf("%f", &new_student.grades[i]) != 1) {
            fprintf(stderr, "Something went wrong. Invalid grade input.\n");
            exit(EXIT_FAILURE);
        }

        average += new_student.grades[i];
    }
    
    average = average / NUM_SUBJECTS;
    new_student.average = average;

    print_student(new_student);
    
    return new_student;
}

void clean_book(GradeBook *gb) {
    for (int i = 0; i < gb->count; ++i) {
        free(gb->students[i].name); // free strdup names
    }
}

int main(void) {
    struct GradeBook class_grades;
    class_grades.count = 0;
    class_grades.student_info = print_student; // function pointer
    
    int num_students = 3;
    printf("Gradebook System: Enter info for %d students.\n", num_students);

    for (int i = 0; i < num_students; ++i) {
        printf("\n==== Student %d ====\n", i + 1);
        class_grades.students[i] = new_student();

        class_grades.count++;
    }

    float class_total = 0;
    for (int i = 0; i < class_grades.count; ++i) {
        class_grades.student_info(class_grades.students[i]);
        class_total += class_grades.students[i].average;
    }

    printf("\nClass Average: %.2f\n", class_total / class_grades.count);

    // memory free
    clean_book(&class_grades);

    return EXIT_SUCCESS;
}
