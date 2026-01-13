#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 128

// STRUCT DEFINITIONS
typedef struct City* CityPosition;
typedef struct Country* CountryPosition;

struct City {
    char name[MAX_NAME];
    int population;
    CityPosition left;
    CityPosition right;
    CityPosition next;
};

struct Country {
    char name[MAX_NAME];
    char fileName[MAX_NAME];
    CityPosition cityRoot;   // model A
    CityPosition cityList;   // model B
    CountryPosition left;
    CountryPosition right;
    CountryPosition next;
};

// ========================
// CITY FUNCTIONS
// ========================

CityPosition createCity(char* name, int population)
{
    CityPosition c = (CityPosition)malloc(sizeof(struct City));
    strcpy(c->name, name);
    c->population = population;
    c->left = c->right = c->next = NULL;
    return c;
}

// Insert city into tree (model A)
CityPosition insertCityTree(CityPosition root, CityPosition newCity)
{
    if (!root) return newCity;

    if (newCity->population < root->population ||
        (newCity->population == root->population && strcmp(newCity->name, root->name) < 0))
        root->left = insertCityTree(root->left, newCity);
    else
        root->right = insertCityTree(root->right, newCity);

    return root;
}

// Insert city into sorted list (model B)
CityPosition insertCityList(CityPosition head, CityPosition newCity)
{
    if (!head ||
        newCity->population < head->population ||
        (newCity->population == head->population && strcmp(newCity->name, head->name) < 0))
    {
        newCity->next = head;
        return newCity;
    }

    CityPosition temp = head;
    while (temp->next &&
        (temp->next->population < newCity->population ||
            (temp->next->population == newCity->population &&
                strcmp(temp->next->name, newCity->name) < 0)))
        temp = temp->next;

    newCity->next = temp->next;
    temp->next = newCity;

    return head;
}

// Free city tree
int freeCityTree(CityPosition root)
{
    if (!root) return 0;
    freeCityTree(root->left);
    freeCityTree(root->right);
    free(root);
    return 0;
}

// Free city list
int freeCityList(CityPosition head)
{
    CityPosition temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}

// ========================
// COUNTRY FUNCTIONS
// ========================

CountryPosition createCountry(char* name, char* file)
{
    CountryPosition c = (CountryPosition)malloc(sizeof(struct Country));
    strcpy(c->name, name);
    strcpy(c->fileName, file);
    c->cityRoot = NULL;
    c->cityList = NULL;
    c->left = c->right = c->next = NULL;
    return c;
}

// Insert country into sorted list (model A)
CountryPosition insertCountryList(CountryPosition head, CountryPosition newCountry)
{
    if (!head || strcmp(newCountry->name, head->name) < 0) {
        newCountry->next = head;
        return newCountry;
    }

    CountryPosition temp = head;
    while (temp->next && strcmp(temp->next->name, newCountry->name) < 0)
        temp = temp->next;

    newCountry->next = temp->next;
    temp->next = newCountry;
    return head;
}

// Insert country into tree (model B)
CountryPosition insertCountryTree(CountryPosition root, CountryPosition newCountry)
{
    if (!root) return newCountry;

    if (strcmp(newCountry->name, root->name) < 0)
        root->left = insertCountryTree(root->left, newCountry);
    else
        root->right = insertCountryTree(root->right, newCountry);

    return root;
}

// Free country list with city trees
int freeCountryListWithTrees(CountryPosition head)
{
    CountryPosition temp;
    while (head) {
        freeCityTree(head->cityRoot);
        temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}

// Free country tree with city lists
int freeCountryTreeWithLists(CountryPosition root)
{
    if (!root) return 0;
    freeCountryTreeWithLists(root->left);
    freeCountryTreeWithLists(root->right);
    freeCityList(root->cityList);
    free(root);
    return 0;
}

// ========================
// FILE LOADING
// ========================

int loadCities(CountryPosition country, int mode)
{
    FILE* f = fopen(country->fileName, "r");
    if (!f) return -1;

    char name[MAX_NAME];
    int population;

    while (fscanf(f, " %[^,],%d", name, &population) == 2)
    {
        CityPosition c = createCity(name, population);
        if (mode == 1)
            country->cityRoot = insertCityTree(country->cityRoot, c);
        else
            country->cityList = insertCityList(country->cityList, c);
    }

    fclose(f);
    return 0;
}

// Recursive loading of all cities in tree (model B)
void loadCitiesTree(CountryPosition root, int mode)
{
    if (!root) return;
    loadCities(root, mode);
    loadCitiesTree(root->left, mode);
    loadCitiesTree(root->right, mode);
}

// ========================
// PRINT FUNCTIONS
// ========================

int printCityTree(CityPosition root)
{
    if (!root) return 0;
    printCityTree(root->left);
    printf("   %s (%d)\n", root->name, root->population);
    printCityTree(root->right);
    return 0;
}

int printCityList(CityPosition head)
{
    while (head) {
        printf("   %s (%d)\n", head->name, head->population);
        head = head->next;
    }
    return 0;
}

int printCountryList(CountryPosition head)
{
    while (head) {
        printf("\n%s\n", head->name);
        printCityTree(head->cityRoot);
        head = head->next;
    }
    return 0;
}

int printCountryTree(CountryPosition root)
{
    if (!root) return 0;
    printCountryTree(root->left);
    printf("\n%s\n", root->name);
    printCityList(root->cityList);
    printCountryTree(root->right);
    return 0;
}

// ========================
// SEARCH FUNCTIONS
// ========================

// Find country in tree
CountryPosition findCountry(CountryPosition root, char* name)
{
    if (!root) return NULL;
    int cmp = strcmp(name, root->name);
    if (cmp == 0) return root;
    if (cmp < 0) return findCountry(root->left, name);
    return findCountry(root->right, name);
}

// Search in tree (model A)
int searchCityTree(CityPosition root, int minPop)
{
    if (!root) return 0;
    searchCityTree(root->left, minPop);
    if (root->population > minPop)
        printf("   %s (%d)\n", root->name, root->population);
    searchCityTree(root->right, minPop);
    return 0;
}

// Search in list (model B)
int searchCityList(CityPosition head, int minPop)
{
    int count = 0;
    while (head) {
        if (head->population > minPop) {
            printf("   %s (%d)\n", head->name, head->population);
            count++;
        }
        head = head->next;
    }
    if (count == 0)
        printf("   No cities with population greater than %d\n", minPop);
    return 0;
}

// ========================
// MAIN
// ========================

int main()
{
    FILE* f = fopen("drzave.txt", "r");
    if (!f) {
        printf("File not found!\n");
        return 1;
    }

    CountryPosition listHead = NULL;
    CountryPosition treeRoot = NULL;

    char countryName[MAX_NAME], fileName[MAX_NAME];

    // READ COUNTRIES AND CREATE BOTH MODELS
    while (fscanf(f, "%s %s", countryName, fileName) == 2)
    {
        listHead = insertCountryList(listHead, createCountry(countryName, fileName));
        treeRoot = insertCountryTree(treeRoot, createCountry(countryName, fileName));
    }
    fclose(f);

    // LOAD CITIES
    CountryPosition temp = listHead;
    while (temp) {
        loadCities(temp, 1);  // mode 1 = tree for model A
        temp = temp->next;
    }

    loadCitiesTree(treeRoot, 2); // mode 2 = list for model B

    // PRINT BOTH MODELS
    printf("\n--- COUNTRIES (LIST + CITY TREES) ---\n");
    printCountryList(listHead);

    printf("\n--- COUNTRIES (TREE + CITY LISTS) ---\n");
    printCountryTree(treeRoot);

    // USER SEARCH
    char searchName[MAX_NAME];
    int minPop;

    printf("\nEnter country name: ");
    scanf("%s", searchName);
    searchName[strcspn(searchName, "\n")] = 0;

    printf("Enter minimum population: ");
    scanf("%d", &minPop);

    CountryPosition found = findCountry(treeRoot, searchName);
    if (found) {
        printf("\nCities in %s with population greater than %d:\n", found->name, minPop);
        searchCityList(found->cityList, minPop);  // now works correctly
    }
    else {
        printf("Country not found!\n");
    }

    // FREE MEMORY
    freeCountryListWithTrees(listHead);
    freeCountryTreeWithLists(treeRoot);

    return 0;
}
