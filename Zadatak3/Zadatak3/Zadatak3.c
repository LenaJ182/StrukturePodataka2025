#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structure that describes one person
typedef struct person {
    char name[50];          // person's first name
    char surname[50];       // person's last name
    int birthYear;          // birth year
    struct person* next;    // pointer to the next person in list
} Person;

// function declarations
Person* createPerson(char* name, char* surname, int birthYear);
void addToBeginning(Person** head, char* name, char* surname, int birthYear);
void addToEnd(Person** head, char* name, char* surname, int birthYear);
void printList(Person* head);
Person* findBySurname(Person* head, char* surname);
void deleteBySurname(Person** head, char* surname);
void addAfter(Person* head, char* targetSurname, char* name, char* surname, int birthYear);
void addBefore(Person** head, char* targetSurname, char* name, char* surname, int birthYear);
void sortListBySurname(Person** head);
void writeListToFile(Person* head, const char* fileName);
void readListFromFile(Person** head, const char* fileName);
void freeList(Person** head);

int main() {
    Person* head = NULL;  // start of list

    char name[50], surname[50], target[50];
    int birthYear;

    //READ FROM FILE 
    readListFromFile(&head, "osobe.txt");

    printf("\n LIST LOADED FROM FILE \n");
    printList(head);

    //  A. add to beginning
    printf("\nAdd new person to BEGINNING:\n");
    printf("Enter name: ");
    scanf("%s", name);
    printf("Enter surname: ");
    scanf("%s", surname);
    printf("Enter birth year: ");
    scanf("%d", &birthYear);

    addToBeginning(&head, name, surname, birthYear);

    //  C. add to end 
    printf("\nAdd new person to END:\n");
    printf("Enter name: ");
    scanf("%s", name);
    printf("Enter surname: ");
    scanf("%s", surname);
    printf("Enter birth year: ");
    scanf("%d", &birthYear);

    addToEnd(&head, name, surname, birthYear);

    // print list after adding
    printf("\n LIST AFTER ADDING \n");
    printList(head);

    //  D. find person by surname 
    printf("\nEnter surname to find: ");
    scanf("%s", surname);

    Person* found = findBySurname(head, surname);
    if (found != NULL)
        printf("Found: %s %s (%d)\n", found->name, found->surname, found->birthYear);
    else
        printf("Person not found.\n");

    //  E. delete person by surname
    printf("\nEnter surname to delete: ");
    scanf("%s", surname);
    deleteBySurname(&head, surname);

    printf("\n LIST AFTER DELETION \n");
    printList(head);

    // NEW PART  

    //  A. add after specific person 
    printf("\nAdd new person AFTER someone:\n");
    printf("After which surname: ");
    scanf("%s", target);
    printf("Enter name: ");
    scanf("%s", name);
    printf("Enter surname: ");
    scanf("%s", surname);
    printf("Enter birth year: ");
    scanf("%d", &birthYear);
    addAfter(head, target, name, surname, birthYear);

    // B. add before specific person 
    printf("\nAdd new person BEFORE someone:\n");
    printf("Before which surname: ");
    scanf("%s", target);
    printf("Enter name: ");
    scanf("%s", name);
    printf("Enter surname: ");
    scanf("%s", surname);
    printf("Enter birth year: ");
    scanf("%d", &birthYear);
    addBefore(&head, target, name, surname, birthYear);

    //  C. sort list by surnames (bubble sort) 
    printf("\nSorting list alphabetically by surnames...\n");
    sortListBySurname(&head);

    printf("\n SORTED LIST \n");
    printList(head);

    // D. write list to file 
    writeListToFile(head, "nova_lista.txt");

    //  E. read list again from file 
    printf("\nReloading list from file 'nova_lista.txt'...\n");
    freeList(&head);
    readListFromFile(&head, "nova_lista.txt");

    printf("\n RELOADED LIST \n");
    printList(head);

    //  free all memory
    freeList(&head);
    printf("\nAll done.\n");

    return 0;
}

// create new person
Person* createPerson(char* name, char* surname, int birthYear) {
    Person* newP = (Person*)malloc(sizeof(Person));
    if (!newP) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    strcpy(newP->name, name);
    strcpy(newP->surname, surname);
    newP->birthYear = birthYear;
    newP->next = NULL;
    return newP;
}

// add to beginning
void addToBeginning(Person** head, char* name, char* surname, int birthYear) {
    Person* newP = createPerson(name, surname, birthYear);
    if (!newP) return;
    newP->next = *head;
    *head = newP;
}

// add to end
void addToEnd(Person** head, char* name, char* surname, int birthYear) {
    Person* newP = createPerson(name, surname, birthYear);
    if (!newP) return;

    if (*head == NULL) {
        *head = newP;
        return;
    }

    Person* q = *head;
    while (q->next != NULL)
        q = q->next;

    q->next = newP;
}

// print list
void printList(Person* head) {
    if (!head) {
        printf("List is empty.\n");
        return;
    }

    Person* q = head;
    while (q != NULL) {
        printf("%s %s (%d)\n", q->name, q->surname, q->birthYear);
        q = q->next;
    }
}

// find by surname
Person* findBySurname(Person* head, char* surname) {
    Person* q = head;
    while (q != NULL) {
        if (strcmp(q->surname, surname) == 0)
            return q;
        q = q->next;
    }
    return NULL;
}

// delete by surname
void deleteBySurname(Person** head, char* surname) {
    Person* q = *head;
    Person* prev = NULL;

    while (q != NULL && strcmp(q->surname, surname) != 0) {
        prev = q;
        q = q->next;
    }

    if (q == NULL) {
        printf("Person not found.\n");
        return;
    }

    if (prev == NULL)
        *head = q->next;
    else
        prev->next = q->next;

    free(q);
    printf("Person deleted.\n");
}

// add AFTER certain surname (using find function)
void addAfter(Person* head, char* targetSurname, char* name, char* surname, int birthYear) {
    Person* q = findBySurname(head, targetSurname); // use previous D function
    if (q == NULL) {
        printf("Target surname not found.\n");
        return;
    }

    Person* newP = createPerson(name, surname, birthYear);
    if (!newP) return;

    // connect new node
    newP->next = q->next;
    q->next = newP;
}

// add BEFORE certain surname
void addBefore(Person** head, char* targetSurname, char* name, char* surname, int birthYear) {
    Person* newP = createPerson(name, surname, birthYear);
    if (!newP) return;

    Person* q = *head;
    Person* prev = NULL;

    while (q != NULL && strcmp(q->surname, targetSurname) != 0) {
        prev = q;
        q = q->next;
    }

    if (q == NULL) {
        printf("Target surname not found.\n");
        free(newP);
        return;
    }

    if (prev == NULL) {
        newP->next = *head;
        *head = newP;
    }
    else {
        newP->next = q;
        prev->next = newP;
    }
}

// sort by surname (bubble sort, swapping nodes)
void sortListBySurname(Person** head) {
    if (*head == NULL || (*head)->next == NULL) return;

    int swapped;
    Person* end = NULL;

    do {
        swapped = 0;
        Person* q = *head;
        Person* prev = NULL;

        while (q->next != end) {
            if (strcmp(q->surname, q->next->surname) > 0) {
                Person* tmp = q->next;
                q->next = tmp->next;
                tmp->next = q;

                if (prev == NULL)
                    *head = tmp;
                else
                    prev->next = tmp;

                prev = tmp;
                swapped = 1;
            }
            else {
                prev = q;
                q = q->next;
            }
        }
        end = q;
    } while (swapped);
}

// write list to file
void writeListToFile(Person* head, const char* fileName) {
    FILE* fp = fopen(fileName, "w");
    if (!fp) {
        printf("Cannot open file for writing.\n");
        return;
    }

    Person* q = head;
    while (q != NULL) {
        fprintf(fp, "%s %s %d\n", q->name, q->surname, q->birthYear);
        q = q->next;
    }

    fclose(fp);
    printf("List saved to '%s'.\n", fileName);
}

// read list from file
void readListFromFile(Person** head, const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        printf("File '%s' not found. Starting empty list.\n", fileName);
        return;
    }

    char name[50], surname[50];
    int year;

    while (fscanf(fp, "%s %s %d", name, surname, &year) == 3)
        addToEnd(head, name, surname, year);

    fclose(fp);
}

// free all memory
void freeList(Person** head) {
    Person* q = *head;
    while (q != NULL) {
        Person* next = q->next;
        free(q);
        q = next;
    }
    *head = NULL;
}
