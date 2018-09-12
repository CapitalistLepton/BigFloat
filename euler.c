#include <stdio.h>
#include <stdlib.h>
#include "bigfloat.h"

#define PERM 29 

int main() {
  int i;
  BigFloat *one = create("1.0");
  BigFloat *fac = create("1.0");
  BigFloat *next = create("2.0");
  BigFloat *e = create("1.0");

  for (i = 0; i < PERM; i++) {
/*    divide(one, fac); // 1/n!
    add(e, one); // Summation */
    fac = multiply(fac, next); // Calculate (n+1)!
/*    printf("e: ");
    print(e); */
    printf("%d!: ", i + 2);
    print(fac);
    clear(one);
    parse(one, "1.0");
    add(next, one); // Calculate n + 1
  }
  print(fac);

  freeBigFloat(one);
  freeBigFloat(fac);
  freeBigFloat(next);
  freeBigFloat(e);

  return 0;
}
