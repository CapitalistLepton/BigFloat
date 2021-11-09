#include <stdlib.h>
#include <string.h>
#include "bigfloat.h"

/*
* Create BigFloat from char array, and return the pointer.
*/
BigFloat *BigFloatCreate(char *str)
{
	int i;
	BigFloat *res;
	res = malloc(sizeof(BigFloat));
	if (res == NULL)
		return NULL;
	BigFloatClear(res);
	res->decimal = 1;
	for (i = 0; i < BIGFLOAT_PRECISION; i++)
		res->digits[i] = 0;
	res->negative = 0;
	if (BigFloatParseString(res, str))
	{
		free(res);
		return NULL;
	}
	return res;
}

/*
* Create BigFloat from integer value, and return the pointer.
*/
BigFloat *BigFloatCreateFromInt(int value)
{
	int i;
	BigFloat *res;
	res = malloc(sizeof(BigFloat));
	if (res == NULL)
		return NULL;
	BigFloatClear(res);
	res->decimal = 0;
	for (i = 0; i < BIGFLOAT_PRECISION; i++)
		res->digits[i] = 0;
	res->negative = (value < 0);
	if (res->negative)
		value = (0 - value);
	int value2 = value;
	unsigned char size;
	for (size = 0; value2 != 0; size++)
		value2 /= 10;
	for (i = 0; value != 0; i++)
	{
		res->digits[(size - i - 1)] = value % 10;
		value /= 10;
		res->decimal++;
	}
	res->digits[i++] = 0;
	return res;
}

void BigFloatFree(BigFloat *b)
{
	if (b != NULL)
		free(b);
	b = NULL;
}

/*
* Parses in a string representing a floating point number and creates a
* BigFloat out of the string representation.
*/
char BigFloatParseString(BigFloat *b, char *str)
{
	if (str == NULL)
	{
		BigFloatClear(b);
		b->negative = 0;
		b->decimal = 1;
		return BIGFLOAT_SUCCESS;
	}
	b->decimal = 0;
	int i = 0;
	int index = 0;
	if (str[0] == '.')
	{
		b->digits[index] = 0;
		b->decimal = 1;
		index = 1;
		i = 1;
	}
	else if (str[0] == '-')
	{
		b->negative = 1;
		i = 1;
	}
	else
		b->negative = 0;
	for (; i < strlen(str); i++)
	{
		if (index == (BIGFLOAT_PRECISION + b->negative) && (b->decimal != 0 || index > (BIGFLOAT_PRECISION + b->negative)))
			return BIGFLOAT_FAILURE;
		if (str[i] == '.')
			b->decimal = (b->negative) ? i - 1 : i;
		else
		{
			b->digits[index] = str[i] - '0';
			if (b->digits[index++] > 9)
				return 1;
		}
	}
	if (b->decimal == 0)
		b->decimal = (strlen(str) - b->negative);
	return BIGFLOAT_SUCCESS;
}

/*
* Converts BigFloat into a char array.
*/
char *BigFloatToString(BigFloat *b, unsigned char decimals)
{
	const unsigned short length = (BIGFLOAT_PRECISION + b->negative + 1);
	char *str = malloc(sizeof(char) * (BIGFLOAT_PRECISION + 2));
	if (str == NULL)
		return NULL;
	memcpy((str + b->negative), b->digits, (BIGFLOAT_PRECISION + b->negative));
	if (b->negative)
		str[0] = '-';
	short i = b->negative;
	if (decimals == BigFloatStringDecimal)
	{
		for (i = (BIGFLOAT_PRECISION + b->negative); i > b->decimal; i--)
			if (str[(i - 1)] != 0)
				break;
		if (b->negative && i == 1)
			i++;
	}
	else
		i = (b->decimal + b->negative);
	for (short ii = b->negative; ii < i; ii++)
		str[ii] += '0';
	memset((str + i), 0, (length - i));
	str[i] = '\0';
	unsigned short new_length = (sizeof(char) * (i + (decimals == BigFloatStringDecimal) + b->negative + 1));
	char *str_short = realloc(str, new_length);
	if (str_short != NULL)
	{
		str = str_short;
		str[(i + (decimals == BigFloatStringDecimal) + b->negative)] = '\0';
	}
	if (decimals == BigFloatStringDecimal)
	{
		memmove((str + b->negative + b->decimal + 1), (str + b->negative + b->decimal), (new_length - b->decimal));
		str[(b->negative + b->decimal)] = '.';
		if (str[(b->negative + b->decimal + 1)] == 0)
			str[(b->negative + b->decimal + 1)] = '0';
	}
	return str;
}

/*
* Adds two BigFloats and puts the result in the first parameter.
*/
char BigFloatAdd(BigFloat *a, BigFloat *b, BigFloat *res)
{
	int i, result;
	int carry = 0;
	BigFloatStandardizeDecimal(a, b);
	BigFloatClear(res);
	res->decimal = a->decimal;
	unsigned char additionType = (a->negative + b->negative);
	if (additionType != 1)
	{
		unsigned char carry_end = 2;
		for (i = (BIGFLOAT_PRECISION - 1); i >= 0; i--)
		{
			result = carry;
			result += a->digits[i] + b->digits[i];
			carry = result / 10;
			res->digits[i] = result % 10;
			if (carry_end == 2)
				carry_end = (carry == 1);
		}
		if (carry_end)
			return BIGFLOAT_FAILURE;
		if (carry != 0)
		{
			BigFloatShiftDownBy((char *)res->digits, BIGFLOAT_PRECISION, 1);
			res->decimal++;
			res->digits[0] = carry;
		}
		BigFloatTrailingZeros(a);
		BigFloatTrailingZeros(b);
		BigFloatTrailingZeros(res);
		if (additionType == 2)
			res->negative = 1;
	}
	else if (a->negative)
	{
		b->negative = 1;
		BigFloatSubtract(b, a, res);
		b->negative = 0;
	}
	else if (b->negative)
	{
		a->negative = 1;
		BigFloatSubtract(a, b, res);
		a->negative = 0;
	}
	return BIGFLOAT_SUCCESS;
}

/*
* Subtract b from a and return a new BigFloat as the result.
*/
char BigFloatSubtract(BigFloat *a, BigFloat *b, BigFloat *res)
{
	int i, result;
	int carry = 0;
	BigFloat *top, *bottom;
	BigFloatStandardizeDecimal(a, b);
	BigFloatClear(res);
	res->decimal = a->decimal;
	if (a->negative && !b->negative)
	{
		a->negative = 0;
		i = BigFloatAdd(a, b, res);
		a->negative = 1;
		res->negative = 1;
		return i;
	}
	else if (!a->negative && b->negative)
	{
		b->negative = 0;
		i = BigFloatAdd(a, b, res);
		b->negative = 1;
		return i;
	}
	if (BigFloatCompare(a, b) >= 0)
	{
		top = a;
		bottom = b;
	}
	else
	{
		top = b;
		bottom = a;
		res->negative = 1;
	}
	for (i = (BIGFLOAT_PRECISION - 1); i >= 0; i--)
	{
		result = carry + top->digits[i];
		if (result < bottom->digits[i])
		{
			carry = -1;
			res->digits[i] = result + 10 - bottom->digits[i];
		}
		else
		{
			carry = 0;
			res->digits[i] = result - bottom->digits[i];
		}
	}
	BigFloatTrailingZeros(a);
	BigFloatTrailingZeros(b);
	BigFloatTrailingZeros(res);
	return BIGFLOAT_SUCCESS;
}

char BigFloatMultiply(BigFloat *a, BigFloat *b, BigFloat *res)
{
	int i = 0;
	BigFloat *line = BigFloatCreate(NULL);
	if (line == NULL)
		return BIGFLOAT_FAILURE;
	BigFloat *one = BigFloatCreate("1.0");
	if (one == NULL)
		return BIGFLOAT_FAILURE;
	BigFloat *temp = BigFloatCreate(NULL);
	if (temp == NULL)
		return BIGFLOAT_FAILURE;
	if (BigFloatCompare(a, one) == 0)
	{
		memcpy(res, b, sizeof(BigFloat));
		i = 1;
	}
	else if (BigFloatCompare(b, one) == 0)
	{
		memcpy(res, a, sizeof(BigFloat));
		i = 1;
	}
	BigFloatFree(one);
	if (i == 1)
	{
		BigFloatFree(temp);
		BigFloatFree(line);
		return BIGFLOAT_SUCCESS;
	}
	BigFloatClear(res);
	res->decimal = BIGFLOAT_PRECISION;
	line->decimal = BIGFLOAT_PRECISION;
	BigFloatZerosFirst(a);
	BigFloatZerosFirst(b);
	for (i = (BIGFLOAT_PRECISION - 1); i >= 0; i--)
	{
		BigFloatMultiplyLine(a, line, b->digits[i]);
		if (line->digits[0] > 0)
		{
			BigFloatFree(temp);
			BigFloatFree(line);
			return BIGFLOAT_FAILURE;
		}
		BigFloatShiftUpBy((char *)line->digits, BIGFLOAT_PRECISION, BIGFLOAT_PRECISION - i);
		if (BigFloatAdd(res, line, temp))
		{
			BigFloatFree(temp);
			BigFloatFree(line);
			return BIGFLOAT_FAILURE;
		}
		line->decimal = BIGFLOAT_PRECISION;
		BigFloatZerosFirst(temp);
		if (temp->digits[0] > 0)
		{
			BigFloatFree(temp);
			BigFloatFree(line);
			return BIGFLOAT_FAILURE;
		}
		memcpy(res, temp, sizeof(BigFloat));
		BigFloatTrailingZeros(temp);
		if (temp->decimal > BIGFLOAT_PRECISION)
		{
			BigFloatFree(temp);
			BigFloatFree(line);
			return BIGFLOAT_FAILURE;
		}
	}
	res->decimal -= BIGFLOAT_PRECISION - a->decimal + BIGFLOAT_PRECISION - b->decimal + 1;
	BigFloatTrailingZeros(a);
	BigFloatTrailingZeros(b);
	BigFloatTrailingZeros(res);
	BigFloatFree(temp);
	if (res->decimal > BIGFLOAT_PRECISION)
		return BIGFLOAT_FAILURE;
	BigFloatFree(line);
	res->negative = (!a->negative != !b->negative); // XOR if either were negative
	return BIGFLOAT_SUCCESS;
}

void BigFloatMultiplyLine(BigFloat *a, BigFloat *line, int mult)
{
	int i, result;
	int carry = 0;
	for (i = (BIGFLOAT_PRECISION - 1); i >= 0; i--)
	{
		result = carry;
		result += a->digits[i] * mult;
		carry = result / 10;
		line->digits[i] = result % 10;
	}
}

char BigFloatDivide(BigFloat *a, BigFloat *b, BigFloat *res)
{
	int i;
	unsigned long long counter;
	int index = 0;
	BigFloatClear(res);
	res->decimal = b->decimal;
	if (BigFloatEquals(b, res)) // is equal to zero, cannot divide by zero
		return BIGFLOAT_FAILURE;
	BigFloat *current = BigFloatCreate(NULL);
	if (current == NULL)
		return BIGFLOAT_FAILURE;
	BigFloat *temp = BigFloatCreate(NULL);
	if (temp == NULL)
		return BIGFLOAT_FAILURE;
	BigFloat *one = BigFloatCreateFromInt(1);
	if (one == NULL)
		return BIGFLOAT_FAILURE;
	BigFloat *ten = BigFloatCreateFromInt(10);
	if (ten == NULL)
		return BIGFLOAT_FAILURE;
	if (BigFloatEquals(a, b)) // is equal to itself
	{
		memcpy(res, one, sizeof(BigFloat));
		BigFloatFree(one);
		BigFloatFree(ten);
		BigFloatFree(current);
		BigFloatFree(temp);
		return BIGFLOAT_SUCCESS;
	}
	int negative = b->negative;
	b->negative = 0;
#if BIGFLOAT_USE_QUICK_DIVIDE == 1
	BigFloat *c = BigFloatCreate(NULL);
	if (c == NULL)
		return BIGFLOAT_FAILURE;
	memcpy(c, b, sizeof(BigFloat));
	b->negative = negative;
	BigFloatTrailingZeros(c);
	BigFloatStandardizeDecimal(c, one);
	res->decimal = one->decimal;
#else
	BigFloatStandardizeDecimal(a, b);
	res->decimal = a->decimal;
#endif
	current->decimal = 0;
#if BIGFLOAT_USE_QUICK_DIVIDE == 1
	int decimal = 0;
	for (i = one->decimal; i < BIGFLOAT_PRECISION; i++)
	{
		int zeroed = 1;
		for (int ii = i; ii < BIGFLOAT_PRECISION; ii++)
		{
			if (c->digits[ii] != 0)
			{
				zeroed = 0;
				break;
			}
		}
		if (zeroed == 1)
			break;
		BigFloatMultiply(one, ten, temp);
		memcpy(one, temp, sizeof(BigFloat));
		BigFloatMultiply(c, ten, temp);
		memcpy(c, temp, sizeof(BigFloat));
		decimal++;
	}
#endif
	for (i = 0; i < BIGFLOAT_PRECISION; i++)
	{
		counter = 0;
#if BIGFLOAT_USE_QUICK_DIVIDE == 1
		current->digits[index++] = one->digits[i];
#else
		current->digits[index++] = a->digits[i];
#endif
		current->decimal++;
		BigFloatTrailingZeros(current);
#if BIGFLOAT_USE_QUICK_DIVIDE == 1
		for (; BigFloatCompare(current, c) >= 0;)
#else
		for (; BigFloatCompare(current, b) >= 0;)
#endif
		{
#if BIGFLOAT_USE_QUICK_DIVIDE == 1
			BigFloatSubtract(current, c, temp);
#else
			BigFloatSubtract(current, b, temp);
#endif
			memcpy(current, temp, sizeof(BigFloat));
			counter++;
		}
		if (counter > 9)
		{
			int ii = 0;
			do
			{
				res->digits[(i - ii)] += (counter % 10);
				counter /= 10;
				if ((i - ii) < 0)
					return BIGFLOAT_FAILURE;
				ii++;
			} while (counter > 0);
		}
		else
			res->digits[i] = counter;
	}
	BigFloatFree(one);
	BigFloatFree(ten);
	BigFloatFree(current);
	res->negative = negative;
#if BIGFLOAT_USE_QUICK_DIVIDE == 1
	BigFloatFree(c);
	res->decimal += decimal;
#else
	b->negative = negative;
#endif
#if BIGFLOAT_USE_QUICK_DIVIDE == 1
	BigFloatTrailingZeros(res);
	memcpy(temp, res, sizeof(BigFloat));
	// printf("TEST!\n");
	BigFloatDecimalLimiter(temp, BIGFLOAT_DECIMAL_LIMITATION);
	BigFloatClear(res);
	if (BigFloatMultiply(a, temp, res) == BIGFLOAT_FAILURE)
	{
		BigFloatFree(temp);
		return BIGFLOAT_FAILURE;
	}
	BigFloatFree(temp);
#endif
	return 0;
}

char BigFloatModulo(BigFloat *a, BigFloat *b, BigFloat *res)
{
	BigFloatClear(res);
	res->decimal = b->decimal;
	if (BigFloatEquals(b, res)) // is equal to zero, cannot divide by zero
		return BIGFLOAT_FAILURE;
	BigFloat *quotient = BigFloatCreate(NULL);
	if (quotient == NULL)
		return BIGFLOAT_FAILURE;
	BigFloat *product = BigFloatCreate(NULL);
	if (product == NULL)
		return BIGFLOAT_FAILURE;
	BigFloat *modulus = BigFloatCreateFromInt(1);
	if (modulus == NULL)
		return BIGFLOAT_FAILURE;
	BigFloatDivide(a, b, quotient);
	BigFloatIntConvert(quotient);
	if (BigFloatCompareDifference(a, product) == -1) // less than zero
	{
		if (BigFloatCompareDifference(b, product)) // less than zero
			if (BigFloatAdd(quotient, modulus, product) == BIGFLOAT_FAILURE)
				return BIGFLOAT_FAILURE;
		else
			if (BigFloatSubtract(quotient, modulus, product) == BIGFLOAT_FAILURE)
				return BIGFLOAT_FAILURE;
		memcpy(quotient, product, sizeof(BigFloat));
	}
	BigFloatMultiply(b, quotient, product);
	BigFloatSubtract(a, product, res);
	BigFloatFree(quotient);
	BigFloatFree(product);
	BigFloatFree(modulus);
	return BIGFLOAT_SUCCESS;
}

/*
* Tests whether or not two BigFloats are equal.
*/
char BigFloatEquals(BigFloat *a, BigFloat *b)
{
	int i;
	if (a == b)
		return 1;
	else
	{
		if (a->decimal == b->decimal && a->negative == b->negative)
		{
			for (i = 0; i < BIGFLOAT_PRECISION; i++)
				if (a->digits[i] != b->digits[i])
					return 0;
			return 1;
		}
		else
			return 0;
	}
}

/*
* Tests whether or not two BigFloats are equal up to the given decimal place.
*/
char BigFloatEqualsUpTo(BigFloat *a, BigFloat *b, int decimal)
{
	int i;
	if (a == b)
		return 1;
	else
	{
		if (a->decimal == b->decimal && a->negative == b->negative)
		{
			for (i = 0; i < a->decimal + decimal; i++)
				if (a->digits[i] != b->digits[i])
					return 0;
			return 1;
		}
		else
			return 0;
	}
}

/*
* Compares two BigFloats so that compare(a, b) > 0 if
* a > b and so on with = and <
*/
char BigFloatCompare(BigFloat *a, BigFloat *b)
{
	if (a != b)
	{
		if (a->decimal != b->decimal)
			return ((char)a->decimal - b->decimal);
		else
		{
			for (int i = 0; i < BIGFLOAT_PRECISION; i++)
				if (a->digits[i] != b->digits[i])
					return ((char)a->digits[i] - b->digits[i]);
			return 0;
		}
	}
	return 0;
}

/*
* States which BigFloat has bias by its value compared to the other.
*/
char BigFloatCompareDifference(BigFloat *a, BigFloat *b)
{
	BigFloatStandardizeDecimal(a, b);
	if (a->negative && !b->negative)
		return -1;
	else if (!a->negative && b->negative)
		return 1;
	char comparison = BigFloatCompare(a, b);
	if (comparison < 0)
		return -!a->negative;
	else if (comparison > 0)
		return !b->negative;
	else
		return comparison;
}

/*
* Shifts the BigFloat down so that there are not any trailing zeros and all
* zeros are leading the BigFloat.
*/
void BigFloatZerosFirst(BigFloat *a)
{
	int i, start;
	for (i = (BIGFLOAT_PRECISION - 1); i >= 0 && !a->digits[i]; i--)
		;
	start = i;
	BigFloatShiftDownBy((char *)a->digits, BIGFLOAT_PRECISION, BIGFLOAT_PRECISION - start - 1);
	a->decimal += BIGFLOAT_PRECISION - start - 1;
}

/*
* Removes any digits after decimal position.
*/
void BigFloatIntConvert(BigFloat *a)
{
	int i;
	for (i = (BIGFLOAT_PRECISION - 1); i >= a->decimal; i--)
		a->digits[i] = 0;
}

/*
* Shifts the BigFloat down so that there are not any leading zeros and all
* zeros are trailing the BigFloat.
*/
void BigFloatTrailingZeros(BigFloat *a)
{
	int i, start;
	for (i = 0; i < BIGFLOAT_PRECISION && !a->digits[i]; i++)
		;
	if (a->decimal - i < 1)
		i = a->decimal - 1;
	start = i;
	BigFloatShiftUpBy((char *)a->digits, BIGFLOAT_PRECISION, start);
	a->decimal -= start;
}

/*
* Takes two BigFloats and shifts them so that they have the same decimal point.
*/
void BigFloatStandardizeDecimal(BigFloat *a, BigFloat *b)
{
	if (b->decimal > a->decimal)
	{
		BigFloatShiftDownBy((char *)a->digits, BIGFLOAT_PRECISION, b->decimal - a->decimal);
		a->decimal = b->decimal;
	}
	else if (b->decimal < a->decimal)
	{
		BigFloatShiftDownBy((char *)b->digits, BIGFLOAT_PRECISION, a->decimal - b->decimal);
		b->decimal = a->decimal;
	}
}

/*
* Shifts a char array down by the specified shift
*/
void BigFloatShiftDownBy(char *ar, int length, int shift)
{
	int i;
	for (i = length - 1; i >= 0; i--)
	{
		if (i - shift >= 0)
			ar[i] = ar[i - shift];
		else
			ar[i] = 0;
	}
}

/*
* Shifts a char array up by the specified shift
*/
void BigFloatShiftUpBy(char *ar, int length, int shift)
{
	int i;
	for (i = 0; i < length; i++)
	{
		if (i + shift < length)
			ar[i] = ar[i + shift];
		else
			ar[i] = 0;
	}
}

/*
* Limits the size of a decimal
*/
void BigFloatDecimalLimiter(BigFloat *a, int decimal)
{
	int i, count;
	BigFloatTrailingZeros(a);
	count = 0;
	for (i = a->decimal; i < BIGFLOAT_PRECISION; i++)
	{
		if (count++ > decimal)
			a->digits[i] = 0;
	}
}

void BigFloatClear(BigFloat *a)
{
	int i;
	if (a != NULL)
	{
		a->negative = 0;
		for (i = 0; i < BIGFLOAT_PRECISION; i++)
			a->digits[i] = 0;
	}
}

int BigFloatToInt(BigFloat *a)
{
	int result = 0;
	for (unsigned char i = 0; (i < a->decimal && i < 10); i++)
	{
		result *= 10;
		result += a->digits[i];
	}
	if (a->negative)
		result = (0 - result);
	return result;
}
