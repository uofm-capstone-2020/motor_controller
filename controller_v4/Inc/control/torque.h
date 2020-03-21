#ifndef __TORQUE_H
#define __TORQUE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Update the torque request read from the CAN bus
 * 
 * @param torque the CAN bus torque reading
 */
#include "main.h"
void Torque_step(void);

#ifdef __cplusplus
}
#endif

#endif /* __TORQUE_H */
