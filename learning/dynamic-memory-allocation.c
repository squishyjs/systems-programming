#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define F                   fflush(stdout)
#define NAME_BUF_DATA       50

struct Person {
    char name[NAME_BUF_DATA];   /* full name */
    unsigned birth_day;
    unsigned birth_month;
    unsigned birth_year;

};

int main(void) {

    struct Person *person_list;
    unsigned number_of_persons, num;
    char buffer[NAME_BUF_DATA];
    char *p;
    int year, month, day;
    int ok;

    do {
        printf("Enter the number of persons: "); F;
        if ((fgets(buffer, sizeof(buffer), stdin) != NULL) 
            && sscanf(buffer, "%u", &number_of_persons) == 1) {
            ok = 1;
            if (number_of_persons > 0) {
                person_list = malloc(number_of_persons * sizeof(struct Person));
                if (person_list == NULL) {
                    printf("Not enough memory to store that many persons!\n");
                    ok = 0;
                }
            }
        }
        else {
            ok = 0;
            printf("Invalid number! Enter again...\n");
        }
    } while (!ok);

    if (!number_of_persons) {
        printf("OK, perhaps another time\n");
        return EXIT_SUCCESS;
    }

   for (int num = 0; num < number_of_persons; ++num) {
      printf("\nEnter the information for person #%u:\n", num);
      printf("Name: ");
      fflush(stdout);
      if (fgets(buffer, sizeof buffer, stdin) == NULL)
      {
         printf("Error reading from stdin; input aborted\n");
         number_of_persons = num;
         break;
      }
      p = strchr(buffer,'\n');
      if (p!=NULL)
         *p = '\0';
      if (strlen(buffer)==0)
      {
         printf("Input stopped\n");
         number_of_persons = num;
         break;
      }
      strcpy(person_list[num].name, buffer);
      do
      {
         printf("Birthday [YYYY-MM-DD]: ");
         fflush(stdout);
         if (fgets(buffer, sizeof buffer, stdin) != NULL
             && sscanf(buffer, "%d-%d-%d", &year, &month, &day) == 3
             && year>=1000 && year<=9999
             && month>=1 && month<=12
             && day>=1 && day<=31)
         {
            ok = 1;
         }
         else
         {
            ok = 0;
            printf("Invalid birthday! Enter again...\n");
         }
      }
      while (!ok);
      person_list[num].birth_year = year;
      person_list[num].birth_month = month;
      person_list[num].birth_day = day;
   }

   printf("\nOK, thank you.\n");
   printf("\nYou entered the following data:\n");
   printf("\n%-10s%-30s%s\n", "Number", "Name", "Birthday");

   for (int num = 0; num < number_of_persons; num++)
   {
      printf("%-10u%-30s%04d-%02d-%02d\n",
                num,
                person_list[num].name,
                person_list[num].birth_year,
                person_list[num].birth_month,
                person_list[num].birth_day);
   }
    /* free manually allocated memory on the heap */
   free(person_list);
    return EXIT_SUCCESS;
}
