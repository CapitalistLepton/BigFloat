#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tap.h>
#include "bigfloat.h"

#define TESTS 9
#define BEFORE 100000
#define AFTER 1000000

void test_add();
void test_subtract();
void test_multiply();
void test_divide();
char *randFloat();

int main() {
  srand(time(NULL));
  diag("Addition");
  test_add();
  diag("Subtraction");
  test_subtract();
  diag("Multiplication");
  test_multiply();
  diag("Division");
  test_divide(); 

  done_testing();
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

    add(one, two, temp);

    sprintf(buf1, "%f", dOne + dTwo);
    parse(res, buf1);
    sprint(buf1, res);
    sprint(buf2, temp);
    ok(equalsUpTo(res, temp, 6), "\n%s\n%s", buf1, buf2);
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

    subtract(one, two, temp);
    print(temp);

    sprintf(buf1, "%f", dOne - dTwo);
    parse(res, buf1);
    sprint(buf1, res);
    sprint(buf2, temp);
    ok(equalsUpTo(res, temp, 6), "\n%s\n%s", buf1, buf2);
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

    multiply(one, two, temp);

    sprintf(buf1, "%f", dOne * dTwo);
    parse(res, buf1);
    sprint(buf1, res);
    sprint(buf2, temp);
    ok(equalsUpTo(res, temp, 4), "\n%s\n%s", buf1, buf2);
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

    divide(one, two, temp);

    sprintf(buf1, "%f", dOne / dTwo);
    parse(res, buf1);
    sprint(buf1, res);
    sprint(buf2, temp);
    ok(equalsUpTo(res, temp, 4), "\n%s\n%s", buf1, buf2);
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
