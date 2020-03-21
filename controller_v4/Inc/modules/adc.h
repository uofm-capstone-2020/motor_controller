#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f7xx_hal.h"
#include "_data.h"

typedef enum {
  BATTERY,
  TOTAL_STATES
} adc_states_t; 

/**
 * Initialize the adc module
 * 
 * @param hadc pointer to the initialized ADC_HandleTypeDef
 */
void adc_init(ADC_HandleTypeDef *hadc);
/**
 * Poll the adc module
 * 
 */
void adc1_poll();
/**
 * Update the LV battery voltage
 *
 */
void update_lvbatt_voltage(void);
#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
