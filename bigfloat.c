#include "bigfloat.h"

/*
 * Create BigFloat and return the pointer.
 */
BigFloat *create(char *str) {
  int i;
  BigFloat *res;
  res = malloc(sizeof(BigFloat));
  res->decimal = 1;
  for (i = 0; i < PRECISION; i++) {
    res->digits[i] = 0;
  }
  res->negative = 0;
  parse(res, str);
  return res;
}

void freeBigFloat(BigFloat *b) {
  free(b);
}

/*
 * Parses in a string representing a floating point number and creates a
 * BigFloat out of the string representation.
 */
void parse(BigFloat *b, char *str) {
  int i = 0;
  int index = 0;
  if (str[0] == '-') {
    b->negative = 1;
    i = 1;
  } else {
    b->negative = 0;
  }
  for (; i < strlen(str); i++) {
    if (str[i] == '.') {
      b->decimal = (b->negative) ? i - 1 : i;
    } else {
      b->digits[index++] = str[i] - '0'; }
  }
}

/*
 * Prints out the given BigFloat.
 */
void print(BigFloat *b) {
  int i;
  if (b->negative) {
    printf("-");
  }
  for (i = 0; i < PRECISION; i++) {
    if (i == b->decimal) {
      printf(".");
    }
    printf("%d", b->digits[i]);
  }
  printf("\n");
}

/*
 * Adds two BigFloats and puts the result in the first parameter.
 * TODO: work with negative BigFloats
 */
void add(BigFloat *a, BigFloat *b) {
  int i, result;
  int carry = 0;
  standardizeDecimal(a, b);
  for (i = PRECISION - 1; i >= 0; i--) {
    result = carry;
/*    result += (a->negative) ? -1 * a->digits[i] : a->digits[i];
    result += (b->negative) ? -1 * b->digits[i] : b->digits[i]; */
    result += a->digits[i] + b->digits[i];
    carry = result / 10;
    a->digits[i] = result % 10;
  }
  if (carry != 0) {
    shiftDownBy(a->digits, PRECISION, 1);
    a->decimal++;
    a->digits[0] = carry;
  }
}

/*
 * Subtract b from a and return a new BigFloat as the result.
 */
BigFloat *subtract(BigFloat *a, BigFloat *b) {
  int i;
  BigFloat *top, *bottom;
  BigFloat *res = create("0.0");
  standardizeDecimal(a, b);
  res->decimal = a->decimal;
  if (compare(a,b) >= 0) {
    top = a;
    bottom = b;
  } else {
    top = b;
    bottom = a;
    res->negative = 1;
  }
  for (i = PRECISION - 1; i >= 0; i--) {
     if (top->digits[i] < bottom->digits[i]) {
       top->digits[i - 1]--;
       res->digits[i] = top->digits[i] + 10 - bottom->digits[i];
     } else {
       res->digits[i] = top->digits[i] - bottom->digits[i];
     }
  }
  trailingZeros(a);
  trailingZeros(b);
  trailingZeros(res);
  return res;
}
  

BigFloat *multiply(BigFloat *a, BigFloat *b) {
  int i;
  int carry = 0;
  BigFloat *res = create("0.0");
  BigFloat *line = create("0.0");
  res->decimal = PRECISION;
  line->decimal = PRECISION;
  zerosFirst(a);
  zerosFirst(b);
  for (i = PRECISION - 1; i >= 0; i--) {
    multiplyLine(a, line, b->digits[i]);
    shiftUpBy(line->digits, PRECISION, PRECISION - i);
    add(res, line);
  }
  res->decimal -= PRECISION - a->decimal + PRECISION - b->decimal + 1;
  trailingZeros(a);
  trailingZeros(b);
  trailingZeros(res);
  freeBigFloat(line);
  line = NULL;
  res->negative = ((a->negative || b->negative) && !(a->negative && b->negative)) ? 1 : 0;
  return res;
}

void multiplyLine(BigFloat *a, BigFloat *line, int mult) {
  int i, result;
  int carry = 0;
  for (i = PRECISION - 1; i >= 0; i--) {
    result = carry;
    result += a->digits[i] * mult;
    carry = result / 10;
    line->digits[i] = result % 10;
  }
}

BigFloat *divide(BigFloat *a, BigFloat *b) {
  int i, counter;
  int carry = 0;
  int index = 0;
  BigFloat *res = create("0.0");
  BigFloat *current = create("0.0");
  current->decimal = 0;
  res->decimal = a->decimal;
  printf("-------------------------\n");
  for (i = 0; i < PRECISION; i++) {
    counter = 0;
    current->digits[index++] = a->digits[i];
    current->decimal++;
    //print(current);
    while (compare(current, b) >= 0) {
      current = subtract(current, b);
      counter++;
    }
    res->digits[i] = counter;
  }
  printf("-------------------------\n");
  trailingZeros(res);
  return res;  
}

/*
 * Tests whether or not two BigFloats are equal.
 */
char equals(BigFloat *a, BigFloat *b) {
  int i;
  if (a == b) {
    return 1;
  } else {
    if (a->decimal == b->decimal && a->negative == b->negative) {
      for (i = 0; i < PRECISION; i++) {
        if (a->digits[i] != b->digits[i]) {
          return 0;
        }
      }
      return 1; 
    } else {
      return 0;
    }
  }
}

/*
 * Compares two BigFloats so that compare(a, b) > 0 if
 * a > b and so on with = and <
 */
char compare(BigFloat *a, BigFloat *b) {
  int i;
  if (a == b) {
    return 0;
  } else {
    if (a->decimal != b->decimal) {
      return (char) a->decimal - b->decimal;
    } else {
      for (i = 0; i < PRECISION; i++) {
        if (a->digits[i] != b->digits[i]) {
          return (char) a->digits[i] - b->digits[i];
        }
      }
      return 0;
    }
  }
}

/*
 * Shifts the BigFloat down so that there are not any trailing zeros and all
 * zeros are leading the BigFloat.
 */
void zerosFirst(BigFloat *a) {
  int i, start;
  for (i = PRECISION - 1; i >= 0 && !a->digits[i]; i--);
  start = i;
  shiftDownBy(a->digits, PRECISION, PRECISION - start - 1);
  a->decimal += PRECISION - start - 1;
}

/*
 * Shifts the BigFloat down so that there are not any leading zeros and all
 * zeros are trailing the BigFloat.
 */
void trailingZeros(BigFloat *a) {
  int i, start;
  for (i = 0; i < PRECISION && !a->digits[i]; i++);
  start = i;
  shiftUpBy(a->digits, PRECISION, start);
  a->decimal -= start;
}

/*
 * Takes two BigFloats and shifts them so that they have the same decimal point.
 */
void standardizeDecimal(BigFloat *a, BigFloat *b) {
  if (b->decimal > a->decimal) {
    shiftDownBy(a->digits, PRECISION, b->decimal - a->decimal);
    a->decimal = b->decimal;
  } else if (b->decimal < a->decimal) {
    shiftDownBy(b->digits, PRECISION, a->decimal - b->decimal);
    b->decimal = a->decimal;
  }
}

/*
 * Shifts a char array down by the specified shift
 */
void shiftDownBy(char *ar, int length, int shift) {
  int i;
  for (i = length - 1; i >= 0; i--) {
    if (i - shift >= 0) {
      ar[i] = ar[i - shift];
    } else {
      ar[i] = 0;
    }
  }
}

/*
 * Shifts a char array up by the specified shift
 */
void shiftUpBy(char *ar, int length, int shift) {
  int i;
  for (i = 0; i < length; i++) {
    if (i + shift < length) {
      ar[i] = ar[i + shift];
    } else {
      ar[i] = 0;
    }
  }
}

void clear(BigFloat *a) {
  int i;
  for (i = 0; i < PRECISION; i++) {
    a->digits[i] = 0;
  }
}

void printAr(char *ar, int length) {
  int i;
  printf("[%d", ar[0]);
  for (i = 1; i < length; i++) {
    printf(", %d", ar[i]);
  }
  printf("]\n");
}
