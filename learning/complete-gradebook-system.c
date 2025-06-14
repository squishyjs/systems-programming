#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

/* ─── handy macros ─────────────────────────────────────────────── */
#define F                   fflush(stdout)
#define PFLUSH(...)         do { printf(__VA_ARGS__); fflush(stdout); } while (0)

#define DB_NAME             "StudentGradeBookDatabaseSystem"
#define DB_STATUS           "FIXING"
#define INIT_BUF            64
#define MAX_NAME_LEN        100
#define MAX_STUDENTS        1000
#define NUM_SUBJECTS        5
#define VIEWING             true

enum CompareFunc   { CMP_NAME = 1, CMP_ID, CMP_GRADE };
enum DatabaseOpts  { ADD_STUDENT = 1, VIEW_ALL, SEARCH_STUDENT,
                     SORT_STUDENTS, SAVE_AND_EXIT };

/* ─── data structs ─────────────────────────────────────────────── */
typedef struct Student {
    char  *name;
    int    studentID;
    float  grades[NUM_SUBJECTS];
    float  average;
} Student;

typedef struct GradeBook {
    Student students[MAX_STUDENTS];
    int     count;
    int     capacity;          /* (unused but kept for style consistency) */
    void  (*student_info)(Student *);
} GradeBook;

/* ─── helper: read an entire line incl. spaces ─────────────────── */
static char *read_line(void)
{
    const size_t INIT = 64;
    size_t cap = INIT, len = 0;
    char *buf = malloc(cap);
    if (!buf) return NULL;

    int ch;
    while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {
        if (len + 1 >= cap) {
            cap *= 2;
            char *tmp = realloc(buf, cap);
            if (!tmp) { free(buf); return NULL; }
            buf = tmp;
        }
        buf[len++] = (char)ch;
    }

    if (ch == EOF && len == 0) {        /* immediate EOF → NULL */
        free(buf);
        return NULL;
    }
    if (len && buf[len - 1] == '\r')    /* trim CR on Windows */
        --len;

    buf[len] = '\0';
    return buf;                         /* caller frees         */
}

/* ─── printing helpers ─────────────────────────────────────────── */
static void print_student(Student *st)
{
    printf("\nStudent name: %s\n", st->name);
    printf("Student id: %d\n", st->studentID);

    printf("Grades: ");
    for (int i = 0; i < NUM_SUBJECTS; ++i) {
        printf("%.2f%s", st->grades[i],
               (i == NUM_SUBJECTS - 1) ? "\n" : ", ");
    }
    printf("Grade Average: %.2f\n", st->average);
    F;
}

/* ─── business logic helpers ───────────────────────────────────── */
static bool invalid_grade(float g)      { return g < 0.0f || g > 100.0f; }

/* one fresh record, fully validated */
static Student new_student(void)
{
    Student st = {0};
    float sum = 0.0f;

    /* name (allow spaces) */
    printf("Enter student name: ");
    getchar();                          /* eat newline from previous scanf */
    st.name = read_line();
    if (!st.name) {
        fprintf(stderr, "Failed to read name.\n");
        exit(EXIT_FAILURE);
    }

    /* id */
    printf("Enter student id: ");
    if (scanf("%d", &st.studentID) != 1) {
        fprintf(stderr, "Invalid student ID.\n");
        exit(EXIT_FAILURE);
    }

    /* grades */
    puts("Entering grades (0-100) …");
    for (int i = 0; i < NUM_SUBJECTS; ++i) {
        float g;
        do {
            printf("Grade %d: ", i + 1);
            if (scanf("%f", &g) != 1) {
                fprintf(stderr, "Invalid grade input.\n");
                exit(EXIT_FAILURE);
            }
        } while (invalid_grade(g));
        st.grades[i] = g;
        sum += g;
    }
    st.average = sum / NUM_SUBJECTS;

    print_student(&st);
    return st;
}

/* free strdup/read_line allocations */
static void free_gradebook(GradeBook *gb)
{
    for (int i = 0; i < gb->count; ++i)
        free(gb->students[i].name);
}

/* CRUD ops */
static void add_student(GradeBook *gb)
{
    puts("\n==== Adding New Student ====");
    gb->students[gb->count++] = new_student();
}

static void view_all_students(GradeBook *gb)
{
    if (gb->count == 0) {
        puts("\nNo student record found in database.");
        return;
    }

    float total = 0.0f;
    puts("\n================ Summary ================");
    for (int i = 0; i < gb->count; ++i) {
        gb->student_info(&gb->students[i]);
        total += gb->students[i].average;
    }
    printf("Class Average: %.2f\n", total / gb->count);
    puts("=========================================");
}

static void search_student(GradeBook *gb, const char *name)
{
    if (!name || !gb) return;

    for (int i = 0; i < gb->count; ++i)
        if (strcmp(gb->students[i].name, name) == 0) {
            printf("\nStudent Found.");
            print_student(&gb->students[i]);
            return;
        }
    puts("\nNo student record found in database.");
}

/* ─── comparators for qsort ─────────────────────────────────────── */
static int cmp_name(const void *a, const void *b) {
    const Student *s1 = (const Student *)a;
    const Student *s2 = (const Student *)b;

    return strcmp(s1->name, s2->name);
}

static int cmp_id(const void *a, const void *b) {
    const Student *s1 = (const Student *)a;
    const Student *s2 = (const Student *)b;

    return (s1->studentID > s2->studentID) - (s1->studentID < s2->studentID);
}

static int cmp_grade_desc(const void *a, const void *b) {
    const Student *s1 = (const Student *)a;
    const Student *s2 = (const Student *)b;

    return (s2->average > s1->average) - (s2->average < s1->average);
}

/* thin wrappers around qsort */
static void sort_students(GradeBook *gb,
                          int (*cmp)(const void *, const void *))
{
    if (gb && gb->count > 0)
        qsort(gb->students, gb->count, sizeof(Student), cmp);
}

/* ─── interactive loop ─────────────────────────────────────────── */
static void launch_student_database(GradeBook *gb)
{
    puts("\nWelcome to the Persistent Student Database!");

    bool running = true;
    while (running) {
        puts("\n1. Add Student");
        puts("2. View All Students");
        puts("3. Search Student (by name)");
        puts("4. Sort Students (by average or name)");
        puts("5. Save & Exit\n");
        printf("> ");

        int opt;
        if (scanf("%d", &opt) != 1) { fputs("Invalid option.\n", stderr); exit(1); }

        switch (opt) {
            case ADD_STUDENT:
                add_student(gb);
                break;

            case VIEW_ALL:
                view_all_students(gb);
                break;

            case SEARCH_STUDENT: {
                printf("Enter name to search: ");
                getchar();                      /* flush trailing '\n'        */
                char *input = read_line();
                search_student(gb, input);
                free(input);
                break;
            }

            case SORT_STUDENTS: {
                puts("Sort by: 1-Name  2-ID  3-Average");
                int k;  scanf("%d", &k);
                switch (k) {
                    case CMP_NAME:  sort_students(gb, cmp_name);        break;
                    case CMP_ID:    sort_students(gb, cmp_id);          break;
                    case CMP_GRADE: sort_students(gb, cmp_grade_desc);  break;
                    default:        puts("Invalid sort key.");          break;
                }
                break;
            }

            case SAVE_AND_EXIT:
                running = false;
                break;

            default:
                puts("Please enter 1-5.");
        }
    }
}

static void print_goodbye(void)
{
    puts("\n=============\nGoodbye!\n=============");
}

/* ─── main ──────────────────────────────────────────────────────── */
int main(void) {
    // instantiate student gradebook
    GradeBook gb = { .count = 0, .student_info = print_student };

    launch_student_database(&gb);
    free_gradebook(&gb);
    print_goodbye();
    return EXIT_SUCCESS;
}
