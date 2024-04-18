#include <stdio.h>
#include <stdlib.h>
#include "../bignum.h"

typedef struct nodeBignum {
    Bignum *value;
    struct nodeBignum *previous;
    struct nodeBignum *next;
} BignumNode;

struct nodeInt {
    int value;
    struct nodeInt *previous;
    struct nodeInt *next;
};
typedef struct nodeInt IntNode;

void printBignumNodes(BignumNode *head) {
    BignumNode *tempNode = head;

    printf("\n");
    while(tempNode != NULL) {
        printBignum(tempNode->value);
        printf(" -> ");
        tempNode = tempNode->next;
    }
    printf("\n");
}
void printIntNodes(IntNode *head) {
    IntNode *tempNode = head;

    printf("\n");
    while(tempNode != NULL) {
        printf("%d -> ", tempNode->value);
        tempNode = tempNode->next;
    }
    printf("\n");
}

void findBignumInNode(BignumNode *head, Bignum *targetNum) {
    BignumNode *tempNode = head;

    while (tempNode != NULL) {
        if (isEqualToBignum(targetNum, tempNode->value)) {
            printf("\nNode ");
            printBignum(targetNum);
            printf(" Found!\n");
            return;
        }
        tempNode = tempNode->next;
    }

    printf("\nNode ");
    printBignum(targetNum);
    printf(" Not Found!\n");
    return;
}

BignumNode* createNewBignumNode(Bignum *num) {
    BignumNode *node = (BignumNode*)malloc(sizeof(BignumNode));
    node->value = num;
    node->previous = NULL;
    node->next = NULL;
    return node;
}

IntNode* createNewIntNode(int num) {
    IntNode *node = (IntNode*)malloc(sizeof(IntNode));
    node->value = num;
    node->previous = NULL;
    node->next = NULL;
    return node;
}

void freeIntNodes(IntNode *head) {
    IntNode *temp = head;

    while(temp != NULL) {
        temp = temp->next;
        free(head);
        head = temp;
    }
}

void deleteIntNode(IntNode *head, int target) {
    IntNode *prev = head;
    IntNode *temp = prev->next;

    while(temp != NULL) {
        if (temp->value == target) {
            printf("Node Found. Deleting Node!");
            prev->next = temp->next;
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    printf("Node Not Found!");
}

void insertIntAtHead(IntNode *intHead, int value) {
    IntNode *newNode = createNewIntNode(value);

    if (intHead == NULL) {
        intHead = newNode;
        return;
    }

    intHead->previous = newNode;
    newNode->next = intHead;
    intHead = newNode;
}

int main(void) {
    BignumNode *bignumHead = NULL, *bignumNode;
    IntNode *intHead = NULL, *intNode;

    for (int i = 0; i < 5; i++) {
        intNode = createNewIntNode(i);
        intNode->next = intHead;
        // intHead->previous = intNode;
        intHead = intNode;
    }

    printIntNodes(intHead);

    deleteIntNode(intHead, 2);

    printIntNodes(intHead);

    freeIntNodes(intHead);

    return 0;
}