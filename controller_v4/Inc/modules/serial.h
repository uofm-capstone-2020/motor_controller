#ifndef __SERIAL_H
#define __SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stdbool.h"
#include "utils/flash.h"

/**
 * Initialize the serial module
 * 
 * @param huart pointer to the initialized UART_HandleTypeDef
 */
void serial_init(UART_HandleTypeDef *huart);

/**
 * Send a log through to the DMA controller to transmit
 * 
 * @param data pointer to the byte array to be sent
 * @param onComplete function pointer to the DMA complete transmit callback.
 * @return -1 if DMA is in use, else 0.
 */
int8_t send_message_DMA(uint8_t* data, int length, void (*onComplete)());

/**
 * Get the HAL_UART_State 
 * 
 * @return the UAL_UART_StateTypeDef of the serial module
 */
HAL_UART_StateTypeDef getUARTState(void);

#ifdef __cplusplus
}
#endif

#endif /* __SERIAL_H */
