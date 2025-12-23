#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ----- Function declarations -----
typedef struct TreeNode {
    int value;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* createNode(int value);
TreeNode* insertNode(TreeNode* root, int value);
int replaceNodes(TreeNode* root);
int inorderPrint(TreeNode* root);
int inorderToFile(TreeNode* root, FILE* file);

// ----- Main function -----
int main() {
    srand((unsigned int)time(NULL));
    TreeNode* root = NULL;
    int numbers[10];

    // Generate 10 random numbers <10,90> and insert into BST
    for (int i = 0; i < 10; i++) {
        numbers[i] = 10 + rand() % 81;
        root = insertNode(root, numbers[i]);
    }

    // Print generated numbers
    printf("Random numbers:\n");
    for (int i = 0; i < 10; i++) printf("%d ", numbers[i]);
    printf("\n");

    // Inorder print and save to file after insert
    FILE* file = fopen("inorder_after_insert.txt", "w");
    if (!file) {
        printf("Cannot open file!\n");
        return 1;
    }
    printf("Inorder after insert:\n");
    inorderPrint(root);
    printf("\n");
    inorderToFile(root, file);
    fclose(file);

    // Replace each node with sum of its descendants
    replaceNodes(root);

    // Inorder print and save to file after replace
    file = fopen("inorder_after_replace.txt", "w");
    if (!file) {
        printf("Cannot open file!\n");
        return 1;
    }
    printf("Inorder after replace:\n");
    inorderPrint(root);
    printf("\n");
    inorderToFile(root, file);
    fclose(file);

    return 0;
}

// ----- Function definitions -----

// Create a new tree node
TreeNode* createNode(int value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->value = value;
    node->left = node->right = NULL;
    return node;
}

// Insert value into BST
TreeNode* insertNode(TreeNode* root, int value) {
    if (!root) return createNode(value);
    if (value < root->value)
        root->left = insertNode(root->left, value);
    else
        root->right = insertNode(root->right, value);
    return root;
}

// Replace each node with sum of its descendants
int replaceNodes(TreeNode* root) {
    if (!root) return 0;
    int leftSum = replaceNodes(root->left);
    int rightSum = replaceNodes(root->right);
    int oldValue = root->value;
    root->value = leftSum + rightSum;
    return root->value + oldValue;
}

// Inorder print to console
int inorderPrint(TreeNode* root) {
    if (!root) return 0;
    inorderPrint(root->left);
    printf("%d ", root->value);
    inorderPrint(root->right);
    return 1;
}

// Inorder print to file
int inorderToFile(TreeNode* root, FILE* file) {
    if (!root) return 0;
    inorderToFile(root->left, file);
    fprintf(file, "%d ", root->value);
    inorderToFile(root->right, file);
    return 1;
}
