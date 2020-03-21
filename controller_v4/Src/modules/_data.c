#include "modules/_data.h"

void data_init(CAN_HandleTypeDef *hcan, ADC_HandleTypeDef *hadc){
    can_init(hcan);
    adc_init(hadc);
}