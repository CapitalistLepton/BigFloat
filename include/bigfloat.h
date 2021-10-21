#ifndef BIG_FLOAT_H
#define BIG_FLOAT_H

#define BIGFLOAT_PRECISION 512

typedef struct bigfloat
{
	unsigned char digits[BIGFLOAT_PRECISION];
	short decimal;
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
void BigFloatParseString(BigFloat *, char *);
char *BigFloatToString(BigFloat *, unsigned char);
void BigFloatAdd(BigFloat *, BigFloat *, BigFloat *);
void BigFloatSubtract(BigFloat *, BigFloat *, BigFloat *);
void BigFloatMultiply(BigFloat *, BigFloat *, BigFloat *);
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
int BigFloatToInt(BigFloat *);

#endif
