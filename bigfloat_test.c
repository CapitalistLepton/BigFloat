#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bigfloat.h"

#define TESTS 4
#define BEFORE 100000
#define AFTER 1000000

void test_add();
void test_subtract();
void test_multiply();
void test_divide();
char *randFloat();

int main() {
  srand(time(NULL));
  printf("-------Addition-------\n");
  test_add();
  printf("-------Multiplication-------\n");
  test_multiply();
  printf("-------Subtraction-------\n");
  test_subtract();
  printf("-------Division-------\n");
  test_divide(); 

/*  BigFloat *one = create("1.0");
  BigFloat *tenFac = create("3628800.0");
  BigFloat *res = divide(one, tenFac);
  print(res);
  printf("decimal: %d\n", res->decimal);
  BigFloat *nineFac = create("362880.0");
  res = divide(one, nineFac);
  print(res);
  printf("decimal: %d\n", res->decimal);

  freeBigFloat(one);
  freeBigFloat(tenFac);
  freeBigFloat(nineFac);
  freeBigFloat(res); */

/*  BigFloat *one = create("1.0");
  BigFloat *res = divide(one, one);
  print(res);
  freeBigFloat(one);
  freeBigFloat(res); */

  return 0;
}

void test_add() {
  int i;
  char *buf1, *buf2;
  double dOne, dTwo;
  BigFloat *one, *two, *res, *temp;

  res = create("0.0"); 
  temp = create("0.0");
  for (i = 0; i < TESTS; i++) {
    buf1 = randFloat();
    dOne = atof(buf1);
    one = create(buf1);
 
    buf2 = randFloat();
    dTwo = atof(buf2);
    two = create(buf2);

    print(one);
    print(two);
    add(one, two, temp);
    print(temp);

    sprintf(buf1, "%f", dOne + dTwo);
    parse(res, buf1);
    print(res);
    printf("%s\n\n", (equals(res, temp)) ? "Pass" : "Fail"); 
    freeBigFloat(one); one = NULL;
    freeBigFloat(two); two = NULL;
  }
  freeBigFloat(temp);
  free(buf1);
  free(buf2);
  freeBigFloat(res);
}

void test_subtract() {
  int i;
  char *buf1, *buf2;
  double dOne, dTwo;
  BigFloat *one, *two, *res, *temp;

  res = create("0.0");
  one = create("0.0");
  two = create("0.0");
  temp = create("0.0");
  for (i = 0; i < TESTS; i++) {
    buf1 = randFloat();
    dOne = atof(buf1);
    parse(one, buf1);

    buf2 = randFloat();
    dTwo = atof(buf2);
    parse(two, buf2);

    print(one);
    print(two);
    subtract(one, two, temp);
    print(temp);

    sprintf(buf1, "%f", dOne - dTwo);
    parse(res, buf1);
    print(res);
    printf("%s\n\n", (equals(res, temp)) ? "Pass" : "Fail");
    clear(one);
    clear(two);
  }
  freeBigFloat(one);
  freeBigFloat(two);
  freeBigFloat(res);
  freeBigFloat(temp);
  free(buf1);
  free(buf2);
}

void test_multiply() {
  int i;
  char *buf1, *buf2;
  double dOne, dTwo;
  BigFloat *one, *two, *res, *temp;

  res = create("0.0");
  one = create("0.0");
  two = create("0.0");
  temp = create("0.0");
  for (i = 0; i < TESTS; i++) {
    buf1 = randFloat();
    dOne = atof(buf1);
    parse(one, buf1);

    buf2 = randFloat();
    dTwo = atof(buf2);
    parse(two, buf2);

    print(one);
    print(two);
    multiply(one, two, temp);
    print(temp);

    sprintf(buf1, "%f", dOne * dTwo);
    parse(res, buf1);
    print(res);
    printf("%s\n\n", (equals(res, temp)) ? "Pass" : "Fail");
    clear(one);
    clear(two);
  }
  freeBigFloat(one);
  freeBigFloat(two);
  freeBigFloat(res);
  freeBigFloat(temp);
  free(buf1);
  free(buf2);
}

void test_divide() {
  int i;
  char *buf1, *buf2;
  double dOne, dTwo;
  BigFloat *one, *two, *res, *temp;

  res = create("0.0");
  one = create("0.0");
  two = create("0.0");
  temp = create("0.0");
  for (i = 0; i < TESTS; i++) {
    buf1 = randFloat();
    dOne = atof(buf1);
    parse(one, buf1);

    buf2 = randFloat();
    dTwo = atof(buf2);
    parse(two, buf2);

    print(one);
    print(two);
    divide(one, two, temp);
    print(one);
    print(two);
    print(temp);

    sprintf(buf1, "%f", dOne / dTwo);
    parse(res, buf1);
    print(res);
    printf("%s\n\n", (equals(res, temp)) ? "Pass" : "Fail");
    clear(one);
    clear(two);
  }
  freeBigFloat(one);
  freeBigFloat(two);
  freeBigFloat(res);
  freeBigFloat(temp);
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
