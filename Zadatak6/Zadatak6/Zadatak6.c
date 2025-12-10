#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple structure for one item
typedef struct Item {
    char name[100];
    int quantity;
    double price;
    struct Item* next;
} Item;

// Structure for one receipt
typedef struct Receipt {
    char date[11];      // YYYY-MM-DD
    Item* items;        // linked list of items
    struct Receipt* next;
} Receipt;

// Function declarations
Receipt* loadReceipts(const char* listFile);
Receipt* loadSingle(const char* filename);
void insertReceipt(Receipt** head, Receipt* r);
void insertItem(Item** head, Item* it);

void query(Receipt* receipts);
int earlier(const char* d1, const char* d2);



int main() {
    Receipt* receipts = loadReceipts("racuni.txt");
    query(receipts);
    return 0;
}

// Loads all receipts listed in "racuni.txt"
Receipt* loadReceipts(const char* listFile) {
    FILE* fp = fopen(listFile, "r");
    if (!fp) {
        printf("Unable to open file list.\n");
        exit(1);
    }

    Receipt* head = NULL;
    char filename[100];

    while (fscanf(fp, "%s", filename) == 1) {
        Receipt* r = loadSingle(filename);
        if (r) insertReceipt(&head, r);
    }

    fclose(fp);
    return head;
}

// Loads one receipt file (date + items)
Receipt* loadSingle(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return NULL;

    Receipt* r = malloc(sizeof(Receipt));
    r->items = NULL;
    r->next = NULL;

    // first line is the date
    fscanf(fp, "%s", r->date);

    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        Item* it = malloc(sizeof(Item));
        it->next = NULL;

        // format: name, quantity, price
        sscanf(line, " %[^,], %d, %lf", it->name, &it->quantity, &it->price);

        insertItem(&r->items, it);
    }

    fclose(fp);
    return r;
}

// Insert receipt by date (earlier first)
void insertReceipt(Receipt** head, Receipt* r) {
    if (*head == NULL || earlier(r->date, (*head)->date)) {
        r->next = *head;
        *head = r;
        return;
    }

    Receipt* p = *head;
    while (p->next && !earlier(r->date, p->next->date))
        p = p->next;

    r->next = p->next;
    p->next = r;
}

// Insert item alphabetically
void insertItem(Item** head, Item* it) {
    if (*head == NULL || strcmp(it->name, (*head)->name) < 0) {
        it->next = *head;
        *head = it;
        return;
    }

    Item* p = *head;
    while (p->next && strcmp(it->name, p->next->name) > 0)
        p = p->next;

    it->next = p->next;
    p->next = it;
}

// Returns 1 if d1 comes before d2
int earlier(const char* d1, const char* d2) {
    return strcmp(d1, d2) < 0;
}

// User query: how much money spent on an item in a date range?
void query(Receipt* receipts) {
    char itemName[100], dateFrom[11], dateTo[11];

    printf("Item name: ");
    scanf(" %99[^\n]", itemName);

    printf("Date from (YYYY-MM-DD): ");
    scanf("%s", dateFrom);

    printf("Date to   (YYYY-MM-DD): ");
    scanf("%s", dateTo);

    double totalMoney = 0;
    int totalQty = 0;

    Receipt* r = receipts;
    while (r) {
        // check date range
        if (strcmp(r->date, dateFrom) >= 0 && strcmp(r->date, dateTo) <= 0) {

            Item* it = r->items;
            while (it) {
                if (strcmp(it->name, itemName) == 0) {
                    totalMoney += it->quantity * it->price;
                    totalQty += it->quantity;
                }
                it = it->next;
            }
        }
        r = r->next;
    }

    printf("\nTotal money spent: %.2f EUR\n", totalMoney);
    printf("Total quantity bought: %d\n\n", totalQty);
}
