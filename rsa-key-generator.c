#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <math.h>

#define RAND_NUM_LOWER_LIMIT 1000
#define RAND_NUM_UPPER_LIMIT 9999

// Global Key limits. 
//Since e(public key) and d(private key) are being used as exponents, they cant be too large for this program as it will cause an overflow.
int PUBLIC_KEY_LOWER_LIMIT = 1000;
int PUBLIC_KEY_UPPER_LIMIT = 5000;
// d(private key) gets a lower limit as it will be an exponent to a quite large base (5-6 digits) which will also cause an overflow.
int PRIVATE_KEY_LOWER_LIMIT = 800;
int PRIVATE_KEY_UPPER_LIMIT = 900;

int isPrime(int randNum);
int generatePrimeNum(int lowerLimit, int upperLimit);
int findGCD(int num1, int num2);
int modInverse(int e, int phiOfN);

int main (void) {
    clock_t start_time = clock();
    srand(time(0));
    
    // Generate P and Q Primes
    unsigned long long int pPrime_Private = generatePrimeNum(RAND_NUM_LOWER_LIMIT, RAND_NUM_UPPER_LIMIT);
    unsigned long long int qPrime_Private = generatePrimeNum(RAND_NUM_LOWER_LIMIT, RAND_NUM_UPPER_LIMIT);

    while(pPrime_Private == qPrime_Private) {
        qPrime_Private = generatePrimeNum(RAND_NUM_LOWER_LIMIT, RAND_NUM_UPPER_LIMIT);
    }

    // Generate N
    unsigned long long int n_Public = pPrime_Private * qPrime_Private;
    // Generate Phi of N
    unsigned long long int phiOfN_Private = (pPrime_Private - 1) * (qPrime_Private - 1);

    // Generate PUBLIC KEY (e theoretically must be prime, and between 2 & phi of n. But use the KEY_LIMITS to scope the key)
    unsigned long long int e_Public = generatePrimeNum(2, phiOfN_Private - 1);
    while((findGCD(e_Public, phiOfN_Private) != 1) && (e_Public > phiOfN_Private)) {
        e_Public = generatePrimeNum(2, phiOfN_Private - 1);
    }

    // Generate PRIVATE KEY
    unsigned long long int d_Private = modInverse(e_Public, phiOfN_Private);

    // if d_Private key was not found with the generated e_Public and n_Public. Generate a new e_Public, then generate d_Private.
    while(d_Private == 0) {
        e_Public = generatePrimeNum(2, phiOfN_Private - 1);
        while((findGCD(e_Public, phiOfN_Private) != 1) && (e_Public > phiOfN_Private)) {
            e_Public = generatePrimeNum(2, phiOfN_Private - 1);
        }
        d_Private = modInverse(e_Public, phiOfN_Private);
    }

    // Print Keys
    printf("\nRandom Prime. P: %llu | Q: %llu\n", pPrime_Private, qPrime_Private);
    printf("N: %llu", n_Public);
    printf("\nPhi of N: %llu", phiOfN_Private);
    printf("\ne (Public Key): %llu", e_Public);
    printf("\nGCD of e and Phi of N: %llu", findGCD(e_Public, phiOfN_Private));
    printf("\nd (Private Key): %llu", d_Private);
    printf("\n(d * e) mod Phi of N: %llu", (d_Private * e_Public) % phiOfN_Private);

    clock_t end_time = clock();
    double CPU_TIME_USED = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("\n\nCPU TIME USED: %lf secs", CPU_TIME_USED);

    printf("\n\n");
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
    for (int d=3; d <= phiOfN; ++d) {
        if ((d * e) % phiOfN == 1) {
            printf("\n\n-> (%d*%d) M %d = %d\n\n",d,e, phiOfN,(d * e) % phiOfN);
            return d;
        }
    }

    return 0;
}