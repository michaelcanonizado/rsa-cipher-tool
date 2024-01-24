#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <math.h>

#define RAND_NUM_LOWER_LIMIT 100
#define RAND_NUM_UPPER_LIMIT 999

int isPrime(int randNum);
int generatePrimeNum(int lowerLimit, int upperLimit);
int findGCD(int num1, int num2);
int modInverse(int e, int phiOfN);
int encryptChar(char character, int e, int n);

int main (void) {
    srand(time(0));
    
    unsigned long long int pPrime = generatePrimeNum(RAND_NUM_LOWER_LIMIT, RAND_NUM_UPPER_LIMIT);
    unsigned long long int qPrime = generatePrimeNum(RAND_NUM_LOWER_LIMIT, RAND_NUM_UPPER_LIMIT);

    while(pPrime == qPrime) {
        qPrime = generatePrimeNum(RAND_NUM_LOWER_LIMIT, RAND_NUM_UPPER_LIMIT);
    }

    unsigned long long int n_Public = pPrime * qPrime;
    unsigned long long int phiOfN = (pPrime - 1) * (qPrime - 1);

    unsigned long long int e = (rand() % (3, phiOfN - 1)) + 3;
    while(findGCD(e, phiOfN) != 1) {
        e = (rand() % (3, phiOfN - 1)) + 3;
    }

    unsigned long long int d_Private = modInverse(e, phiOfN);

    printf("Random Prime. P: %llu | Q: %llu\n", pPrime, qPrime);
    printf("N: %llu", n_Public);
    printf("\nPhi of N: %llu", phiOfN);
    printf("\ne (Public Key): %llu", e);
    printf("\nGCD of e and Phi of N: %llu", findGCD(e, phiOfN));
    printf("\nd (Private Key): %llu", d_Private);
    printf("\n(d * e) mod Phi of N: %llu", (d_Private * e) % phiOfN);

    unsigned int encryptedChar = encryptChar('c', e, n_Public);
    printf("\nEncrypted Char: %d", encryptedChar);


    return 0;
}

int isPrime(int randNum) {
    if (randNum < 2) {
        return 0;
    }

    for (int i = 2; i <= (randNum/2); i++) {
        if (randNum % i == 0) {
            return 0;
        }
    }

    return 1;
}

int generatePrimeNum(int lowerLimit, int upperLimit) {
    int randNum = (rand() % (upperLimit - lowerLimit)) + lowerLimit;

    while(isPrime(randNum) == 0) {
        randNum = (rand() % (upperLimit - lowerLimit)) + lowerLimit;
    }

    return randNum;
}

int findGCD(int num1, int num2) {
    int gcd;

    for(int i=1; i <= num1 && i <= num2; ++i)
    {
        // Checks if i is factor of both integers
        if(num1%i==0 && num2%i==0)
            gcd = i;
    }

    return gcd;
}

int modInverse(int e, int phiOfN) {
    for (int d=1; d <= phiOfN; ++d) {
        if ((d * e) % phiOfN == 1) {
            return d;
        }
    }

    return 0;
}

int encryptChar(char character, int e, int n) {
    int asciiCharacter = 15;

    printf("\nE: %llu", pow(asciiCharacter, e));

    unsigned long long int charToThePowerOfE = pow(asciiCharacter, e);
    printf("\n\ncharToThePowerOfE: %llu", charToThePowerOfE);

    int encryptedCharacter = charToThePowerOfE % n;

    return encryptedCharacter;
}