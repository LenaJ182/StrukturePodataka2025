#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struktura koja opisuje osobu
typedef struct person {
    char name[50];           // ime osobe
    char surname[50];        // prezime osobe
    int birthYear;           // godina rođenja
    struct person* next;     // pokazivač na sljedeću osobu u listi
} Person;

// deklaracije funkcija 
Person* createPerson(char* name, char* surname, int birthYear);
void addToBeginning(Person** head, char* name, char* surname, int birthYear);
void addToEnd(Person** head, char* name, char* surname, int birthYear);
void printList(Person* head);
Person* findBySurname(Person* head, char* surname);
void deleteBySurname(Person** head, char* surname);

int main() {
    Person* head = NULL; // pokazivac na pocetak liste koja je prazna na početku

    // Ucitavanje podataka iz txt datoteke 
    FILE* file = fopen("osobe.txt", "r"); // otvaramo datoteku za citanje

    if (file == NULL) { // ako se datoteka ne može otvorit
        printf("Greška: ne mogu otvoriti datoteku 'osobe.txt'!\n");
        return 1; // završavamo program jer nema podataka
    }

    char name[50];     // privremena varijabla za ime
    char surname[50];  // privremena varijabla za prezime
    int birthYear;     // privremena varijabla za godinu rođenja

    // čitamo dok ima redaka u datoteci
    while (fscanf(file, "%s %s %d", name, surname, &birthYear) == 3) {
        // dodajemo svaku osobu na kraj liste
        addToEnd(&head, name, surname, birthYear);
    }

    fclose(file); // zatvaramo datoteku jer smo sve učitali

    printf("\n LISTA UCITANA IZ DATOTEKE \n");
    printList(head); // ispis svih učitanih osoba

    //  A: dodavanje nove osobe na početak liste
    printf("\nDodaj novu osobu na pocetak:\n");
    printf("Unesi ime: ");
    scanf("%s", name); // unos imena s tipkovnice
    printf("Unesi prezime: ");
    scanf("%s", surname); // unos prezimena
    printf("Unesi godinu rodenja: ");
    scanf("%d", &birthYear); // unos godine

    addToBeginning(&head, name, surname, birthYear); // dodaj na početak

    //  C: dodavanje nove osobe na kraj liste
    printf("\nDodaj novu osobu na kraj:\n");
    printf("Unesi ime: ");
    scanf("%s", name);
    printf("Unesi prezime: ");
    scanf("%s", surname);
    printf("Unesi godinu rodenja: ");
    scanf("%d", &birthYear);

    addToEnd(&head, name, surname, birthYear); // dodaj na kraj liste

    // ispis cijele liste nakon dodavanja 
    printf("\n LISTA NAKON DODAVANJA \n");
    printList(head);

    //  pronalazak osobe po prezimenu 
    printf("\nUnesi prezime koje zelis pronaci: ");
    scanf("%s", surname); // unos prezimena

    Person* found = findBySurname(head, surname); // traži u listi
    if (found != NULL)
        printf("Pronadena je osoba: %s %s (%d)\n", found->name, found->surname, found->birthYear);
    else
        printf("Osoba nije pronadena\n");

    //  E: brisanje osobe po prezimenu 
    printf("\nUnesi prezime osobe koju zelis obrisati: ");
    scanf("%s", surname);
    deleteBySurname(&head, surname); // briše iz liste

    // ispis liste nakon brisanja 
    printf("\n LISTA NAKON BRISANJA \n");
    printList(head);

    //  oslobađanje memorije 
    // prolazimo kroz cijelu listu i brišemo svaku osobu iz memorije
    while (head != NULL) {
        Person* next = head->next; // zapamti sljedeću osobu prije brisanja
        free(head);                // oslobodi trenutnu osobu
        head = next;               // idi na iduću
    }

    
    return 0;
}


// funkcija koja stvara novu osobu u memoriji
Person* createPerson(char* name, char* surname, int birthYear) {
    Person* newPerson = (Person*)malloc(sizeof(Person)); // alokacija memorije
    if (newPerson == NULL) { // provjera 
        printf("Greska u alokaciji memorije\n");
        return NULL;
    }

    // kopira podatke u novu osobu
    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL; // novi element još nema sljedećeg

    return newPerson;
}

// A. dodavanje na početak liste
void addToBeginning(Person** head, char* name, char* surname, int birthYear) {
    Person* newPerson = createPerson(name, surname, birthYear); // stvori novu osobu
    if (newPerson == NULL) return; // ako je došlo do greške

    newPerson->next = *head; // nova osoba pokazuje na trenutnu prvu osobu
    *head = newPerson;       // nova osoba postaje prva u listi
}

// C. dodavanje na kraj liste
void addToEnd(Person** head, char* name, char* surname, int birthYear) {
    Person* newPerson = createPerson(name, surname, birthYear);
    if (newPerson == NULL) return;

    // ako je lista prazna, nova osoba postaje prva
    if (*head == NULL) {
        *head = newPerson;
        return;
    }

    // inače idemo do zadnje osobe u listi
    Person* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    // nova osoba postaje zadnji element
    current->next = newPerson;
}

// B. ispis cijele liste
void printList(Person* head) {
    if (head == NULL) {
        printf("Lista je prazna\n");
        return;
    }

    Person* current = head;
    while (current != NULL) {
        printf("%s %s (%d)\n", current->name, current->surname, current->birthYear);
        current = current->next;
    }
}

// D. pronalaženje osobe po prezimenu
Person* findBySurname(Person* head, char* surname) {
    Person* current = head;

    // prolazimo cijelu listu i uspoređujemo prezime
    while (current != NULL) {
        if (strcmp(current->surname, surname) == 0) {
            return current; // ako se prezime podudara vraćamo pokazivač
        }
        current = current->next; // idemo na sljedeću osobu
    }

    return NULL; // ako osoba nije pronađena
}

// E. brisanje osobe po prezimenu
void deleteBySurname(Person** head, char* surname) {
    Person* current = *head;   // pokazivač na trenutnu osobu
    Person* previous = NULL;   // pokazivač na prethodnu osobu

    // prolazimo listom dok ne pronađemo prezime koje se podudara
    while (current != NULL && strcmp(current->surname, surname) != 0) {
        previous = current;    // zapamti prethodnu
        current = current->next; // idi na sljedeću
    }

    if (current == NULL) { // ako osoba nije pronađena
        printf("Osoba '%s' nije pronadena\n", surname);
        return;
    }

    // ako je prva osoba u listi ta koju brišemo
    if (previous == NULL)
        *head = current->next; // početak sada pokazuje na drugu osobu
    else
        previous->next = current->next; // preskačemo izbrisanu osobu

    free(current); // oslobađamo memoriju za tu osobu
    printf("Osoba '%s' je obrisana iz liste\n", surname);
}
