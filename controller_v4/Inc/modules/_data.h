#ifndef __DATA_H
#define __DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f7xx_hal.h"
#include "control/mc_params.h"
#include "can.h"
#include "adc.h"

/**
 * Initialize the data module
 * 
 * @param hcan pointer to the initialized CAN_HandleTypeDef
 * @param hadc pointer to the initialized ADC_HandleTypeDef
 */
void data_init(CAN_HandleTypeDef *hcan, ADC_HandleTypeDef *hadc);

#ifdef __cplusplus
}
#endif

#endif /* __DATA_H */