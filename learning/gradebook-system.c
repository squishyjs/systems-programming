#include <complex.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define F                       fflush(stdout)
#define PFLUSH(...)             do { printf(__VA_ARGS__); fflush(stdout); } while (0);
#define FREE(ptr)               do { free(ptr); (ptr) = NULL; } while (0)

#define DB_NAME                 "StudentGradeBookDatabaseSystem"
#define DB_STATUS               "FIXING"
#define INIT_BUF                64
#define MAX_NAME_LEN            100
#define MAX_STUDENTS            1000
#define NUM_SUBJECTS            5
#define VIEWING                 true     

enum CompareFunc { CMP_NAME=1, CMP_ID, CMP_GRADE };
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
    const size_t INIT = 64;             /* start with 64 bytes */
    size_t cap = INIT, len = 0;
    char *buf = malloc(cap);
    if (!buf) return NULL;

    int ch;
    while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {
        if (len + 1 >= cap) {           /* +1 for '\0', appended at the end */
            cap *= 2;
            char *tmp = realloc(buf, cap);
            if (!tmp) { free(buf); return NULL; }
            buf = tmp;
        }
        buf[len++] = (char)ch;
    }

    if (ch == EOF && len == 0) {        /* nothing read → EOF → caller sees NULL */
        free(buf);
        return NULL;
    }
    if (len && buf[len-1] == '\r')      /* strip Windows CR */
        len--;

    buf[len] = '\0';
    return buf;                         /* caller frees */
}

void print_student(Student *student) {
    printf("\nStudent name: %s\n", student->name);
    printf("Student id: %d\n", student->studentID);

    printf("something\n"); puts("using something here");
    printf("Grades: ");
    for (int i = 0; i < NUM_SUBJECTS; ++i) {
        if (i == NUM_SUBJECTS - 1) {
            printf("%.2f\n", student->grades[i]); F;
            continue;
        }
        printf("%.2f, ", student->grades[i]);
    }

    printf("Grade Average: %.2f\n", student->average);
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

    // print student information
    print_student(&new_student);
    
    return new_student;
}

void free_gradebook(GradeBook *grade_book) {
    for (int i = 0; i < grade_book->count; ++i) {
        // free strdup malloc used for name
        free(grade_book->students[i].name);
    }
    // uncomment the below if using dynamic memory
    // free(grade_book->students);
}

void add_student(GradeBook *grade_book) {
    printf("\n==== Adding New Student ====\n");

    grade_book->students[grade_book->count] = new_student(); // create the new student
    grade_book->count++;

}

void view_all_students(GradeBook *grade_book) {
    if (grade_book->count == 0) {
        printf("\nNo student record found in database.\n");
        return;
    }

    float class_total = 0;
    printf("\n================ Summary ================");
    for (int i = 0; i < grade_book->count; ++i) {
        grade_book->student_info(grade_book->students[i]);
        class_total += grade_book->students[i].average;
    }
    printf("=========================================\n");
}

void search_student(GradeBook *grade_book, const char *query_name) {
    // FIXME
    if (!grade_book || !query_name) {
        return;                             /* basic guard */
    }

    for (int i = 0; i < grade_book->count; ++i) {
        if (strcmp(grade_book->students[i].name, query_name) == 0) {
            printf("\nStudent Found.\n");
            print_student(&grade_book->students[i]);

            return;
        }
    }
    printf("\nNo student record found in database.\n");
}

static int compare_by_name(const void *student_1, const void *student_2) {
    const Student *s1 = student_1;
    const Student *s2 = student_2;

    return strcmp(s1->name, s2->name);
}

static int compare_by_id(const void *student_1, const void *student_2) {
    const Student *s1 = student_1;
    const Student *s2 = student_2;

    return (s1->studentID > s2->studentID) - (s1->studentID < s2->studentID);
}

static int comapre_by_grade_desc(const void *student_1, const void *student_2) {
    const Student *s1 = student_1;
    const Student *s2 = student_2;

    // TODO: need to get their GPA over the 5 grades, then compare
    return EXIT_FAILURE;
}

void sort_students_desc(
    GradeBook *grade_book,
    int (*cmp)(const void *, const void *))
{
    // FIXME
    if (!grade_book || grade_book->count == 0) {
        return;
    }
    
    qsort(grade_book->students, grade_book->count, sizeof(Student), cmp);
}

void sort_students_asc(
    GradeBook *grade_book,
    int (*cmp)(const void *, const void *))
{
    // FIXME
    if (!grade_book || grade_book->count == 0) {
        return;
    }
    
    qsort(grade_book->students, grade_book->count, sizeof(Student), cmp);
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
                //FIXME
                char *input_name = read_line();
                search_student(grade_book_db, input_name);
                free(input_name);

                break;
            case SORT_STUDENTS:
                printf("Choose an option between 1 and 3:\n");
                int opt;
                scanf("%d", &opt);
                
                switch (opt) {
                    case CMP_NAME:
                        sort_students_desc(grade_book_db, compare_by_name);
                        break;
                    case CMP_ID:
                        sort_students_desc(grade_book_db, compare_by_id);
                        break;
                    case CMP_GRADE:
                        sort_students_desc(grade_book_db, comapre_by_grade_desc);
                }

                break;

            case SAVE_AND_EXIT:
                PROGRAMMING = false;
                break;
            default:
                printf("Please enter an option between 1 and 5.");
                break;
        }
    }
}

void print_goodbye(void) {
    printf("=============");
    printf("\nGoodbye!\n");
    printf("=============");
}

int main(void) {
    struct GradeBook class_grades;
    class_grades.count = 0;
    // FIXME
    class_grades.student_info = &print_student; // function pointer

    // int num_students = 3;
    // printf("\nGradebook System: Enter info for %d students.\n", num_students);

    // for (int i = 0; i < num_students; ++i) { // get the number of students in the current_database
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
    print_goodbye();

    return EXIT_SUCCESS;
}
