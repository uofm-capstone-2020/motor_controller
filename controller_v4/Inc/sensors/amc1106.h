/*
 * amc1106.h
 *
 *  Created on: Oct 31, 2019
 *      Author: jolen
 */

#ifndef AMC1106_H_
#define AMC1106_H_


#define DFSDM_TO_CURRENT_RATIO 0.000294;
#define SHUNT_R 0.0001
#define SHUNT_Y = 10000
#define PHASE_CURRENT_LIMIT 40
#define VDC_LIMIT 325
#define VDC_MULTIPLIER 5.16E-04
#define PHASEA 0
#define PHASEB 1
#define PHASEC 2
#define VDC 3


#include "main.h"
#include "stm32f7xx_hal.h"

void amc1106_init(void);
void start_Vdc_conversion(void);
void update_Vdc(void);

#endif /* AMC1106_H_ */
