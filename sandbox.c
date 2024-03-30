#include <stdio.h>
#include "bignum.h"

int main() {

 Bignum num1 = initBignum();
 Bignum num2 = initBignum(); 
 Bignum res = initBignum(); 
 setBignum(&num1, "123", positive);
 setBignum(&num2, "987", positive);

 multiplyBignum(&res, &num1, &num2);

  printf("\n");
  printBignum(&num1);
  printf(" * ");
  printBignum(&num2);
  printf(" = ");
  printBignum(&res);


  return 0;
}
