#ifndef __CAN_H
#define __CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f7xx_hal.h"
#include "_data.h"

#define CAN_TORQUE_REQUEST_ID   0x90
#define CAN_MODE_SELECT_ID      0x51
#define CAN_READ_REG_ID         0x3D

#define MODE_ENABLE     0x00
#define MODE_DISABLE    0x04

/**
 * Initialize the can module
 * 
 * @param hcan pointer to the initialized CAN_HandleTypeDef
 */
void can_init(CAN_HandleTypeDef *hcan);

/**
 * Poll the can module
 * 
 */
void can_poll();

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H */