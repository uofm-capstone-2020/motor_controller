/*
 * HO250S.h
 *
 *  Created on: Mar 3, 2020
 *      Author: jolen
 */

#ifndef HO250S_H_
#define HO250S_H_

#define CURRENT_MULTIPLIER_A -0.04572218087 // measured gain (theoretical = 10/2^16/0.0032)
#define CURRENT_MULTIPLIER_B -0.04661682608
#define CURRENT_MULTIPLIER_C -0.04533213279
#define IABC_LIMIT 100
#define LOW 0

void start_iabc_conversions(void);
void update_iabc(void);
void ho250S_init(void);
void check_for_iabc_zeros(void);

#endif /* HO250S_H_ */
