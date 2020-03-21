/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "control/mc_params.h"
#include <stdint.h>
#include <math.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

float fSaturate(float x, float xLowerLimit, float xUpperLimit);
float absVal(float num);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DFSDM_VDC_Pin GPIO_PIN_4
#define DFSDM_VDC_GPIO_Port GPIOE
#define LED_BLUE_Pin GPIO_PIN_0
#define LED_BLUE_GPIO_Port GPIOF
#define LED_GREEN_Pin GPIO_PIN_1
#define LED_GREEN_GPIO_Port GPIOF
#define LED_RED_Pin GPIO_PIN_2
#define LED_RED_GPIO_Port GPIOF
#define MC_ENBL_OUT_Pin GPIO_PIN_8
#define MC_ENBL_OUT_GPIO_Port GPIOF
#define MC_ENBL_IN_Pin GPIO_PIN_9
#define MC_ENBL_IN_GPIO_Port GPIOF
#define DFSDM_IA_Pin GPIO_PIN_0
#define DFSDM_IA_GPIO_Port GPIOC
#define DFSDM_IB_Pin GPIO_PIN_1
#define DFSDM_IB_GPIO_Port GPIOC
#define DFSDM_CLK_Pin GPIO_PIN_2
#define DFSDM_CLK_GPIO_Port GPIOC
#define DFSDM_IC_Pin GPIO_PIN_3
#define DFSDM_IC_GPIO_Port GPIOC
#define VLL_AB_Pin GPIO_PIN_0
#define VLL_AB_GPIO_Port GPIOA
#define VLL_BC_Pin GPIO_PIN_1
#define VLL_BC_GPIO_Port GPIOA
#define MOTOR_TEMP_Pin GPIO_PIN_3
#define MOTOR_TEMP_GPIO_Port GPIOA
#define BATTERY_Pin GPIO_PIN_6
#define BATTERY_GPIO_Port GPIOA
#define IGBT_TEMP_Pin GPIO_PIN_7
#define IGBT_TEMP_GPIO_Port GPIOA
#define DC_CURRENT_Pin GPIO_PIN_5
#define DC_CURRENT_GPIO_Port GPIOC
#define NGATEA_Pin GPIO_PIN_8
#define NGATEA_GPIO_Port GPIOE
#define GATEA_Pin GPIO_PIN_9
#define GATEA_GPIO_Port GPIOE
#define NGATEB_Pin GPIO_PIN_10
#define NGATEB_GPIO_Port GPIOE
#define GATEB_Pin GPIO_PIN_11
#define GATEB_GPIO_Port GPIOE
#define NGATEC_Pin GPIO_PIN_12
#define NGATEC_GPIO_Port GPIOE
#define GATEC_Pin GPIO_PIN_13
#define GATEC_GPIO_Port GPIOE
#define RDC_FAULTRES_Pin GPIO_PIN_10
#define RDC_FAULTRES_GPIO_Port GPIOB
#define RDC_FAULT_Pin GPIO_PIN_11
#define RDC_FAULT_GPIO_Port GPIOB
#define RDC_CS_Pin GPIO_PIN_12
#define RDC_CS_GPIO_Port GPIOB
#define RDC_CLK_Pin GPIO_PIN_13
#define RDC_CLK_GPIO_Port GPIOB
#define RDC_MISO_Pin GPIO_PIN_14
#define RDC_MISO_GPIO_Port GPIOB
#define RDC_MOSI_Pin GPIO_PIN_15
#define RDC_MOSI_GPIO_Port GPIOB
#define RDC_NRST_Pin GPIO_PIN_8
#define RDC_NRST_GPIO_Port GPIOD
#define GATE_DRIVER_FAULT_Pin GPIO_PIN_11
#define GATE_DRIVER_FAULT_GPIO_Port GPIOD
#define IABC_CNVST_Pin GPIO_PIN_9
#define IABC_CNVST_GPIO_Port GPIOG
#define IABC_MISO_Pin GPIO_PIN_12
#define IABC_MISO_GPIO_Port GPIOG
#define IABC_CLK_Pin GPIO_PIN_13
#define IABC_CLK_GPIO_Port GPIOG
#define IABC_OCD_Pin GPIO_PIN_14
#define IABC_OCD_GPIO_Port GPIOG
#define IABC_CS_C_Pin GPIO_PIN_4
#define IABC_CS_C_GPIO_Port GPIOB
#define IABC_CS_B_Pin GPIO_PIN_6
#define IABC_CS_B_GPIO_Port GPIOB
#define IABC_CS_A_Pin GPIO_PIN_7
#define IABC_CS_A_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define HTIM3_FREQ 16000
#define HIGH 1
#define LOW 0
#define TRUE 1
#define FALSE 0

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
