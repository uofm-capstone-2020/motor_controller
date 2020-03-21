/*
 * PGA411.h
 *
 *  Created on: Oct 21, 2019
 *      Author: jolen
 */

#ifndef PGA411_H_
#define PGA411_H_

#endif /* PGA411_H_ */

#define CRC_INITSEED 0x3F
#define CRC_BYTECOUNT 24 /* value taken from manual pg 56 */
#define CRC_POLYNOM 6  /* not sure about this value, the highest polynomial is 6 */
#define SPI_DUMMY 0x00
#define PGA411_REG_COUNT 28 /* Number of registers you can read from the PGA411 */
#define ONE_BYTE 0x01
#define FOUR_BYTES 0x04
#define TIME_OUT 100
#define LOW 0
#define HIGH 1
#define DEV_OVUV1 0
#define DEV_OVUV2 1
#define DEV_OVUV3 2
#define DEV_OVUV4 3
#define DEV_OVUV5 4
#define DEV_OVUV6 5
#define DEV_TLOOP_CFG 6
#define DEV_AFE_CFG 7
#define DEV_PHASE_CFG 8
#define DEV_CONFIG1 9
#define DEV_CONTROL1 10
#define DEV_CONTROL2 11
#define DEV_CONTROL3 12
#define DEV_STAT1 13
#define DEV_STAT2 14
#define DEV_STAT3 15
#define DEV_STAT4 16
#define DEV_STAT5 17
#define DEV_STAT6 18
#define DEV_STAT7 19
#define DEV_CLCRC 20
#define DEV_CRC 21
#define CRCCALC 22
#define DEV_EE_CTRL1 23
#define DEV_CRC_CTRL1 24
#define DEV_EE_CTRL4 25
#define DEV_UNLK_CTRL1 26
#define ANGLE_FRAME 0x41000029
#define VELOCITY_FRAME 0x6F000003
#define FAULT_PIN_ERROR_BIT 0
#define CRC_ERROR_BIT 1
#define FAFECAL_BIT 0b0000000010000000
#define DEV_STAT4_FAULT_BITS 0b1111111011110000
#define VELOCITY_MULTIPLIER 3.74507028292

typedef struct {
	__UINT8_TYPE__ addr;
	__UINT16_TYPE__ dataout;
	__UINT8_TYPE__ reserved;
	__UINT8_TYPE__ mcrc;
	__UINT8_TYPE__ scrc;
	__UINT16_TYPE__ datain;
	__UINT32_TYPE__ frame;
} pga411_spi_frame_t;

typedef struct {
	__UINT16_TYPE__ reg;			/* PGA Register Address */
	__UINT16_TYPE__ read_add; 		/* SPI Read Address */
	__UINT16_TYPE__ write_add;	 	/* SPI Write Address */
	__UINT16_TYPE__ def_val;  		/* Default value */
	__UINT16_TYPE__ real_val; 		/* Real value */
} pga411_regs_t;


enum{READ,WRITE}; /* Enum selects SPI operation*/
enum{DIAG,NORMAL}; /* Operational states of PGA */

void pga411_Init(void);
void pga411_FaultReset(void);
void pga411_ReadAll(void);
void pga411_checkFaults(void);
void update_rdc_values(void);
