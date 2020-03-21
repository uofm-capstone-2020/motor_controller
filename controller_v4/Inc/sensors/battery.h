#ifndef __BATTERY_H
#define __BATTERY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "control/mc_params.h"

/**
 * Update the battery voltage read from ADC
 * 
 * @param voltage the adc voltage reading
 */
void update_battery_voltage(uint32_t adc_reading);

#ifdef __cplusplus
}
#endif

#endif /* __BATTERY_H */