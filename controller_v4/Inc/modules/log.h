#ifndef __LOG_H
#define __LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "_data.h"

#define LOG_SIZE 5000
#define LOG_SIZE_HALF 2500
#define LIVE_LOG_SIZE 500
#define LIVE_LOG_DMA_LENGTH_HIGH_FREQ (((sizeof(high_freq_log_t) + 5) * LIVE_LOG_SIZE) + 10)
#define LIVE_LOG_DMA_LENGTH_LOW_FREQ (sizeof(low_freq_log_t) + 5 + 10)
typedef struct {
    float id_ref;
    float iq_ref;
    float id;
    float iq;
    float Vdc;
    float vd_ref;
    float vq_ref;
    float TqRef;
    float TqLim;
    float TqRequest;
    float rpm;
    float motorTemp;
    float igbtTemp;
    float power;
    uint32_t faults;
} low_freq_log_t;

typedef struct {
    float id_ref;
    float iq_ref;
    float id;
    float iq;
    float Vdc;
    float vd_ref;
    float vq_ref;
    float TqRef;
    float TqLim;
    float TqRequest;
    float rpm;
    float motorTemp;
    float igbtTemp;
    float power;
    uint32_t faults;
    float ia;
    float ib;
    float ic;
    float theta;
} high_freq_log_t;

typedef enum {
  LOW_FREQ,
  HIGH_FREQ
} log_frequency_t; 

/**
 * Initialize the log module
 * 
 */
void log_init();

/**
 * Cease logging.
 */
void cease_logging();

/**
 * Resume logging.
 */
void resume_logging();

/**
 * Add data struct to log queue
 * 
 * @param data to be added to log
 */
void log_push(MotorController_Variables* data);

/**
 * read from the log queue
 * 
 * @param Data pointer which will contain the Data struct
 * @return 0 on success, -1 if empty.
 */
int log_pop(high_freq_log_t *data);

/**
 * enable the live log
 * 
 */
void enable_live_log();

/**
 * disable the live log
 * 
 */
void disable_live_log();
/*
  set the frequency
 */
void set_frequency(log_frequency_t f);

#ifdef __cplusplus
}
#endif

#endif /* __LOG_H */
