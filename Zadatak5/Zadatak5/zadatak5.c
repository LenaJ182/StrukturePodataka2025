#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Structure for a single stack node
typedef struct Node {
    double value;
    struct Node* next;
} Node;

// Function to create a new stack node
Node* createNode(double value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Error: memory allocation failed!\n");
        exit(1);
    }
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

// Pushes a new value onto the stack
void push(Node** top, double value) {
    Node* newNode = createNode(value);
    newNode->next = *top;  // link the new node to the previous top
    *top = newNode;        // update the top pointer
}

// Pops the top value from the stack and returns it
double pop(Node** top) {
    if (*top == NULL) {
        printf("Error: stack underflow!\n");
        exit(1);
    }
    Node* temp = *top;
    double value = temp->value;
    *top = temp->next;  // move top pointer to the next node
    free(temp);
    return value;
}

// Checks if a character is an operator
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Evaluates a postfix expression read from a file
double evaluatePostfix(FILE* fp) {
    Node* stack = NULL;
    char token[50];  // token can be a number or operator

    // Read tokens until the end of the file
    while (fscanf(fp, "%s", token) == 1) {
        // If token starts with a digit or a negative sign followed by a digit → it's a number
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            push(&stack, atof(token));  // convert to double and push to stack
        }
        // If token is a single-character operator
        else if (isOperator(token[0]) && token[1] == '\0') {
            double b = pop(&stack);  // operand 2
            double a = pop(&stack);  // operand 1
            double result;

            // Perform the corresponding arithmetic operation
            switch (token[0]) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/':
                if (b == 0) {
                    printf("Error: division by zero!\n");
                    exit(1);
                }
                result = a / b;
                break;
            default:
                printf("Error: unknown operator!\n");
                exit(1);
            }

            // Push the result back onto the stack
            push(&stack, result);
        }
        // Invalid input (neither number nor operator)
        else {
            printf("Error: invalid token '%s'\n", token);
            exit(1);
        }
    }

    // Final result should be the only value left on the stack
    double finalResult = pop(&stack);

    // If the stack is not empty, expression was malformed
    if (stack != NULL) {
        printf("Error: malformed postfix expression!\n");
        exit(1);
    }

    return finalResult;
}

int main() {
    FILE* fp = fopen("postfix.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open file!\n");
        return 1;
    }

    double result = evaluatePostfix(fp);
    printf("Result of postfix expression: %.2f\n", result);

    fclose(fp);
    return 0;
}
