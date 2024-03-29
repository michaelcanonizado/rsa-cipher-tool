#include <stdio.h>
#include "bignum.h"

int main() {
  Bignum x = initBignum();
  Bignum res = initBignum();

  setBignum(&x, "567", positive);

  halfBignum(&res, &x);

  printf("\nRes: ");
  printBignum(&res);

  return 0;
}
