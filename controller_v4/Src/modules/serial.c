#include "modules/serial.h"
#include "modules/log.h"

void download_log();

static char buffer[32];
static UART_HandleTypeDef *_huart;

void (*TxCpltCallback)();

static char data[32];
static uint8_t command;
static uint16_t id;
static HAL_StatusTypeDef status;

Address derive_address(uint16_t id){
  return (0x081FFFFF - ((id + 1) * 4));
}

void serial_init(UART_HandleTypeDef *huart){
    _huart = huart;
    HAL_UART_Receive_IT(_huart, buffer, 7);
}

void error_handler()
{
  HAL_UART_Transmit(_huart, "ERROR!!!!", 9, 0xFFFF);
}

void _handle(){
  for(int i = 0; i < 4; i++){
    data[i] = buffer[6-i];
  }
  command = buffer[0];
  id = (buffer[1] << 8) | buffer[2];
  switch(command){
    case(0xEE):
      status = flash_erase_sector(FLASH_SECTOR_11, VOLTAGE_RANGE_3, error_handler);
      break;
    case(0x97):
      status = flash_program_sequential(derive_address(id), data, error_handler);
      break;
    case(0xaa):
      //update params
    case(0x1f):
      set_frequency(HIGH_FREQ);
      enable_live_log();
      return;
    case(0x1e):
      set_frequency(LOW_FREQ);
      enable_live_log();
      return;
    case(0x10):
      //disable_live_log();
      return;
    case(0x1d):
      HAL_UART_Transmit(_huart, "DOWNLOAD_ACK!!!!", 16, 0xFFFF);
      HAL_UART_Transmit(_huart, "LEN_5000!!!!", 12, 0xffff);
      cease_logging();
      download_log();
      break;
    default:
      error_handler();
      status = HAL_ERROR;
      break;
  }
  if(status == HAL_OK){
    HAL_UART_Transmit(_huart, "ACK!!!!", 7, 0xFFFF);
  }
  else{
    HAL_UART_Transmit(_huart, "NACK!!!!", 8, 0xFFFF);
  }
}

int8_t send_message_DMA(uint8_t data[], int length, void (*onComplete)()){
  if (_huart->gState != HAL_UART_STATE_READY){
    return -1;
  }
  TxCpltCallback = onComplete;
  HAL_UART_Transmit_DMA(_huart, data, length);
  return 0;
}

HAL_UART_StateTypeDef getUARTState(){
  return _huart->gState;
}

void download_log(){
  high_freq_log_t log;
  int ptr = 0;
  uint8_t buf[(sizeof(high_freq_log_t) + 3)];
  int status = log_pop(&log);
  if(status == -1){
    HAL_UART_Transmit(_huart, "LOG_END!!!!", 11, 0xFFFF);
    resume_logging();
  }
  else{
    buf[ptr++] = 0x11;
    memcpy(&buf[ptr], &log, sizeof(high_freq_log_t));
    ptr = ptr + sizeof(high_freq_log_t);
    buf[ptr++] = 0x21;
    buf[ptr++] = 0x21;
    buf[ptr++] = 0x21;
    buf[ptr++] = 0x21;
    send_message_DMA(buf, 73, download_log);
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  _handle();
  HAL_UART_Receive_IT(_huart, buffer, 7);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  TxCpltCallback();
}
