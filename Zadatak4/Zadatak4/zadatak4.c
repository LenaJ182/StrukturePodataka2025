#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct term {
    int coef;
    int exp;
    struct term* next;
} Term;

// Function prototypes
Term* readPolynomialFromFile(FILE* file);
void insertSorted(Term** head, int coef, int exp);
Term* addPolynomials(Term* p1, Term* p2);
Term* multiplyPolynomials(Term* p1, Term* p2);
void printPolynomial(Term* head);
void freePolynomial(Term* head);

int main() {
    FILE* file = fopen("polinomi.txt", "r");
    if (!file) {
        printf("Cannot open file.\n");
        return -1;
    }

    printf("Reading polynomials from file...\n");

    Term* P1 = readPolynomialFromFile(file);
    Term* P2 = readPolynomialFromFile(file);

    printf("P1(x) = ");
    printPolynomial(P1);
    printf("P2(x) = ");
    printPolynomial(P2);

    Term* sum = addPolynomials(P1, P2);
    printf("P1(x) + P2(x) = ");
    printPolynomial(sum);

    Term* product = multiplyPolynomials(P1, P2);
    printf("P1(x) * P2(x) = ");
    printPolynomial(product);

    freePolynomial(P1);
    freePolynomial(P2);
    freePolynomial(sum);
    freePolynomial(product);

    fclose(file);
    return 0;
}

// Reads polynomial until empty line
Term* readPolynomialFromFile(FILE* file) {
    Term* head = NULL;
    int coef, exp;
    while (fscanf(file, "%d %d", &coef, &exp) == 2) {
        insertSorted(&head, coef, exp);
    }
    return head;
}

// Inserts term in sorted order (descending exponent)
void insertSorted(Term** head, int coef, int exp) {
    Term* newNode = (Term*)malloc(sizeof(Term));
    newNode->coef = coef;
    newNode->exp = exp;
    newNode->next = NULL;

    if (!*head || (*head)->exp < exp) {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    Term* current = *head;
    while (current->next && current->next->exp > exp)
        current = current->next;

    if (current->next && current->next->exp == exp) {
        current->next->coef += coef;
        free(newNode);
    }
    else {
        newNode->next = current->next;
        current->next = newNode;
    }
}

Term* addPolynomials(Term* p1, Term* p2) {
    Term* result = NULL;
    while (p1) {
        insertSorted(&result, p1->coef, p1->exp);
        p1 = p1->next;
    }
    while (p2) {
        insertSorted(&result, p2->coef, p2->exp);
        p2 = p2->next;
    }
    return result;
}

Term* multiplyPolynomials(Term* p1, Term* p2) {
    Term* result = NULL;
    for (Term* t1 = p1; t1; t1 = t1->next) {
        for (Term* t2 = p2; t2; t2 = t2->next) {
            insertSorted(&result, t1->coef * t2->coef, t1->exp + t2->exp);
        }
    }
    return result;
}

void printPolynomial(Term* head) {
    if (!head) {
        printf("0\n");
        return;
    }

    while (head) {
        printf("%dx^%d", head->coef, head->exp);
        head = head->next;
        if (head) printf(" + ");
    }
    printf("\n");
}

void freePolynomial(Term* head) {
    while (head) {
        Term* temp = head;
        head = head->next;
        free(temp);
    }
}
