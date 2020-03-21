/*
 * HCPL7510.c
 *
 *  Created on: Jan 8, 2020
 *      Author: jolen
 */

#include "sensors/HCPL7510.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

void hcpl7510_init(void);
void update_phase_voltages(void);
extern void Error_Handler(void);

void hcpl7510_init(void) {
	//update_phase_voltages();
}

void compare_vll_vdq(void) {
	float vca;
	float vdq_mag;
	float vabc_mag;
	float mag_diff;
	float mag_diff_ratio;

	vca = - mc_vars.vll[VAB] - mc_vars.vll[VBC];
	vdq_mag = mc_vars.vdq_ref[0] * mc_vars.vdq_ref[1] + mc_vars.vdq_ref[1] * mc_vars.vdq_ref[1];
	vabc_mag = mc_vars.vll[VAB]*mc_vars.vll[VAB]+mc_vars.vll[VBC]*mc_vars.vll[VBC]+vca*vca;
	vabc_mag *= 2 / 9;

	mag_diff = vdq_mag-vabc_mag;
	if (mag_diff < 0) { // if negative make positive
		mag_diff = - mag_diff;
	}

	if (mag_diff > 5) { // if the magnitude difference is greater than 5V check for a huge error in vdq
		// this check is just in case vdq is small (it'll blow up the mag_diff_ratio)
		mag_diff_ratio = mag_diff/vdq_mag;

		if (mag_diff_ratio > 1.1) {
			mc_vars.fault_reg1|= 1 << VLL_MAG_HIGH;
			Error_Handler();
		} else if (mag_diff_ratio < 0.9) {
			mc_vars.fault_reg1 |= 1 << VLL_MAG_LOW;
			Error_Handler();
		}
	}

}

void update_phase_voltages(void) {
	uint32_t raw_voltage;
	HAL_ADCEx_InjectedStart(&hadc1);
	HAL_ADCEx_InjectedStart(&hadc2);

	if (HAL_ADCEx_InjectedPollForConversion(&hadc1, 100) == HAL_OK) {
		raw_voltage = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);
		mc_vars.vll[VAB] = (float) raw_voltage * 0.265 - 541; // vab
	} else {
		mc_vars.fault_reg1 |= 1 << ADC_FLAG_BIT;
		Error_Handler();
	}

	if (HAL_ADCEx_InjectedPollForConversion(&hadc2, 100) == HAL_OK) {
		raw_voltage = HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_1);
		mc_vars.vll[VBC] = (float) raw_voltage * 0.267 - 549; // vab
	} else {
		mc_vars.fault_reg1 |= 1 << ADC_FLAG_BIT;
		Error_Handler();
	}

	compare_vll_vdq();
}
