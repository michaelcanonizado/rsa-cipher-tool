#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bignum.h"

int main() {
    clock_t start = clock();

    Bignum num1;
    Bignum num2;
    Bignum num3;
    Bignum res1;
    Bignum res2;

    printf("\n......................INITIALIZEING BIGNUMS.................\n");

    initBignum(&num1);
    initBignum(&num2);
    initBignum(&num3);
    initBignum(&res1);
    initBignum(&res2);

    printf("\n\n\n......................ADDRESS OF BIGNUMS....................\n");

    printf("\nnum1 %p.%p", &num1, num1.digits);
    printf("\nnum2 %p.%p", &num2, num2.digits);
    printf("\nnum2 %p.%p", &num3, num3.digits);
    printf("\nres1 %p.%p", &res1, res1.digits);
    printf("\nres1 %p.%p", &res2, res2.digits);
    

    printf("\n\n\n......................SETTING OF BIGNUMS.....................\n");

    setBignum(&num1, "35090623376973383154315846016055247071314417045350012035885640546524692884442520598598106279917805826498452947749210540326082508465412527280919094115253780538881702652015456019905397501258687028001832360425402794201144208801497898483350179318273608219034295264727330032093286823490657626781873907720266247365104836133435279070605305654930334314716687873760132714340344830929264950255209448936634768294288839604204291264316634676703675788317695132082777381159996374163172855641913686751024152655424146893240985638954898246326473629134626885987428797387751925596013678757235767146375045131581514013653367641313585880462135692192701182930582608945235203488683985982357981540864715827896181499846076973422144188859295955359385806510160269045507438446788441526770545597441", positive);
    setBignum(&num2, "2", positive);
    setBignum(&num3, "35090623376973383154315846016055247071314417045350012035885640546524692884442520598598106279917805826498452947749210540326082508465412527280919094115253780538881702652015456019905397501258687028001832360425402794201144208801497898483350179318273608219034295264727330032093286823490657626781873907720266247365104836133435279070605305654930334314716687873760132714340344830929264950255209448936634768294288839604204291264316634676703675788317695132082777381159996374163172855641913686751024152655424146893240985638954898246326473629134626885987428797387751925596013678757235767146375045131581514013653367641313585880462135692192701182930582608945235203488683985982357981540864715827896181499846076973422144188859295955359385806510160269045507438446788441526770545597441", positive);

    printf("\n\n\n....................PERFORMING OPERATIONS....................\n");

    multiplyBignum(&res1, &num1, &num3);

    printf("\nMultiply = ");
    printBignumExtended(&res1, res1.length + 100);

    // divideBignum(&res1, &num1, &num2);

    // printf("\nDivide = ");
    // printBignumExtended(&res1, 50);

    // halfBignum(&res2, &num1);

    // printf("\nHalf = ");
    // printBignumExtended(&res2, 50);

    resetBignum(&num1);
    resetBignum(&num2);
    resetBignum(&num3);
    resetBignum(&res1);
    resetBignum(&res2);

    printf("\nnum1 = ");
    printBignumExtended(&num1, 50);
    printf("\nnum2 = ");
    printBignumExtended(&num2, 50);
    printf("\nnum3 = ");
    printBignumExtended(&num3, 50);
    printf("\nres1 = ");
    printBignumExtended(&res1, 50);
    printf("\nres2 = ");
    printBignumExtended(&res2, 50);

    printf("\n\n\n......................FREEING BIGNUMS.........................\n");

    printBignumNodeList();

    freeAllBignums();


    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
