#ifndef BIG_FLOAT_H
#define BIG_FLOAT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PRECISION 100

typedef struct bigfloat {
  char digits[PRECISION];
  char decimal;
  char negative;
} BigFloat;

BigFloat *create(char *);
void freeBigFloat(BigFloat *);
void parse(BigFloat *, char *);
void print(BigFloat *);
void sprint(char *, BigFloat *);
void add(BigFloat *, BigFloat *, BigFloat *);
void subtract(BigFloat *, BigFloat *, BigFloat *);
void multiply(BigFloat *, BigFloat *, BigFloat *);
void divide(BigFloat *, BigFloat *, BigFloat *);
char equals(BigFloat *, BigFloat *);
char equalsUpTo(BigFloat *, BigFloat *, int);
char compare(BigFloat *, BigFloat *);
void clear(BigFloat *);

void standardizeDecimal(BigFloat *, BigFloat *);
void multiplyLine(BigFloat *, BigFloat *, int);
void zerosFirst(BigFloat *);
void trailingZeros(BigFloat *);
void shiftDownBy(char *, int, int);
void shiftUpBy(char *, int, int);
void printAr(char *, int);

#endif
