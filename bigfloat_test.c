#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bigfloat.h"

#define TESTS 4
#define BEFORE 100000
#define AFTER 1000000

void test_add();
void test_multiply();
char *randFloat();

int main() {
  srand(time(NULL));
  test_add();
  test_multiply();
  

  return 0;
}

void test_add() {
  int i;
  char *buf1, *buf2;
  double dOne, dTwo;
  BigFloat *one, *two, *res;

  res = create("0.0"); 
  for (i = 0; i < TESTS; i++) {
    buf1 = randFloat();
    dOne = atof(buf1);
    one = create(buf1);
 
    buf2 = randFloat();
    dTwo = atof(buf2);
    two = create(buf2);

    print(one);
    print(two);
    add(one, two);
    print(one);

    sprintf(buf1, "%f", dOne + dTwo);
    parse(res, buf1);
    print(res);
    printf("%s\n\n", (equals(res, one)) ? "Pass" : "Fail"); 
    freeBigFloat(one); one = NULL;
    freeBigFloat(two); two = NULL;
  }
  free(buf1);
  free(buf2);
  freeBigFloat(res);
}

void test_multiply() {
  int i;
  char *buf1, *buf2;
  double dOne, dTwo;
  BigFloat *one, *two, *res;

  res = create("0.0");
  one = create("0.0");
  two = create("0.0");
  for (i = 0; i < TESTS; i++) {
    buf1 = randFloat();
    dOne = atof(buf1);
    parse(one, buf1);

    buf2 = randFloat();
    dTwo = atof(buf2);
    parse(two, buf2);

    print(one);
    print(two);
    one = multiply(one, two);
    print(two);
    print(one);

    sprintf(buf1, "%f", dOne * dTwo);
    parse(res, buf1);
    print(res);
    printf("%s\n\n", (equals(res, one)) ? "Pass" : "Fail");
    clear(one);
    clear(two);
  }
  freeBigFloat(one);
  freeBigFloat(two);
  freeBigFloat(res);
  free(buf1);
  free(buf2);
}

char *randFloat() {
  char *res = malloc(102);
  int bef, aft;
  bef = rand() % BEFORE;
  aft = rand() % AFTER;
  sprintf(res, "%d.%d", bef, aft);
  return res;
}
