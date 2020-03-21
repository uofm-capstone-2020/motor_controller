/*
 * control.h
 *
 *  Created on: Jan 8, 2020
 *      Author: jolen
 */

#ifndef CONTROL_CONTROL_H_
#define CONTROL_CONTROL_H_

#define HIGH 1
#define LOW 0
#define PREV_STATE 0
#define PRESENT_STATE 1

#include "stm32f7xx_hal.h"
#include "mc_params.h"


TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;

void control_init(void);
void control_enable(void);
void check_enable_switch(void);
void control_disable(void);

#endif /* CONTROL_CONTROL_H_ */
