/*
 * PGA411.c
 *
 *  Created on: Oct 21, 2019
 *      Author: jolen
 *
 * Check out datasheet: http://www.ti.com/lit/ds/symlink/pga411-q1.pdf
 * Check out software manual: http://www.ti.com/lit/an/slaa708/slaa708.pdf
 * Check out so much documentation: http://www.ti.com/product/PGA411-Q1/technicaldocuments
 *
 * If you have any issues with this chip, TI also has great support and will get back to you in a few days.
 *
 * The PGA411 IC is a resolver to digital chip. It communicates using SPI with the STM32. The STM32 can set
 * parameters in the PGA411 like excitation voltage, frequency, and what faults to mask. You can also read
 * what faults are apparent in it's fault registers. The PGA411 creates the excitation wave to be send to the
 * resolver and then reads the cosine and sine waves that are returned from the resolver. It does all the math
 * for you to get the resolver angle and velocity.
 */

#include "sensors/PGA411.h"
#include "main.h"

static uint16_t pga411_crc2(uint32_t);
static void pga411_Reset(void);
static void pga411_DefaultCfg(void);
static pga411_spi_frame_t pga411_XmitSPI(uint16_t,pga411_regs_t,uint16_t);
static uint16_t pga411_ReadReg(pga411_regs_t reg);
static void pga411_State(uint16_t state);
static void pga411_DeviceUnlock(void);
static void pga411_EEPROMUnlock(void);
static void pga411_getFaultsRegs(void);
static void request_velocity(void);
static void spi_txrx_byte(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData);
static uint32_t hal_Xmit4BSPI(uint32_t mosi);
static uint32_t fast_txrx(uint32_t mosi);

uint8_t ignore_crc_error;
extern SPI_HandleTypeDef hspi2;

pga411_regs_t pga411_regs[PGA411_REG_COUNT] = {

	[DEV_OVUV1] = { 0x00 , 0x53 , 0x87, 0xAB40 }, // 1010101101000000
		// 1010 = EXTOUT_GL = pre-excitation gain
		// 101 = EXTILIMTH_L1_2 = 230 mA exciter amp current limit low level
		// 101 = EXTILIMTH_H1_2 = 230 mA exciter amp current limit high level
		// 000 = OSHORTL = Vcc*0.475 OSIN/OCOS low threshold level
		// 000 = OSHORTH = Vcc*0.525 OSIN/OCOS high threshold level
	[DEV_OVUV2] = { 0x01 , 0x6B , 0x26, 0x00ED }, // 11101101
		// 0 = XEXT_AMP = Use an internal exciter amplifier
		// 11 = TRDHL = 8ms tracking loop error deglitch select
		// 101 = DVMSENH = 3.35 V IZx input integrity check high threshold
		// 101 = DVMSENL = 1.65 V IZx input integrity check low threshold
	[DEV_OVUV3] = { 0x02 , 0x65 , 0x17, 0xFCFF }, // 1111110011111111
		// 111 = EXTUVT = 405us Exciter Amplifier Undervoltage Deglitch
		// 111 = EXTOVT = 15.2us Exciter Amplifier Overvoltage Deglitch
		// 00 = OVIZL = Vcc*0.25 IZx Input Overvoltage Low Threshold level select
		// 11 = OVIZH = Vcc*0.9 IZx Input Overvoltage High Threshold level select
		// 111 = OOPENTHL = Vcc*0.075 OSIN/OCOS open circuit low Threshold
		// 111 = OOPENTHH = Vcc*0.925 OSIN/OCOS open circuit high Threshold
	[DEV_OVUV4] = { 0x03 , 0xEC , 0x39, 0x07E6 }, // 11111100110
		// 111 = TSHORT = 140us OSIN/OCOS short circuit deglitch select
		// 111 = TEXTMON = 140us Exciter Monitor Faults response delay time (FEXTMONL & FEXTMONH)
		// 00 = AUTOPHASE_CFG = enhanced auto phase by using short fault detection threshold levels
		// 1 = VEXT_CFG = VEXT supply used for exciter amplifier
		// 1 = nBOOST_FF = Boost forward feed controlled turned off
		// 0 = FSHORT_CFG = AND of sin, cos short fault condition
	[DEV_OVUV5] = { 0x04 , 0x52 , 0x75, 0x1C00 }, // 1110000000000
		// 111 = TOPEN = 140us OSIN/OCOS open circuit deglitch select
		// 0000000000 = RESERVED
	[DEV_OVUV6] = { 0x05 , 0xE9 , 0x83, 0x038F }, // 1110001111
		// 111 = IZTHL = 15.2us
		// 0 = BOOST_VEXT_MASK = no mask
		// 00 = RESERVED
		// 11 = LPETHL = 0.8V Tracking loop error low threshold
		// 11 = LEPTHH = 0.8V Tracking Loop Error High Threshold
	[DEV_TLOOP_CFG] = { 0x06 , 0xA6 , 0x42, 0x0514 }, // 10100010100
		// 10 = MKP = 32 digital tracking loop gain multiplier
		// 100 = DKP = 64 digital tracking loop gain constant
		// 01 = OHYS = 1 least sig. bit for output angle hysteresis
		// 0 = SENCLK = Exciter clock is referenced to system clock
		// 100 = DKI = 2^12 bits PI controller feedback integration weight
	[DEV_AFE_CFG] = { 0x07 , 0xC2 , 0x91, 0x0005 }, // 0101
		// 01 = GAINCOS = 1 COS input AFE gain
		// 01 = GAINSIN = 1 SIN input AFE gain
	[DEV_PHASE_CFG] = { 0x08 , 0x57 , 0x85, 0x1800 }, // 101000000000
		// 00 = EXTUVT_CFG = UV or UV2 with 50 to 400us deglitching on OR inputs
		// 0 = PDEN = manual phase delay is disabled
		// 1 = APEN = automatic phase control enabled
		// 01 = EXTMMODE = 7 VRMS exciter mode
		// 0000 = EXTOUT = 1.2V exciter offset voltage adjust (footroom)
		// 000000 = PHASEDEMOD = 1 SIN input AFE gain
	[DEV_CONFIG1] = { 0x09 , 0xBE , 0xEB, 0x0084 }, // 010000100
		// 01 = NPLE = 2x pole pairs
		// 0 = RESERVED
		// 000 = SELFEXT = 10kHz automatic phase control enabled
		// 100 = MODEXT = 12V to 16 V power supply for exciter
	[DEV_CONTROL1] = { 0x0A , 0x90 , 0x0D, 0x2FFE }, /* Must unlock to write to reg */
	[DEV_CONTROL2] = { 0x0B , 0x63 , 0x38, 0x001F }, /* Must unlock to write to reg */
	[DEV_CONTROL3] = { 0x0C , 0xDD , 0xAE, 0x0000 },
	[DEV_STAT1] = { 0x0D , 0x81}, /* read only */
	[DEV_STAT2] = { 0x0E , 0x4D}, /* read only */
	[DEV_STAT3] = { 0x0F , 0x84}, /* read only */
	[DEV_STAT4] = { 0x10 , 0x1F}, /* read only */ // faults
	[DEV_STAT5] = { 0x11 , 0x41}, /* read only */ // theta
	[DEV_STAT6] = { 0x12 , 0x6F}, /* read only */ // velocity
	[DEV_STAT7] = { 0x13 , 0xE1}, /* read only */
	[DEV_CLCRC] = { 0x14 , 0x4F , 0xFC },
	[DEV_CRC] = { 0x15 , 0x0F , 0xE7 },
	[CRCCALC] = { 0x16 , 0xD9 }, /* read only */
	[DEV_EE_CTRL1] = { 0x17 , 0xE3 , 0x6E },  /* Must unlock eeprom to write to reg */
	[DEV_CRC_CTRL1] = { 0x18 , 0x7A , 0xB6 },
	[DEV_EE_CTRL4] = { 0x19, 0xBA , 0x56 },
	[DEV_UNLK_CTRL1] = { 0x1A , 0x64 , 0x95 }
};


/* CRC6 calculation - optimized for PGA411*/
/* taken from https://e2e.ti.com/support/sensors/f/1023/t/774733?tisearch=e2e-sitesearch&keymatch=tida-00796
 * tested function with CRC values given in PGA411 user manual on page 55 and passed */
uint16_t pga411_crc2(uint32_t datin) {
	uint16_t byte_idx, bit_idx, crc = 0x3F;

    for (byte_idx = 18; byte_idx >= 0 && byte_idx <= 18; byte_idx-=6)
    {
           crc ^= (datin >> byte_idx) & 0x3F;


           for (bit_idx = 0; bit_idx < 6; bit_idx++)
          {
             crc <<= 1;
             crc ^= (crc & 0x40) ? 0x5B : 0;
           }
    }
    return (crc & 0x3F);
}

/* Initialization PGA411-Q1 related hardware*/
void pga411_Init(void) {
	pga411_Reset(); /* perform PGA411 reset*/
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, RDC_FAULTRES_Pin, LOW); /* assert FAULTRES low - open collector */
	pga411_DefaultCfg();
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, RDC_FAULTRES_Pin, HIGH); /* return back to high-z*/
	HAL_Delay(100);
	pga411_ReadAll();
	request_velocity();
}

/* Perform PGA411-Q1 reset using GPIO*/
void pga411_Reset(void) {
	HAL_GPIO_WritePin(GPIOD, RDC_NRST_Pin, LOW); /* RESET_Nlow */
	HAL_Delay(1); /* hold in reset for 500us*/
	HAL_GPIO_WritePin(GPIOD, RDC_NRST_Pin, HIGH); /* release from reset */
	HAL_Delay(10); /* wait at least 10 ms */
}

/* Perform PGA411-Q1 FAULT reset using GPIO */
void pga411_FaultReset(void) {
	HAL_GPIO_WritePin(GPIOB, RDC_FAULTRES_Pin, LOW); /* assert FAULTRES low - open collector */
	HAL_Delay(1); /* hold in the state for 1ms */
	HAL_GPIO_WritePin(GPIOB, RDC_FAULTRES_Pin, HIGH); /* return back to high-z*/
}

/* Write default configuration to PGA */
void pga411_DefaultCfg(void) {
	uint8_t i;
	/* go to diagnostic state */
	pga411_State(DIAG);
	/* unlock registers */
	pga411_DeviceUnlock();
	/* first 12 registers to default*/
	for (i = 0; i < 12; i++) {
		pga411_XmitSPI(WRITE, pga411_regs[i], pga411_regs[i].def_val);
		pga411_ReadReg(pga411_regs[i]);
	}
	/* go to normal state */
	pga411_State(NORMAL);
}

/* Transmit data to PGA over SPI, for reg use defined macros */
pga411_spi_frame_t pga411_XmitSPI(uint16_t dir,pga411_regs_t reg,uint16_t wdata) {
	pga411_spi_frame_t out, in;

	/* do we read data? */
	if (dir== READ){ out.addr = reg.read_add;} /* read address*/
	/* or write data? */
	else{ out.addr= reg.write_add;} /* write address*/

	/* compose the rest of the frame*/
	out.dataout = wdata; /* realdata(W) or dummydata(R) */
	out.reserved= 0x00;/* always zero */

	out.frame = out.addr;
	out.frame = out.frame << 16;
	out.frame ^= (out.dataout & 0x00FFFF);

	out.mcrc= pga411_crc2(out.frame); /* calculateTX CRC */
	out.frame = out.frame << 8;
	out.frame ^= (out.mcrc&0x00000003F);

	in.frame = hal_Xmit4BSPI(out.frame);
	in.scrc = in.frame & 0x00000003F;
	in.frame = in.frame >> 8;
	uint8_t crc2 = pga411_crc2(in.frame);
	in.datain = in.frame & 0x00FFFF;
	in.addr = in.frame >> 16;

	/* checkRX CRC */
	if (crc2!= in.scrc) {
		asm("NOP"); /* if error -> terminate */
	}

	return in;
}

/* Read data from defined register*/
uint16_t pga411_ReadReg(pga411_regs_t reg) {
	uint16_t rx_data;
	// transmit that you want to read the register

	pga411_XmitSPI(READ, reg, SPI_DUMMY);
	// transmit again to get the response
	rx_data = pga411_XmitSPI(READ, reg, SPI_DUMMY).datain;
	reg.real_val = rx_data;
	return(rx_data);
}

/* Write to a register  */
void pga411_WriteReg(pga411_regs_t reg, uint16_t data) {
	/* here we just making it nice,can be macro too */
	pga411_XmitSPI(WRITE,reg,data);
}

/* Read all registers from PGA */
void pga411_ReadAll(void) {
	uint16_t i;
	uint16_t rx_data;

	/* register by register (27+1) */
	/* PGA411 always returns previous read register value*/
	/* thats why 28 reads are needed (first read returns whatever data)*/
	for (i = 0; i < 28; i++) {
		if (i<27) {
			rx_data= pga411_XmitSPI(READ,pga411_regs[i], SPI_DUMMY).datain;
		} else { // don't ask for register that doesn't exist or it'll throw a fault
			rx_data= pga411_XmitSPI(READ,pga411_regs[i-1], SPI_DUMMY).datain;
		}
		// don't add value on the first read
		if(i) {
			pga411_regs[i-1].real_val= rx_data;
		}
	}
}

/* Change state diagnostic/normal */
void pga411_State(uint16_t state) {
	uint16_t temp;
	pga411_regs_t reg;

	/* Enter Diagnostic state*/
	if (state== DIAG) {
		/* Set DIAG bit in DEV_CONTROL3 */
		pga411_WriteReg( pga411_regs[DEV_CONTROL3], 0x0004);
	} else { /* Go back to normal state */
		/* read content of  DEV_CONTROL1 register */
		reg = pga411_regs[DEV_CONTROL1];
		temp= pga411_ReadReg(reg);
		pga411_DeviceUnlock();
		temp|= 0x0001; /* set bit DIAGEXIT */
		pga411_WriteReg(reg, temp); /* finish R-M-W*/
	}
}

/* Device unlock (must be in diagnostic state)*/
void pga411_DeviceUnlock(void) {
	hal_Xmit4BSPI(0x95000F39);
	hal_Xmit4BSPI(0x95005522);
	hal_Xmit4BSPI(0x9500AA25);
	hal_Xmit4BSPI(0x9500F03E);

}

/* EEPROM unlock is required to access DEV_CONTROL1 and DEV_CONTROL2 Registers
 * (must be in diagnostic state)*/
void pga411_EEPROMUnlock(void) {
	hal_Xmit4BSPI(0x56000F);
	hal_Xmit4BSPI(0x560055);
	hal_Xmit4BSPI(0x5600AA);
	hal_Xmit4BSPI(0x5600F0);
}

/* Get the fault register values */
void pga411_getFaultsRegs(void) {
	mc_faults.rdc_faults_reg1 = pga411_ReadReg(pga411_regs[DEV_STAT1]);
	mc_faults.rdc_faults_reg2 = pga411_ReadReg(pga411_regs[DEV_STAT3]);
	mc_faults.rdc_faults_reg3 = pga411_ReadReg(pga411_regs[DEV_STAT4]);
	mc_faults.rdc_faults_reg3 &= DEV_STAT4_FAULT_BITS;
	mc_faults.rdc_faults_reg4 = pga411_ReadReg(pga411_regs[DEV_STAT7]);
	mc_faults.rdc_faults_reg4 &= FAFECAL_BIT;
}

/* Check for faults by looking in the fault registers */
void pga411_checkFaults(void) {
	pga411_getFaultsRegs();
	if (mc_faults.rdc_faults_reg1 > 0 || mc_faults.rdc_faults_reg2 > 0 ||
		mc_faults.rdc_faults_reg3 > 0 || mc_faults.rdc_faults_reg4 > 0) {
		mc_vars.fault_reg1 |= 1 << RDC_FAULT_REG_FLAG_BIT;
		Error_Handler();
	}
	ignore_crc_error = TRUE;
	request_velocity();
	ignore_crc_error = FALSE;
}

/* Transmit four bytes */
uint32_t hal_Xmit4BSPI(uint32_t mosi) {
	uint32_t miso = 0; /* in coming data*/
	uint8_t buffer1 = 0;
	uint8_t buffer2 = 0;

	/* Send data byte by byte */
	HAL_GPIO_WritePin(GPIOB, RDC_CS_Pin, 0);
	miso = buffer2;
	// first byte
	buffer1 = mosi >> 24;
	HAL_SPI_TransmitReceive(&hspi2,&buffer1,&buffer2,ONE_BYTE,TIME_OUT); /* most significant byte first */
	miso = buffer2;

	// second byte
	buffer1 = mosi >> 16;
	HAL_SPI_TransmitReceive(&hspi2,&buffer1,&buffer2,ONE_BYTE,TIME_OUT);
	miso= ((miso<< 8) | buffer2);

	// third byte
	buffer1 = mosi >> 8;
	HAL_SPI_TransmitReceive(&hspi2,&buffer1,&buffer2,ONE_BYTE,TIME_OUT);
	miso= ((miso<< 8) | buffer2);

	// last byte
	buffer1 = mosi;
	HAL_SPI_TransmitReceive(&hspi2,&buffer1,&buffer2,ONE_BYTE,TIME_OUT);
	miso= ((miso<< 8) | buffer2);
	HAL_GPIO_WritePin(GPIOB, RDC_CS_Pin, 1);

	// NCS needs to be high for at least 200 ns

	return(miso);
}

/* Request velocity */
void request_velocity(void) {
	ignore_crc_error = TRUE;
	fast_txrx(VELOCITY_FRAME); 	// get theta ask for velocity
	ignore_crc_error = FALSE;
}

/* Update velocity and theta */
void update_rdc_values(void) {
	uint32_t theta_raw, velocity_raw;
	float velocity_float;

	velocity_raw = fast_txrx(ANGLE_FRAME); 	// get velocity ask for theta
	theta_raw = fast_txrx(VELOCITY_FRAME); 	// get theta ask for velocity

	theta_raw &=  0x01FFF; 		// keep only 12 bits
	mc_vars.theta_e = (float) (theta_raw * TWOPI / 4096);	// convert 12 bits to 16 bits to make taking the modulus of two pi easier
	mc_vars.theta_e *= mc_params.p;		// convert 12 mechanical theta to electrical theta
	mc_vars.theta_e -= mc_params.resolver_offset; // Subtract offset

	/* Take mod of theta_e */
	while (mc_vars.theta_e > TWOPI) {
		mc_vars.theta_e -= TWOPI;
	}

	// theta +/- 2PI/3 is used a lot so might as well calculate it here
	mc_vars.theta_plus_2PIdiv3 = mc_vars.theta_e + TWOPIDIV3;
	if (mc_vars.theta_plus_2PIdiv3 > TWOPI) {
		mc_vars.theta_plus_2PIdiv3 -= TWOPI;
	}
	mc_vars.theta_minus_2PIdiv3 = mc_vars.theta_e - TWOPIDIV3;
	if (mc_vars.theta_minus_2PIdiv3 <0) {
		mc_vars.theta_minus_2PIdiv3 += TWOPI;
	}

	// This might be able to be refactored just using signed ints
	if (velocity_raw & 0x0800) { /* negative number? */
		/* convert to positive number first*/
		velocity_float= (((~velocity_raw)+1) & 0x07FF);
		velocity_float*= -1; /* and makeit negative*/
	} else {
		/* positive number ,preserve only needed bits */
		velocity_float= (velocity_raw & 0x07FF);
	}

	mc_vars.wElectrical =  (float) ((velocity_float+1) * VELOCITY_MULTIPLIER * 10.0);
}

/*
 * Send & Recieve 1 byte over serial
 * This was refactored from HAL since HAL's TxRx is really slow and bloated.
 */
void spi_txrx_byte(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData) {
	/* Set the transaction information */
	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pRxBuffPtr  = (uint8_t *)pRxData;
	hspi->RxXferCount = ONE_BYTE;
	hspi->RxXferSize  = ONE_BYTE;
	hspi->pTxBuffPtr  = (uint8_t *)pTxData;
	hspi->TxXferCount = ONE_BYTE;
	hspi->TxXferSize  = ONE_BYTE;

	/*Init field not used in handle to zero */
	hspi->RxISR       = NULL;
	hspi->TxISR       = NULL;


	SET_BIT(hspi->Instance->CR2, SPI_RXFIFO_THRESHOLD);
	*(__IO uint8_t *)&hspi->Instance->DR = (*hspi->pTxBuffPtr);
	hspi->pTxBuffPtr++;
	hspi->TxXferCount--;

	while (hspi->RxXferCount > 0U)
	{
	  /* Wait until RXNE flag is reset */
	  if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE))
	  {
		  (*(uint8_t *)hspi->pRxBuffPtr) = *(__IO uint8_t *)&hspi->Instance->DR;
		  hspi->pRxBuffPtr++;
		  hspi->RxXferCount--;
	  }
	}
}

/*
 * Send and recieve 4 bytes (32 bits - the size of the frame sent to/from the PGA411)
 */
uint32_t fast_txrx(uint32_t mosi) {
	uint32_t miso = 0; /* in coming data*/
	uint8_t buffer1 = 0;
	uint8_t buffer2 = 0;
	uint8_t crc_in,crc_calc;

	/* Send data byte by byte */
	HAL_GPIO_WritePin(GPIOB, RDC_CS_Pin, 0);
	miso = buffer2;
	// first byte
	buffer1 = mosi >> 24;
	spi_txrx_byte(&hspi2,&buffer1,&buffer2); /* most significant byte first */
	miso = buffer2;

	// second byte
	buffer1 = mosi >> 16;
	spi_txrx_byte(&hspi2,&buffer1,&buffer2);
	miso= ((miso<< 8) | buffer2);

	// third byte
	buffer1 = mosi >> 8;
	spi_txrx_byte(&hspi2,&buffer1,&buffer2);
	miso= ((miso<< 8) | buffer2);

	// last byte
	buffer1 = mosi;
	spi_txrx_byte(&hspi2,&buffer1,&buffer2);
	// dont need the last byte, it's the crc
	crc_in = buffer2;
	crc_calc = pga411_crc2(miso);

	if (crc_in != crc_calc && !ignore_crc_error) {
		mc_vars.fault_reg1 |= 1 << RDC_CRC_FLAG_BIT;
		Error_Handler();
	}

	//miso= ((miso<< 8) | buffer2);
	HAL_GPIO_WritePin(GPIOB, RDC_CS_Pin, 1);

	// NCS needs to be high for at least 200 ns

	return(miso);
}
