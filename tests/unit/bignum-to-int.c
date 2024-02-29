#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../../bignum.h"

int main(void) {
    int errorCounter = 0;

    printf("\n\n-------------------------------------------------------------------------------------------------------------");
    printf("\n| %27s            || %27s              || %13s       |", "GENERATED BIGNUM", "BIGNUM TO INT", "STATUS");
    printf("\n-------------------------------------------------------------------------------------------------------------");
    printf("\n| %5s  | %6s | %17s    || %18s     | %6s | %5s  || %7s  | %6s   |", "SIGN", "LENGTH", "BIGNUM DIGITS", "LONG LONG INT", "LENGTH", "SIGN", "LENGTH", "SIGN");
    printf("\n-------------------------------------------------------------------------------------------------------------");

    for (int i = -100; i <= 100; i++) {
        Bignum num = initBignum();
        
        if (i < 0) {
            intToBignum(&num, i * -1, negative);
        } else {
            intToBignum(&num, i, positive);
        }
        
        long long int result = bignumToInt(&num);

        char signStatus[10] = "MATCH";
        char lengthStatus[10] = "MATCH";

        if (
            (num.sign == positive && result < 0) || 
            (num.sign == negative && result >= 0)
            ) {
            strcpy(signStatus, "MISMATCH");
            errorCounter++;
        }

        if (num.length != getLengthOfInteger(result)) {
            strcpy(lengthStatus, "MISATCH");
            errorCounter++;
        }
        

        printf("\n| %5s  | %6d | ", num.sign == positive ? "POS" : "NEG", num.length);
        for (int i = num.length; i < 20; i++) {
            printf(" ");
        }
        for (int i = num.length - 1; i >= 0 ; i--) {
            printf("%d", num.digits[i]);
        }

        printf(" ||   %20lld | %6d | %5s  || %8s | %8s |", result, getLengthOfInteger(result), result < 0 ? "NEG" : "POS", lengthStatus, signStatus);
    }

    printf("\n-------------------------------------------------------------------------------------------------------------");
    printf("\n| %48d %-56s |", errorCounter, "ERRORS FOUND");
    printf("\n-------------------------------------------------------------------------------------------------------------");

    printf("\n\n\n");

    return 0;
}