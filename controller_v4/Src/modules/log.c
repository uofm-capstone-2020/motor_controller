#include "modules/log.h"
#include "modules/serial.h"
#include <stdbool.h>
#include <string.h>

high_freq_log_t data_log[LOG_SIZE];
high_freq_log_t temp_buf;
uint8_t live_data_log[LIVE_LOG_DMA_LENGTH_HIGH_FREQ];
uint8_t live_data[LIVE_LOG_DMA_LENGTH_LOW_FREQ];

uint16_t w_ptr;
uint16_t ll_w_ptr;
uint16_t r_ptr;
uint16_t count;
uint16_t triggered_count;
bool triggered;
bool cease_log;
bool live_log_enable;
log_frequency_t frequency = HIGH_FREQ;

void live_log_push_high_freq(high_freq_log_t* data);
void live_log_push_low_freq(high_freq_log_t* data);
void do_nothing(void);

void log_init(){
    w_ptr = 0;
    r_ptr = 0;
    count = 0;
    cease_log = false;
    triggered_count = 0;
    triggered = false;
    live_log_enable = true;
}

void log_push(MotorController_Variables* data){
    if(cease_log){
        return;
    }
    high_freq_log_t *log_ptr = triggered_count >= LOG_SIZE_HALF ? &temp_buf : &data_log[w_ptr];
    log_ptr->id_ref = (float)data->idq_ref[0];
    log_ptr->iq_ref = (float)data->idq_ref[1];
    log_ptr->id = (float)data->idq[0];
    log_ptr->iq = (float)data->idq[1];
    log_ptr->Vdc = (float)data->Vdc;
    log_ptr->vd_ref = (float)data->vdq_ref[0];
    log_ptr->vq_ref = (float)data->vdq_ref[1];
    log_ptr->TqRef = (float)data->TqRef;
    log_ptr->TqLim = (float)data->TqLim;
    log_ptr->TqRequest = (float)data->TqRequest;
    log_ptr->rpm = (float)data->rpm;
    log_ptr->motorTemp = (float)data->motorTemp;
    log_ptr->igbtTemp = (float)data->igbtTemp;
    log_ptr->power = (float)data->power_out;
    log_ptr->faults=(uint32_t)data->fault_reg1;
    log_ptr->ia = (float)data->iabc[0];
    log_ptr->ib = (float)data->iabc[1];
    log_ptr->ic = (float)data->iabc[2];
    log_ptr->theta = (float)data->theta_e;
    if(triggered){
        triggered_count++;
    }
    else if(data->TqRequest > mc_params.Tq_trigger || data->rpm > mc_params.rpm_trigger){
        triggered = true;
    }
    if(!(triggered_count >= LOG_SIZE_HALF)){
        count = (count == LOG_SIZE) ? LOG_SIZE : count+1;
        r_ptr = (count == LOG_SIZE && w_ptr == r_ptr) ? ((r_ptr + 1) % LOG_SIZE) : r_ptr;
        w_ptr = (w_ptr + 1) % LOG_SIZE;
    }
    if(live_log_enable && getUARTState() == HAL_UART_STATE_READY){
        switch(frequency){
            case LOW_FREQ:
                live_log_push_low_freq(log_ptr);
                break;
            case HIGH_FREQ:
                live_log_push_high_freq(log_ptr);
                break;
            default:
                live_log_push_low_freq(log_ptr);
                break;
        }
    }
}

void live_log_push_high_freq(high_freq_log_t* data){
    live_data_log[ll_w_ptr++] = 0x11;
    memcpy(&live_data_log[ll_w_ptr], data, sizeof(high_freq_log_t));
    ll_w_ptr = ll_w_ptr + sizeof(high_freq_log_t);
    live_data_log[ll_w_ptr++] = 0x21;
    live_data_log[ll_w_ptr++] = 0x21;
    live_data_log[ll_w_ptr++] = 0x21;
    live_data_log[ll_w_ptr++] = 0x21;
    if(ll_w_ptr == LIVE_LOG_DMA_LENGTH_HIGH_FREQ - 10){
        live_data_log[ll_w_ptr++] = 0x4C;
        live_data_log[ll_w_ptr++] = 0x4C;
        live_data_log[ll_w_ptr++] = 0x5F;
        live_data_log[ll_w_ptr++] = 0x45;
        live_data_log[ll_w_ptr++] = 0x4E;
        live_data_log[ll_w_ptr++] = 0x44;
        live_data_log[ll_w_ptr++] = 0x21;
        live_data_log[ll_w_ptr++] = 0x21;
        live_data_log[ll_w_ptr++] = 0x21;
        live_data_log[ll_w_ptr++] = 0x21;
        ll_w_ptr = 0;
        send_message_DMA(live_data_log, LIVE_LOG_DMA_LENGTH_HIGH_FREQ, do_nothing);
    }
}

void live_log_push_low_freq(high_freq_log_t* data){
    int ptr = 0;
    live_data[ptr++] = 0x11;
    memcpy(&live_data[ptr], data, sizeof(low_freq_log_t));
    ptr = ptr + sizeof(low_freq_log_t);
    live_data[ptr++] = 0x21;
    live_data[ptr++] = 0x21;
    live_data[ptr++] = 0x21;
    live_data[ptr++] = 0x21;
    live_data[ptr++] = 0x4C;
    live_data[ptr++] = 0x4C;
    live_data[ptr++] = 0x5F;
    live_data[ptr++] = 0x45;
    live_data[ptr++] = 0x4E;
    live_data[ptr++] = 0x44;
    live_data[ptr++] = 0x21;
    live_data[ptr++] = 0x21;
    live_data[ptr++] = 0x21;
    live_data[ptr++] = 0x21;
    send_message_DMA(live_data, LIVE_LOG_DMA_LENGTH_LOW_FREQ, do_nothing);
}

void set_frequency(log_frequency_t f){
    frequency = f;
}

void do_nothing(){
    return;
}

void enable_live_log(){
    live_log_enable = true;
}

void disable_live_log(){
    live_log_enable = false;
}

void cease_logging(){
    cease_log = true;
}

void resume_logging(){
    cease_log = false;
}

int log_pop(high_freq_log_t *data){
    int res = (count > 1) ? 0 : -1;
    if(count > 0){
        *data = data_log[r_ptr];
        r_ptr = (r_ptr + 1) % LOG_SIZE;
        count--;
    }
    return res;
}
