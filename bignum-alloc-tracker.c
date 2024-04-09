#include <stdio.h>
#include "bignum.h"

struct node {
    Bignum *bignumPtr;
    struct node *previous;
    struct node *next;
};
typedef struct node BignumNode;

void printNodes(BignumNode *head) {
    BignumNode *tempNode = head;

    printf("\n");
    while(tempNode != NULL) {
        printBignum(tempNode->bignumPtr);
        printf(" -> ");
        tempNode = tempNode->next;
    }
    printf("\n");
}

void findBignumInNode(BignumNode *head, Bignum *targetNum) {
    BignumNode *tempNode = head;

    while (tempNode != NULL) {
        if (isEqualToBignum(targetNum, tempNode->bignumPtr)) {
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

int main(void) {
    Bignum num0, num1, num2, num3, target;
    initBignum(&num0);
    initBignum(&num1);
    initBignum(&num2);
    initBignum(&num3);
    initBignum(&target);
    setBignum(&num0, "100", positive);
    setBignum(&num1, "123", positive);
    setBignum(&num2, "456", positive);
    setBignum(&num3, "789", positive);
    setBignum(&target, "4567", positive);

    BignumNode *head, n0, n1, n2, n3;

    // Initialize List
    n1.bignumPtr = &num1;
    n2.bignumPtr = &num2;
    n3.bignumPtr = &num3;

    head = &n3;
    n3.next = &n2;
    n2.next = &n1;
    n1.next = NULL;

    printNodes(head);

    // Insert Node between n3 and n2
    n0.bignumPtr = &num0;
    n0.next = &n2;
    n3.next = &n0;
    printNodes(head);

    // Delete inserted Node
    n3.next = n0.next;
    n0.next = NULL;
    printNodes(head);

    // Find a Bignum in Linked List
    findBignumInNode(head, &target);

    return 0;
}