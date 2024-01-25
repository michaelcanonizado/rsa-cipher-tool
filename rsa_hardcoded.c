#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <math.h>

#define RAND_NUM_LOWER_LIMIT 100
#define RAND_NUM_UPPER_LIMIT 999

// Global Key limits. 
//Since e(public key) and d(private key) are being used as exponents, they cant be too large for this program as it will cause an overflow.
int PUBLIC_KEY_LOWER_LIMIT = 1000;
int PUBLIC_KEY_UPPER_LIMIT = 8500;
// d(private key) gets a lower limit as it will be an exponent to a quite large base (5-6 digits) which will also cause an overflow.
int PRIVATE_KEY_LOWER_LIMIT = 100;
int PRIVATE_KEY_UPPER_LIMIT = 999;

int isPrime(int randNum);
int generatePrimeNum(int lowerLimit, int upperLimit);
int findGCD(int num1, int num2);
int modInverse(int e, int phiOfN);
unsigned int encryptChar(char character, int exponent, int divisor);
unsigned int decryptChar(unsigned int character, int exponent, int divisor);
int arrayModulo(int arr[], int size, int divisor);
int multiplyArrayItems(int x, int result[], int size);

int main (void) {
    srand(time(0));
    
    // Generate P and Q Primes
    // unsigned long long int pPrime = generatePrimeNum(RAND_NUM_LOWER_LIMIT, RAND_NUM_UPPER_LIMIT);
    // unsigned long long int qPrime = generatePrimeNum(RAND_NUM_LOWER_LIMIT, RAND_NUM_UPPER_LIMIT);
    unsigned long long int pPrime_Private = 317;
    unsigned long long int qPrime_Private = 653;

    while(pPrime_Private == qPrime_Private) {
        qPrime_Private = generatePrimeNum(RAND_NUM_LOWER_LIMIT, RAND_NUM_UPPER_LIMIT);
    }

    // Generate N
    unsigned long long int n_Public = pPrime_Private * qPrime_Private;
    // Generate Phi of N
    unsigned long long int phiOfN_Private = (pPrime_Private - 1) * (qPrime_Private - 1);

    // Generate PUBLIC KEY (e theoretically must be prime, and between 2 & phi of n. But use the KEY_LIMITS to scope the key)
    unsigned long long int e_Public = generatePrimeNum(PUBLIC_KEY_LOWER_LIMIT, PUBLIC_KEY_UPPER_LIMIT);
    while((findGCD(e_Public, phiOfN_Private) != 1) && (e_Public > phiOfN_Private)) {
        e_Public = generatePrimeNum(PUBLIC_KEY_LOWER_LIMIT, PUBLIC_KEY_UPPER_LIMIT);
    }

    // Generate PRIVATE KEY
    unsigned long long int d_Private = modInverse(e_Public, phiOfN_Private);

    // if d_Private key was not found with the generated e_Public and n_Public. Generate a new e_Public, then generate d_Private.
    while(d_Private == 0) {
        e_Public = generatePrimeNum(PUBLIC_KEY_LOWER_LIMIT, PUBLIC_KEY_UPPER_LIMIT);
        while((findGCD(e_Public, phiOfN_Private) != 1) && (e_Public > phiOfN_Private)) {
            e_Public = generatePrimeNum(PUBLIC_KEY_LOWER_LIMIT, PUBLIC_KEY_UPPER_LIMIT);
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

    // Test keys if it can encrypt
    unsigned int encryptedChar = encryptChar('c', e_Public, n_Public);
    printf("\n\nEncrypted Char: %d", encryptedChar);

    // Test keys if it can decrypt
    unsigned int decryptedChar = decryptChar(encryptedChar, d_Private, n_Public);
    printf("\nDecrypted Char: %d", decryptedChar);

    char d_message[] = "Hello World";
    unsigned int e_message[sizeof d_message/sizeof d_message[0]];

    // Encrypt message
    for (int i = 0; i < sizeof d_message/sizeof d_message[0]; i++) {
        e_message[i] = encryptChar(d_message[i], e_Public, n_Public);
    }

    // Print message
    printf("\n\nMessage: ");
    printf("%s - ", d_message);
    for (int i = 0; i < sizeof d_message/sizeof d_message[0]-1; i++) {
        printf("%d,", d_message[i]);
    }
    // Print encrypted message
    printf("\n\nEncrypted Message: ");
    for (int i = 0; i < sizeof e_message/sizeof e_message[0]-1; i++) {
        printf("%d,", e_message[i]);
    }

    // Decrypt message
    for (int i = 0; i < sizeof e_message/sizeof e_message[0]-1; i++) {
        d_message[i] = decryptChar(e_message[i], d_Private, n_Public);
    }
    // Print decrypted message
    printf("\n\nDecrypted Message: ");
    for (int i = 0; i < sizeof d_message/sizeof d_message[0]-1; i++) {
        printf("%d,", d_message[i]);
    }
    printf("\n");
    for (int i = 0; i < sizeof d_message/sizeof d_message[0]-1; i++) {
        printf("%c,", d_message[i]);
    }

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
    for (int d=1; d <= PRIVATE_KEY_UPPER_LIMIT; ++d) {
        if ((d * e) % phiOfN == 1) {
            return d;
        }
    }

    return 0;
}

unsigned int encryptChar(char character, int exponent, int divisor) {
    character = (int)character;
    
    int size = 0;
    int result[10000];
    int temp = character;

    // Load each digit of base(x) into array backwards to get size
    while(temp != 0) {
        result[size++] = temp % 10;
        temp = temp / 10;
    }

    // Perform multiplication
    for (int i = 2; i <= exponent; i++) {
        size = multiplyArrayItems(character, result, size);
    }
    
    // GET MODULO
    int moduloResult = arrayModulo(result, size, divisor);

    return moduloResult;
}

unsigned int decryptChar(unsigned int character, int exponent, int divisor) {
    
    int size = 0;
    int result[10000];
    int temp = character;

    // Load each digit of base(x) into array backwards to get size
    while(temp != 0) {
        result[size++] = temp % 10;
        temp = temp / 10;
    }

    // Perform multiplication
    for (int i = 2; i <= exponent; i++) {
        size = multiplyArrayItems(character, result, size);
    }
    
    // GET MODULO
    int moduloResult = arrayModulo(result, size, divisor);

    return moduloResult;
}

// Function to calculate modulo of an array of digits (FOR LOOP WILL START FROM THE END OF THE ARRAY AS THE ARRAY IS IN REVERSE)
int arrayModulo(int arr[], int size, int divisor) {
    int remainder = 0;

    // Iterate through the array from left to right
    for (int i = size-1; i >= 0; i--) {
        
        // Calculate the current result including the next digit
        int currentResult = remainder * 10 + arr[i];
        // Update the remainder for the next iteration
        remainder = currentResult % divisor;
    }

    // The final remainder is the result of the modulo operation
    return remainder;
}

int multiplyArrayItems(int x, int result[], int size) {
    int carry = 0;
    int product;
    // printf("\nx: %d size: %d", x, size);
    // Multiply n with each digit of result[]
    for (int i = 0; i < size; i++) {
        product = result[i] * x + carry;
        result[i] = product % 10;
        carry = product / 10;
    }

    // Count how many digits in array
    while(carry) {
        result[size] = carry % 10;
        carry = carry / 10;
        size++;
    }

    return size;
}