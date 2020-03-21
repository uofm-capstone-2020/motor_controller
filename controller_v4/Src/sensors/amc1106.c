/*
 * AMC1106.c
 *
 * See datasheet: http://www.ti.com/lit/ds/symlink/amc1106m05.pdf
 *
 * Created on: Mar 5, 2020
 *      Author: jolen
 *
 * The AMC1106 is a sigma delta modulator that outputs 0's and 1's relative to the value at it's
 * input. STM32F767 has a built in sigma delta filter. To calibrate it I followed the recommended
 * filter settings in the datasheet and took measurements of NUMS vs voltage input to figure out
 * it's linear equation.
 *
 * Currently we are only using AMC1106 for the Vdc measurements since they were too noisy for the
 * current measurements.
 */

#include "sensors/amc1106.h"

extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter3;
extern DFSDM_Channel_HandleTypeDef hdfsdm1_channel3;

void start_Vdc_conversion(void);
void amc1106_init(void);
void update_Vdc(void);

/*
 * Initialize the sigma delta channel and filter
 */
void amc1106_init(void) {
	HAL_DFSDM_ChannelMspInit(&hdfsdm1_channel3);
	HAL_DFSDM_FilterMspInit(&hdfsdm1_filter3);
	start_Vdc_conversion();
	update_Vdc();
}

/*
 * Start Vdc conversion. This is seperate from the update_Vdc() because it takes some time
 * to filter the signal. To save processing time, instead of polling other tasks are done while
 * this completes.
 */
void start_Vdc_conversion(void) {
	HAL_DFSDM_FilterRegularStart(&hdfsdm1_filter3);
}

/*
 * Get the filtered sigma delta value for Vdc
 */
void update_Vdc(void) {
	HAL_DFSDM_FilterPollForRegConversion(&hdfsdm1_filter3, 100);
	mc_vars.Vdc = (float) HAL_DFSDM_FilterGetRegularValue(&hdfsdm1_filter3, (uint32_t *) &hdfsdm1_channel3);
	mc_vars.Vdc = mc_vars.Vdc * VDC_MULTIPLIER - 176; // the offset -176 can change over time so this might need
													  // to be updated if you notice a difference in the measurement
	if (mc_vars.Vdc > VDC_LIMIT) {
		mc_vars.fault_reg1 |= 1 << HIGH_VDC_FLAG_BIT;
		Error_Handler();
	}
}
