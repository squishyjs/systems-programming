#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define F                   fflush(stdout)
#define MAX_NAME_LEN        100
#define MAX_STUDENTS        1000
#define NUM_SUBJECTS        5
#define VIEWING             true     

enum DatabaseMenu {
    ADD_STUDENT=1,
    VIEW_ALL,
    SEARCH_STUDENT,
    SORT_STUDENTS,
    SAVE_AND_EXIT
};

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

bool invalid_grade(float grade) {
    if (grade < 0 || grade > 100) {
        return true;
    }
    return false;
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
        grade = new_student.grades[i];
        while (invalid_grade(grade)) {
            printf("Please enter a grade between (0-100).\n");
            printf("Grade %d: ", i + 1);
            if (scanf("%f", &new_student.grades[i]) != 1) {
                fprintf(stderr, "Something went wrong. Invalid grade input.\n");
                exit(EXIT_FAILURE);
            }
            grade = new_student.grades[i];
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
        // free strdup malloc
        free(gb->students[i].name);
    }
}

void add_student() {
    // TODO
}

void view_all_students() {
    // TODO
}

void search_student() {
    // TODO
}

void sort_students() {
    // TODO
}

void save_exit() {
    // TODO
}

void launch_student_database() {
    printf("\nWelcome to the Persistent Student Database!\n");
    bool BROWSING = true;

    printf("1. Add Student\n");
    printf("2. View All Students\n");
    printf("3. Search Student (by name or ID)\n");
    printf("4. Sort Students (by average or name)\n");
    printf("5. Save & Exit\n\n");
    printf("> ");

    int option;
    if (scanf("%d", &option) != 1) {
        fprintf(stderr, "Something went wrong. Invalid grade input.\n");
        exit(EXIT_FAILURE);
    }

    switch (option) {
        case ADD_STUDENT:
            add_student();
            break;
        case VIEW_ALL:
            view_all_students();
            break;
        case SEARCH_STUDENT:
            search_student();
            break;
        case SORT_STUDENTS:
            sort_students();
            break;
        case SAVE_AND_EXIT:
            save_exit();
            break;
        default:
            printf("Please enter (1-5).");
            break;
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
    printf("\n======= Summary =======");
    for (int i = 0; i < class_grades.count; ++i) {
        class_grades.student_info(class_grades.students[i]);
        class_total += class_grades.students[i].average;
    }

    printf("\nClass Average: %.2f\n", class_total / class_grades.count);
    printf("\n=======================");

    // free memory
    clean_book(&class_grades);
    
    // launch_student_database();

    return EXIT_SUCCESS;
}
