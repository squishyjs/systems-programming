#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define F                   fflush(stdout)
#define INIT_BUF            64
#define MAX_NAME_LEN        100
#define MAX_STUDENTS        1000
#define NUM_SUBJECTS        5
#define VIEWING             true     

enum DatabaseOpts {
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
    int capacity;
    void (*student_info)(Student);
} GradeBook;

char *read_line(void) {
    int size = INIT_BUF;
    char *buffer = malloc(INIT_BUF);
     if (!buffer) {
        return NULL;
    }

    int i = 0;
    int ch;

    while ((ch = getchar() != '\n' && ch != EOF)) {
        if (i >= size - 1) {
            size *= 2;
            char *new_buffer = realloc(buffer, size);
            if (!new_buffer) {
                free(buffer);

                return NULL;
            }
            buffer = new_buffer;
        }
        buffer[i++] = (char)ch;
    }

    buffer[i] = '\0';
    return buffer;      // caller must free input buffer
}

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

void free_gradebook(GradeBook *grade_book) {
    for (int i = 0; i < grade_book->count; ++i) {
        // free strdup malloc
        free(grade_book->students[i].name);
    }
    // uncomment the below if using dynamic memory
    // free(grade_book->students);
}

void add_student(GradeBook *grade_book) {
    printf("\n==== Adding New Student ====\n");

    grade_book->students[grade_book->count] = new_student(); // create the new student (process)
    grade_book->count++;

}

void view_all_students(GradeBook *grade_book) {
    // TODO
    float class_total = 0;
    printf("\n======= Summary =======");
    for (int i = 0; i < grade_book->count; ++i) {
        grade_book->student_info(grade_book->students[i]);
        class_total += grade_book->students[i].average;
    }
    printf("=======================\n");
}

void search_student(GradeBook *grade_book) {
    // TODO
}

void sort_students(GradeBook *grade_book) {
    // TODO
}

void save_exit() {
    // TODO
    // free database memory grade book?
}

void launch_student_database(GradeBook *grade_book_db) {
    bool PROGRAMMING = true;
    printf("\nWelcome to the Persistent Student Database!\n");
    
    
    while (PROGRAMMING) {
        printf("\n");
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
                add_student(grade_book_db);
                break;
            case VIEW_ALL:
                view_all_students(grade_book_db);
                break;
            case SEARCH_STUDENT:
                search_student(grade_book_db);
                break;
            case SORT_STUDENTS:
                sort_students(grade_book_db);
                break;
            case SAVE_AND_EXIT:
                save_exit();
                PROGRAMMING = false;
                break;
            default:
                printf("Please enter an option between 1 and 5.");
                break;
        }
    }
}

int main(void) {
    struct GradeBook class_grades;
    class_grades.count = 0;
    class_grades.student_info = print_student; // function pointer

    // int num_students = 3;
    // printf("\nGradebook System: Enter info for %d students.\n", num_students);

    // for (int i = 0; i < num_students; ++i) {
    //     printf("\n==== Student %d ====\n", i + 1);
    //     class_grades.students[i] = new_student();

    //     class_grades.count++;
    // }

    // float class_total = 0;
    // printf("\n======= Summary =======");
    // for (int i = 0; i < class_grades.count; ++i) {
    //     class_grades.student_info(class_grades.students[i]);
    //     class_total += class_grades.students[i].average;
    // }

    // printf("\nClass Average: %.2f\n", class_total / class_grades.count);
    // printf("=======================\n");

    // free memory
    launch_student_database(&class_grades);
    free_gradebook(&class_grades);
    printf("\nGoodbye!\n");

    return EXIT_SUCCESS;
}
