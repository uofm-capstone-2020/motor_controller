/*
 * battery.c
 *
 * Created on: Mar 5, 2020
 *      Author: jolen
 *
 *      */
#include "sensors/battery.h"
extern void Error_Handler(void);

/*
 * Used to read the battery voltage for brownout protection
 */
void update_battery_voltage(uint32_t adc_reading){
    float result = ((3.3 / 4096) * 51 / 10 * adc_reading);
    mc_vars.bat_voltage = result;
    if (result < 10.0){
    	mc_vars.fault_reg1 |= 1 << LOW_BATTERY_FAULT_BIT;
        Error_Handler();
    }
}
