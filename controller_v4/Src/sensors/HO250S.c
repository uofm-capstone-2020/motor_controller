/*
 * HO250S.c
 *
 * See datasheet: https://www.lem.com/sites/default/files/products_datasheets/ho_50_250-s-0100_series.pdf
 *
 *  Created on: Mar 5, 2020
 *      Author: jolen
 *
 *  Since the sigma delta filters were too noisy, 3 Hall effect sensors with 3 external ADCs on
 *  the current sensor PCB were used to read the phase currents. The 3 external ADCS start to
 *  convert when they receive the start signal from the STM32. The STM32 then reads each ADC
 *  using SPI. The STM32 choses between ADCs by using a chip select.
 */

#include "sensors/HO250S.h"
#include "stm32f7xx_hal.h"
#include "main.h"

extern SPI_HandleTypeDef hspi6;

void delay_us(int);
uint16_t fast_rx(void);
void spi_rx_byte(SPI_HandleTypeDef *hspi, uint8_t *pRxData);
void recalibrate_adcs(void);
void get_current_offsets(void);

float nums_list[100];
float offset_vals[3];

/*
 * Initilize the ADCs that read the HO250 sensors. The ADCs are recalibarted by sending 1024
 * clock signals. The offsets of the current sensor are then read.
 */
void ho250S_init(void) {
	HAL_GPIO_WritePin(IABC_CNVST_GPIO_Port, IABC_CNVST_Pin, 1);
	HAL_GPIO_WritePin(IABC_CS_A_GPIO_Port, IABC_CS_A_Pin, 1);
	HAL_GPIO_WritePin(IABC_CS_B_GPIO_Port, IABC_CS_B_Pin, 1);
	HAL_GPIO_WritePin(IABC_CS_C_GPIO_Port, IABC_CS_C_Pin, 1);
	HAL_Delay(1);
	recalibrate_adcs();
	HAL_Delay(500);
	get_current_offsets();
}

/*
 * Start the ADC conversion.
 */
void start_iabc_conversions(void) {
	HAL_GPIO_WritePin(IABC_CNVST_GPIO_Port, IABC_CNVST_Pin, 0);
}

/*
 * Check for phase current zeros. If the phase current magnitude is < 0.5, then just make it zero. We had some
 * issues with the control system and small noise when we were asking for zero torque and this fixed it.
 */
void check_for_iabc_zeros(void) {
	if (absVal(mc_vars.iabc[0])<0.5) {
		mc_vars.iabc[0]=0;
	}
	if (absVal(mc_vars.iabc[1])<0.5) {
		mc_vars.iabc[1]=0;
	}
	if (absVal(mc_vars.iabc[2])<0.5) {
		mc_vars.iabc[2]=0;
	}
}

/*
 * Update the phase currents by reading the currents from the three external ADCs over SPI
 */
void update_iabc(void) {
	int16_t temp;

	// Phase A
	HAL_GPIO_WritePin(IABC_CS_A_GPIO_Port, IABC_CS_A_Pin, 0);
	temp = fast_rx();
	mc_vars.iabc[0] = (float) (temp*CURRENT_MULTIPLIER_A);
	mc_vars.iabc[0] -= offset_vals[0];
	delay_us(1); // YOU NEED to wait 1us or your values will sometimes have errors.

	// Phase B
	HAL_GPIO_WritePin(IABC_CS_B_GPIO_Port, IABC_CS_B_Pin, 0);
	temp = fast_rx();
	mc_vars.iabc[1] = (float) (temp*CURRENT_MULTIPLIER_B);
	mc_vars.iabc[1] -= offset_vals[1];
	delay_us(1); // YOU NEED to wait 1us or your values will sometimes have errors.

	// Phase C
	HAL_GPIO_WritePin(IABC_CS_C_GPIO_Port, IABC_CS_C_Pin, 0);
	temp = fast_rx();
	mc_vars.iabc[2] = (float) (temp*CURRENT_MULTIPLIER_C);
	mc_vars.iabc[2] -= offset_vals[2];

	// reset conversion pin
	HAL_GPIO_WritePin(IABC_CNVST_GPIO_Port, IABC_CNVST_Pin, 1);

	// reset chip selects
	HAL_GPIO_WritePin(IABC_CS_A_GPIO_Port, IABC_CS_A_Pin, 1);
	HAL_GPIO_WritePin(IABC_CS_B_GPIO_Port, IABC_CS_B_Pin, 1);
	HAL_GPIO_WritePin(IABC_CS_C_GPIO_Port, IABC_CS_C_Pin, 1);

	// check for faults
	if (absVal(mc_vars.iabc[0]) > IABC_LIMIT) {
		mc_vars.fault_reg1 |= 1 << HIGH_IA_FLAG_BIT;
		mc_vars.fault_current[0] = mc_vars.iabc[0];
		Error_Handler();
	}
	if (absVal(mc_vars.iabc[1]) > IABC_LIMIT) {
		mc_vars.fault_reg1 |= 1 << HIGH_IB_FLAG_BIT;
		mc_vars.fault_current[1] = mc_vars.iabc[1];
		Error_Handler();
	}
	if (absVal(mc_vars.iabc[2]) > IABC_LIMIT) {
		mc_vars.fault_reg1 |= 1 << HIGH_IC_FLAG_BIT;
		mc_vars.fault_current[2] = mc_vars.iabc[2];
		Error_Handler();
	}
}

/*
 * Recalibrate external ADCs by sending 1024 clock cycles.
 */
void recalibrate_adcs(void) {
	uint16_t dummyData = 0x0000;
	HAL_GPIO_WritePin(IABC_CNVST_GPIO_Port, IABC_CNVST_Pin, 0);
	for (int i =0; i<128; i++) {
		HAL_SPI_Transmit(&hspi6, &dummyData, 1, 100);
	}
	for (int i=0; i<5; i++) {
		HAL_Delay(100);
	}
	HAL_GPIO_WritePin(IABC_CNVST_GPIO_Port, IABC_CNVST_Pin, 1);
}

/*
 * Current HO 250S offsets before the controller starts by taking the average of 100 samples
 */
void get_current_offsets(void) {
	float temp[3];
	for (int i=0; i<3; i++){
		temp[i] = 0;
		offset_vals[i]=0;
	}
	for (int i=0; i<100; i++) {
		start_iabc_conversions();
		HAL_Delay(1);
		update_iabc();
		temp[0] += mc_vars.iabc[0];
		temp[1] += mc_vars.iabc[1];
		temp[2] += mc_vars.iabc[2];
		HAL_Delay(1);
	}
	offset_vals[0] = temp[0] / 100;
	offset_vals[1] = temp[1] / 100;
	offset_vals[2] = temp[2] / 100;
}

/*
 * The HAL Rx function is slow and bloated. Here's the refactored HAL code to just the basics that we need.
 * This is also used in the PGA411.
 */
uint16_t fast_rx(void) {
	uint16_t miso = 0; /* in coming data*/
	uint8_t buffer2 = 0;

	/* Send data byte by byte */
	// first byte
	spi_rx_byte(&hspi6,&buffer2); /* most significant byte first */
	miso = buffer2;

	// second byte
	spi_rx_byte(&hspi6,&buffer2);
	miso= ((miso<< 8) | buffer2);

	return(miso);
}

/*
 * Send a byte over SPI. Refactored from the HAL libraries.
 */
void spi_rx_byte(SPI_HandleTypeDef *hspi, uint8_t *pRxData) {
	uint8_t temp = 0x00;
	/* Set the transaction information */
	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pRxBuffPtr  = (uint8_t *)pRxData;
	hspi->RxXferCount = 1;
	hspi->RxXferSize  = 1;
	hspi->pTxBuffPtr  = (uint8_t *)&temp;
	hspi->TxXferCount = 1;
	hspi->TxXferSize  = 1;

	/*Init field not used in handle to zero */
	hspi->RxISR       = NULL;
	hspi->TxISR       = NULL;


	SET_BIT(hspi->Instance->CR2, SPI_RXFIFO_THRESHOLD);
	*(__IO uint8_t *)&hspi->Instance->DR = (*hspi->pTxBuffPtr);
	hspi->pTxBuffPtr++;
	hspi->TxXferCount--;

	while (hspi->RxXferCount > 0U)
	{
	  /* Wait until RXNE flag is reset */
	  if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE))
	  {
		  (*(uint8_t *)hspi->pRxBuffPtr) = *(__IO uint8_t *)&hspi->Instance->DR;
		  hspi->pRxBuffPtr++;
		  hspi->RxXferCount--;
	  }
	}
}

/*
 * Delay in microseconds. This is approximate, but when optimizing for speed, it takes ~ 1us when microsec = 1
 * Hal only has delay in ms, and that's too long.
 */
void delay_us(int microsec){
	int num = 2*microsec;
	for (int i=0; i<num; i++) {
		HAL_GPIO_WritePin(IABC_CS_B_GPIO_Port, IABC_CS_A_Pin, 0);
	}
}
