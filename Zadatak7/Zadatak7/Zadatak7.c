#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

/*
   Structure that represents one directory in the tree.
   Each directory knows its parent, first child and next sibling.
*/
struct _Directory;
typedef struct _Directory* Position;
typedef struct _Directory
{
    char name[MAX_NAME_LENGTH];      // directory name
    Position parent;                 // pointer to parent directory
    Position firstChild;             // pointer to first child directory
    Position nextSibling;            // pointer to next directory on same level
} Directory;

/* Function declarations */
Position createDirectory(const char* directoryName, Position parentDirectory);
int addDirectory(Position currentDirectory);
Position changeDirectory(Position currentDirectory);
Position goToParentDirectory(Position currentDirectory);
int printDirectoryContent(Position currentDirectory);
int freeDirectoryTree(Position rootDirectory);

int main()
{
    int menuChoice = 0;

    /* Create root directory */
    Position rootDirectory = createDirectory("root", NULL);
    if (rootDirectory == NULL)
    {
        printf("Memory allocation error!\n");
        return -1;
    }

    /* Current directory starts as root */
    Position currentDirectory = rootDirectory;

    do
    {
        /* Print menu and current directory */
        printf("\nCurrent directory: %s\n", currentDirectory->name);
        printf("1 - md\n");
        printf("2 - cd dir\n");
        printf("3 - cd..\n");
        printf("4 - dir\n");
        printf("5 - exit\n");
        printf("Choice: ");
        scanf("%d", &menuChoice);
        switch (menuChoice)
        {
        case 1:
            /* Create new directory inside current directory */
            if (addDirectory(currentDirectory) != 0)
                printf("Error creating directory!\n");
            break;

        case 2:
            /* Change current directory to child directory */
            currentDirectory = changeDirectory(currentDirectory);
            break;

        case 3:
            /* Go back to parent directory */
            currentDirectory = goToParentDirectory(currentDirectory);
            break;

        case 4:
            /* Print content of current directory */
            printDirectoryContent(currentDirectory);
            break;

        case 5:
            printf("Exiting program...\n");
            break;

        default:
            printf("Wrong menu choice!\n");
        }

    } while (menuChoice != 5);

    /* Free all allocated memory before program ends */
    freeDirectoryTree(rootDirectory);

    return 0;
}

/*
   Creates new directory and sets its pointers.
*/
Position createDirectory(const char* directoryName, Position parentDirectory)
{
    Position newDirectory = NULL;

    /* Allocate memory for new directory */
    newDirectory = (Position)malloc(sizeof(Directory));
    if (newDirectory == NULL)
        return NULL;


    strcpy(newDirectory->name, directoryName); // copy directory name


    newDirectory->parent = parentDirectory; // set parent directory


    newDirectory->firstChild = NULL; //new directory has no children at start


    newDirectory->nextSibling = NULL; //new directory has no siblings at start

    return newDirectory;
}

/*
   Adds new directory as child of current directory.
*/
int addDirectory(Position currentDirectory)
{
    char directoryName[MAX_NAME_LENGTH] = { 0 };
    Position newDirectory = NULL;
    Position temp = NULL;

    /* Read directory name from user */
    printf("Directory name: ");
    scanf("%s", directoryName);

    /* Create new directory */
    newDirectory = createDirectory(directoryName, currentDirectory);
    if (newDirectory == NULL)
        return -1;

    if (currentDirectory->firstChild == NULL) //If current directory has no children 
    {

        currentDirectory->firstChild = newDirectory;  // Set new directory as first child
    }
    else
    {
        /* Go through siblings to find last child */
        temp = currentDirectory->firstChild;
        while (temp->nextSibling != NULL)
        {
            temp = temp->nextSibling;
        }


        temp->nextSibling = newDirectory; // Add new directory at the end 
    }

    return 0;
}

/*
   Changes current directory to selected child directory.
*/
Position changeDirectory(Position currentDirectory)
{
    char directoryName[MAX_NAME_LENGTH] = { 0 };
    Position temp = currentDirectory->firstChild;

    /* Read directory name */
    printf("Directory name: ");
    scanf("%s", directoryName);

    /* Search through child directories */
    while (temp != NULL)
    {

        if (strcmp(temp->name, directoryName) == 0)   // Check if directory name matches 
        {

            return temp;  // Directory found
        }


        temp = temp->nextSibling; // Move to next sibling 
    }


    printf("Directory not found!\n"); // If directory is not found 
    return currentDirectory;
}

/*
   Moves one level up in directory tree.
*/
Position goToParentDirectory(Position currentDirectory)
{

    if (currentDirectory->parent != NULL)  // Check if parent directory exists 
    {
        return currentDirectory->parent;
    }


    printf("Already in root directory!\n"); // Already in root directory 
    return currentDirectory;
}

/*
   Prints all child directories of current directory.
*/
int printDirectoryContent(Position currentDirectory)
{
    Position temp = currentDirectory->firstChild;
    int directoryCount = 0;

    /* Check if directory is empty */
    if (temp == NULL)
    {
        printf("Directory is empty.\n");
        return 0;
    }

    /* Print all child directories */
    while (temp != NULL)
    {
        printf("%s\n", temp->name);
        directoryCount++;

        /* Move to next sibling */
        temp = temp->nextSibling;
    }

    return directoryCount;
}
/*
   Frees memory for whole directory tree.
*/
int freeDirectoryTree(Position rootDirectory)
{

    if (rootDirectory == NULL)  //If directory does not exist 
        return 0;


    freeDirectoryTree(rootDirectory->firstChild); //Free all child directories 


    freeDirectoryTree(rootDirectory->nextSibling);   // Free all sibling directories 


    free(rootDirectory);    // Free current directory 

    return 0;
}