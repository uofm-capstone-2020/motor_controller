#include "control/mc_params.h"
#include "control/math_fcns.h"

extern float sinTable[1000];
extern float cosTable[1000];

/*
 * Interpolates a sin value given theta using a sin lookup table of 1000 points.
 */
float sin_interpolation(float theta) {
	float sinVal, x;
	uint16_t xa, xb;

	x = (float) (theta * 999.0 / TWOPI);
	xa = (uint16_t) x;
	xb = xa+1;
	if(xb > 999) {
		return sinTable[999];
	}
	sinVal = (float) sinTable[xa] + ((sinTable[xb]-sinTable[xa])*(x-xa)) ;

	return sinVal;
}

/*
 * Interpolates a cos value given theta using a cos lookup table of 1000 points.
 */
float cos_interpolation(float theta) {
	float cosVal, x;
	uint16_t xa, xb;

	x = (float) (theta * 999.0 / TWOPI);
	xa = (uint16_t) x;
	xb = xa+1;
	if(xb > 999) {
		return cosTable[999];
	}
	cosVal = (float) cosTable[xa] + ((cosTable[xb]-cosTable[xa])*(x-xa)) ;

	return cosVal;
}

/*
 * Get absolute value of float. Math.c has this function but this seems to be faster.
 */
float absVal(float num) {
	if (num < 0.0) {
		return -num;
	} else {
		return num;
	}
}

/*
 * Saturate a float value x between a lower limit and upper limit.
 */
float fSaturate(float x, float xLowerLimit, float xUpperLimit) {
	if (x <= xLowerLimit) {
		return xLowerLimit;
	} else if (x >= xUpperLimit) {
		return xUpperLimit;
	}
	return x;
}

/*
 * Saturate a signed integer value x between a lower limit and upper limit.
 */
int iSaturate(int x, int xLowerLimit, int xUpperLimit) {
	if (x <= xLowerLimit) {
		return xLowerLimit;
	} else if (x >= xUpperLimit) {
		return xUpperLimit;
	}
	return x;
}

/*
 * Find the max of three integers. Used in the PWM calculations.
 */
int max(int x, int y, int z) {
	int max;
	if (x > y) {
		if (x > z) {
			max = x;
		} else {
			max = z;
		}
	} else {
		 if (y > z) {
			 max = y;
		 } else {
			 max = z;
		 }
	}
	return max;
}

/*
 * Find the min of three integers. Used in the PWM calculations.
 */
int min(int x, int y, int z) {
	int min;
	if (x < y) {
		if (x < z) {
			min = x;
		} else {
			min = z;
		}
	} else {
		 if (y < z) {
			 min = y;
		 } else {
			 min = z;
		 }
	}
	return min;
}

