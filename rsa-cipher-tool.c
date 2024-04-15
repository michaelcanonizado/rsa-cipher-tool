#include <stdio.h>
#include "src/bignum.h"

int main(void) {

    Bignum one;
    initBignum(&one);
    setBignum(&one, "1", positive);

    Bignum pPrimePrivate, qPrimePrivate;
    initBignum(&pPrimePrivate);
    initBignum(&qPrimePrivate);

    Bignum nPublic;
    initBignum(&nPublic);

    Bignum phiOfNPrivate, pPrimePrivateMinusOne, qPrimePrivateMinusOne;
    initBignum(&phiOfNPrivate);
    initBignum(&pPrimePrivateMinusOne);
    initBignum(&qPrimePrivateMinusOne);

    Bignum ePublic, dPrivate;
    initBignum(&ePublic);
    initBignum(&dPrivate);

    setBignum(&pPrimePrivate, "11", positive);
    setBignum(&qPrimePrivate, "13", positive);

    multiplyBignum(&nPublic, &pPrimePrivate, &qPrimePrivate);

    subtractBignum(&pPrimePrivateMinusOne, &pPrimePrivate, &one);
    subtractBignum(&qPrimePrivateMinusOne, &qPrimePrivate, &one);
    multiplyBignum(&phiOfNPrivate, &pPrimePrivateMinusOne, &qPrimePrivateMinusOne);

    setBignum(&ePublic, "7",positive);
    setBignum(&dPrivate, "103",positive);

    printf("\np: ");
    printBignum(&pPrimePrivate);
    printf("\nq: ");
    printBignum(&qPrimePrivate);
    printf("\nnPublic: ");
    printBignum(&nPublic);
    printf("\nphiOfN: ");
    printBignum(&phiOfNPrivate);
    printf("\ne: ");
    printBignum(&ePublic);
    printf("\nd: ");
    printBignum(&dPrivate);

    freeAllBignums();

    return 0;
}