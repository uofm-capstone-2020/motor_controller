#include "modules/adc.h"
#include "sensors/battery.h"

void handle_value(uint32_t value, adc_states_t state);
extern void Error_Handler(void);
static ADC_HandleTypeDef *_hadc;

void adc_init(ADC_HandleTypeDef *hadc){
    _hadc = hadc;
}

void update_lvbatt_voltage(){
    HAL_ADC_Start(_hadc);
    for(adc_states_t state = 0; state < TOTAL_STATES; state++){
        if( HAL_ADC_PollForConversion(_hadc, 100) == HAL_OK ){
            handle_value( HAL_ADC_GetValue(_hadc), state );
        }
        else{
        	mc_vars.fault_reg1 |= 1 << ADC_FLAG_BIT;
            Error_Handler();
        }
    }
}

void handle_value(uint32_t value, adc_states_t state){
    switch(state){
        case BATTERY:
            return update_battery_voltage(value);
        default: 
            return;
    }
}
