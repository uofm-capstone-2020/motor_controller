/*
 * thermistor.c
 *
 * See motor thermistor datasheet: https://www.nxp.com/docs/en/data-sheet/KTY81_SER.pdf
 * See thermistor section in IGBT datasheet: https://www.infineon.com/dgdl/Infineon-Deadtime_calculation_for_IGBT_modules-ApplicationNotes-v01_00-EN.pdf?fileId=db3a30431a5c32f2011a5daefc41005b
 *
 *  Created on: Mar 5, 2020
 *      Author: jolen
 *
 *  Motor thermistor:
 *  	This uses two linear lines to estimate the motor temperature.
 *
 *  IGBT thermistor:
 *  	This uses a look up table to estimate IGBT temperature.
 */

#include "sensors/thermistor.h"
#include "main.h"

HAL_StatusTypeDef status;
extern ADC_HandleTypeDef hadc3;
extern ADC_HandleTypeDef hadc2;

// Function takes in the adc value from the motor thermistor and calculates the motor temperatures from
// an equation that depends on the values of the input adc value (using two different equations to decrease
// the temperature error)
void update_motor_temp(uint32_t motorTemp_adc) {
	uint32_t motor_temp_degC;
	float motor_temp_voltage;
    motor_temp_voltage = motorTemp_adc * 3.3 / 4095; // calculate temperature from ADC value
    if (motor_temp_voltage<2.2) { // lower temperature section -55 deg C to 60 deg C
        motor_temp_degC=(motor_temp_voltage+0.908)/0.00836-273;
    } else { // middle temperature section 60 deg C to 125 deg C
        motor_temp_degC=(motor_temp_voltage-0.618)/0.00444-273;
    }

	mc_vars.motorTemp = (float) motor_temp_degC;
	// check for high/low temp extremes
	if (motor_temp_degC > 150) {
		mc_vars.fault_reg1  |= 1 << HIGH_MOTOR_TEMP_FLAG_BIT;
		Error_Handler();
	}
}

// Function takes in the adc value from the IGBT thermistors, looks it up in a lookup table,
// and interpolates to find calculate the IGBT temperature
void update_IGBT_temp(uint32_t igbt_adc_voltage){
	uint16_t index1 = (igbt_adc_voltage-2000)/40; // convert digital voltage to index of look up table
	uint32_t igbt_adc_voltage_rounded = index1 * 40 + 2000; // get the ADC voltage value rounded to the nearest 100th (decimal number)
	if (index1 < LOOPUP_TABLE_SIZE && index1 > 0) {
		uint8_t temp1 = IGBT_LOOKUP_TABLE[index1]; // get the rounded down voltage temperature
		uint8_t temp2 = IGBT_LOOKUP_TABLE[index1+1]; // get the rounded up voltage temperature
		mc_vars.igbtTemp = (float) (igbt_adc_voltage-igbt_adc_voltage_rounded)*(temp2-temp1)/40+temp1; // interpolate to get temperature
		// check for high/low temp extremes
		if (mc_vars.igbtTemp > 100) {
			mc_vars.fault_reg1 |= 1 << HIGH_IGBT_TEMP_FLAG_BIT;
			Error_Handler();
		}
	} else {
		mc_vars.fault_reg1 |= 1 << IGBT_THERMISTOR_FLAG_BIT;
		Error_Handler();
	}
}

// Get ADC values from thermistors and calculate temperature for Motor and IGBT
void update_temps(void) {
	uint32_t motorTemp_adc;
	uint32_t igbt_adc_voltage;
	HAL_ADC_Start(&hadc3);
	HAL_ADC_Start(&hadc2);
	if (HAL_ADC_PollForConversion(&hadc3, 100) == HAL_OK) {
		motorTemp_adc = HAL_ADC_GetValue(&hadc3);
		update_motor_temp(motorTemp_adc);
	} else {
		mc_vars.fault_reg1 |= 1 << ADC_FLAG_BIT;
		Error_Handler();
	}
	if (HAL_ADC_PollForConversion(&hadc2, 100) == HAL_OK) {
		igbt_adc_voltage = HAL_ADC_GetValue(&hadc2);
		update_IGBT_temp(igbt_adc_voltage);
	} else  {
		mc_vars.fault_reg1 |= 1 << ADC_FLAG_BIT;
		Error_Handler();
	}

}
