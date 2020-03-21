/*
 * math_fcns.h
 *
 *  Created on: Mar 3, 2020
 *      Author: jolen
 */

#ifndef MATH_FCNS_H_
#define MATH_FCNS_H_

float sin_interpolation(float);
float cos_interpolation(float);
float fSaturate(float, float, float);
float absVal(float);
int iSaturate(int, int, int) ;
int max(int, int, int);
int min(int, int, int);
float check_zero_Vdc(void);

#endif /* MATH_FCNS_H_ */
