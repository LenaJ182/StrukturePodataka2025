#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int value;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct QueueNode {
    TreeNode* treeNode;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

TreeNode* createNode(int value);
TreeNode* insertNode(TreeNode* root, int value);
TreeNode* searchNode(TreeNode* root, int value);
TreeNode* deleteNode(TreeNode* root, int value);
TreeNode* findMin(TreeNode* root);

int inorderTraversal(TreeNode* root);
int preorderTraversal(TreeNode* root);
int postorderTraversal(TreeNode* root);
int levelOrderTraversal(TreeNode* root);

Queue* createQueue();
int enqueue(Queue* queue, TreeNode* treeNode);
TreeNode* dequeue(Queue* queue);
int isQueueEmpty(Queue* queue);

int main() {
    TreeNode* root = NULL;
    int choice, value;

    do {
        printf("\n--- Binary Search Tree ---\n");
        printf("1 - Insert a number\n");
        printf("2 - Search a number\n");
        printf("3 - Delete a number\n");
        printf("4 - Print inorder\n");
        printf("5 - Print preorder\n");
        printf("6 - Print postorder\n");
        printf("7 - Print level order\n");
        printf("0 - Exit\n");
        printf("Your choice: ");

        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter number to insert: ");
            scanf("%d", &value);
            root = insertNode(root, value);
            printf("Inserted.\n");
        }
        else if (choice == 2) {
            printf("Enter number to search: ");
            scanf("%d", &value);
            printf(searchNode(root, value) ? "Found.\n" : "Not found.\n");
        }
        else if (choice == 3) {
            printf("Enter number to delete: ");
            scanf("%d", &value);
            root = deleteNode(root, value);
            printf("Deleted if it existed.\n");
        }
        else if (choice == 4) {
            printf("Inorder: ");
            inorderTraversal(root);
            printf("\n");
        }
        else if (choice == 5) {
            printf("Preorder: ");
            preorderTraversal(root);
            printf("\n");
        }
        else if (choice == 6) {
            printf("Postorder: ");
            postorderTraversal(root);
            printf("\n");
        }
        else if (choice == 7) {
            printf("Level order: ");
            levelOrderTraversal(root);
            printf("\n");
        }
        else if (choice != 0) {
            printf("Invalid option.\n");
        }

    } while (choice != 0);

    printf("Program finished.\n");
    return 0;
}

TreeNode* createNode(int value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) return NULL;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreeNode* insertNode(TreeNode* root, int value) {
    if (root == NULL)
        return createNode(value);

    if (value < root->value)
        root->left = insertNode(root->left, value);
    else if (value > root->value)
        root->right = insertNode(root->right, value);

    return root;
}

TreeNode* searchNode(TreeNode* root, int value) {
    if (root == NULL || root->value == value)
        return root;

    if (value < root->value)
        return searchNode(root->left, value);

    return searchNode(root->right, value);
}

TreeNode* findMin(TreeNode* root) {
    while (root && root->left)
        root = root->left;
    return root;
}

TreeNode* deleteNode(TreeNode* root, int value) {
    if (root == NULL)
        return NULL;

    if (value < root->value)
        root->left = deleteNode(root->left, value);
    else if (value > root->value)
        root->right = deleteNode(root->right, value);
    else {
        if (root->left == NULL || root->right == NULL) {
            TreeNode* temp = root->left ? root->left : root->right;
            free(root);
            return temp;
        }

        TreeNode* temp = findMin(root->right);
        root->value = temp->value;
        root->right = deleteNode(root->right, temp->value);
    }
    return root;
}

int inorderTraversal(TreeNode* root) {
    if (!root) return 0;
    inorderTraversal(root->left);
    printf("%d ", root->value);
    inorderTraversal(root->right);
    return 0;
}

int preorderTraversal(TreeNode* root) {
    if (!root) return 0;
    printf("%d ", root->value);
    preorderTraversal(root->left);
    preorderTraversal(root->right);
    return 0;
}

int postorderTraversal(TreeNode* root) {
    if (!root) return 0;
    postorderTraversal(root->left);
    postorderTraversal(root->right);
    printf("%d ", root->value);
    return 0;
}

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

int enqueue(Queue* queue, TreeNode* treeNode) {
    QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
    node->treeNode = treeNode;
    node->next = NULL;

    if (!queue->rear) {
        queue->front = queue->rear = node;
        return 1;
    }

    queue->rear->next = node;
    queue->rear = node;
    return 1;
}

TreeNode* dequeue(Queue* queue) {
    if (isQueueEmpty(queue))
        return NULL;

    QueueNode* temp = queue->front;
    TreeNode* node = temp->treeNode;
    queue->front = temp->next;

    if (!queue->front)
        queue->rear = NULL;

    free(temp);
    return node;
}

int isQueueEmpty(Queue* queue) {
    return queue->front == NULL;
}

int levelOrderTraversal(TreeNode* root) {
    if (!root) return 0;

    Queue* queue = createQueue();
    enqueue(queue, root);

    while (!isQueueEmpty(queue)) {
        TreeNode* current = dequeue(queue);
        printf("%d ", current->value);

        if (current->left)
            enqueue(queue, current->left);
        if (current->right)
            enqueue(queue, current->right);
    }
    return 0;
}
