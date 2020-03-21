#include <control/torque.h>
#include "modules/can.h"

CAN_HandleTypeDef *_hcan;

CAN_RxHeaderTypeDef *header;
uint8_t can_data[3];
uint8_t data_id;
CAN_FilterTypeDef sFilterConfig;

void can_init(CAN_HandleTypeDef *hcan){
    _hcan = hcan;
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    sFilterConfig.FilterIdHigh = 0;
    sFilterConfig.FilterIdLow = 0;
    sFilterConfig.FilterMaskIdHigh = 0;
    sFilterConfig.FilterMaskIdLow = 0;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterActivation = ENABLE;
    HAL_CAN_ConfigFilter(_hcan, &sFilterConfig);
    HAL_CAN_Start(_hcan);
    //To enable interrupt, uncomment the following. 
    //HAL_CAN_ActivateNotification(_hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void can_poll(){
	uint32_t fifo = 0;
	uint32_t fill_level = HAL_CAN_GetRxFifoFillLevel(_hcan, fifo);
	uint16_t torque = 0;
	for(int i = 0; i < fill_level; i++){
		HAL_CAN_GetRxMessage(_hcan, fifo, header,can_data);
		data_id = can_data[0];
		torque = can_data[1] << 8 | (can_data[2]);
		mc_vars.can_watchdog = 1;
		mc_vars.TqRequest = (float)(torque*10/1454);
	}
}

// void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *hcan){
//   handle();
//   HAL_CAN_Receive_IT(_hcan, buffer, 7);
// }
