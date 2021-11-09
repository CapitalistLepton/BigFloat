#ifndef BIG_FLOAT_H
#define BIG_FLOAT_H

#define BIGFLOAT_PRECISION 256
#define BIGFLOAT_DECIMAL_LIMITATION (BIGFLOAT_PRECISION / 3)
#define BIGFLOAT_SUCCESS 0
#define BIGFLOAT_FAILURE 1

// Performs devision by using multiplication
#define BIGFLOAT_USE_QUICK_DIVIDE 0

typedef struct bigfloat
{
	unsigned char digits[BIGFLOAT_PRECISION];
	unsigned short decimal;
	unsigned char negative;
} BigFloat;

enum BigFloatToString
{
	BigFloatStringInt,
	BigFloatStringDecimal,
};

BigFloat *BigFloatCreate(char *);
BigFloat *BigFloatCreateFromInt(int);
void BigFloatFree(BigFloat *);
char BigFloatParseString(BigFloat *, char *);
char *BigFloatToString(BigFloat *, unsigned char);
char BigFloatAdd(BigFloat *, BigFloat *, BigFloat *);
char BigFloatSubtract(BigFloat *, BigFloat *, BigFloat *);
char BigFloatMultiply(BigFloat *, BigFloat *, BigFloat *);
char BigFloatDivide(BigFloat *, BigFloat *, BigFloat *);
char BigFloatModulo(BigFloat *, BigFloat *, BigFloat *);
char BigFloatEquals(BigFloat *, BigFloat *);
char BigFloatEqualsUpTo(BigFloat *, BigFloat *, int);
char BigFloatCompare(BigFloat *, BigFloat *);
char BigFloatCompareDifference(BigFloat *a, BigFloat *b);
void BigFloatClear(BigFloat *);
void BigFloatStandardizeDecimal(BigFloat *, BigFloat *);
void BigFloatMultiplyLine(BigFloat *, BigFloat *, int);
void BigFloatZerosFirst(BigFloat *);
void BigFloatIntConvert(BigFloat *);
void BigFloatTrailingZeros(BigFloat *);
void BigFloatShiftDownBy(char *, int, int);
void BigFloatShiftUpBy(char *, int, int);
void BigFloatDecimalLimiter(BigFloat *, int);
int BigFloatToInt(BigFloat *);

#endif
