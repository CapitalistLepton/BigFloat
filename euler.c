#include <stdio.h>
#include <stdlib.h>
#include "bigfloat.h"

#define PERM 69 

int main() {
  int i = 0;
  BigFloat *one = create("1.0");
  BigFloat *fac = create("1.0");
  BigFloat *next = create("2.0");
  BigFloat *e = create("1.0");
  BigFloat *facFrac = create("0.0");
  BigFloat *temp = create("0.0");

  BigFloat *trueE = create("2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427427");

  for(; i < PERM; i++) {
    divide(one, fac, facFrac); // 1/n!
    add(e, facFrac, temp); // Summation
    memcpy(e, temp, sizeof(BigFloat));
    multiply(fac, next, temp); // Calculate (n+1)!
    memcpy(fac, temp, sizeof(BigFloat));
    printf("1/%d!: ", i + 1);
    print(facFrac);
    printf("e:     ");
    print(e);
    printf("%d!:   ", i + 2);
    print(fac);
    add(next, one, temp); // Calculate n + 1
    memcpy(next, temp, sizeof(BigFloat));
    printf("\n");
  }
  printf("Did it work?\ne:                 ");
  print(e);
  printf("Actual value of e: ");
  print(trueE);

  freeBigFloat(facFrac);
  freeBigFloat(temp);
  freeBigFloat(one);
  freeBigFloat(fac);
  freeBigFloat(next);
  freeBigFloat(e);
  freeBigFloat(trueE);

  return 0;
}
