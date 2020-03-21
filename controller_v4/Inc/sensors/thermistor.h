/*
 * thermistor.h
 *
 *  Created on: Oct 30, 2019
 *      Author: jolen
 */

#ifndef THERMISTOR_H_
#define THERMISTOR_H_

#include <stdint.h>
#include "main.h"

#define LOOPUP_TABLE_SIZE 50
#define HIGH 1
#define LOW  0
#define TEMP_OK    1
#define TEMP_ERROR 0

void update_motor_temp(uint32_t);
void update_IGBT_temp(uint32_t igbt_adc_voltage);
void update_temps(void);

static uint16_t IGBT_LOOKUP_TABLE[50] = { 999	,
		563	,
		196	,
		156	,
		134	,
		120	,
		110	,
		101	,
		94	,
		88	,
		83	,
		78	,
		74	,
		70	,
		67	,
		64	,
		61	,
		58	,
		55	,
		52	,
		50	,
		48	,
		45	,
		43	,
		41	,
		39	,
		37	,
		35	,
		33	,
		31	,
		29	,
		27	,
		25	,
		23	,
		21	,
		19	,
		17	,
		15	,
		13	,
		11	,
		9	,
		7	,
		5	,
		3	,
		1	,
		-1	,
		-4	,
		-6	,
		-9	,
		-12	};

#endif
