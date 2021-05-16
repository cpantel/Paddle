/*
 * ciaaI2C.c
 *
 *  Created on: Jun 16, 2014
 *      Author: pablo
 */


#include "ciaaI2C.h"


void ciaaI2CInit(void)
{
	//configuro i2c y lo activo
	Chip_I2C_Init(I2C0);
	Chip_I2C_SetClockRate(I2C0, 100000);
	Chip_SCU_I2C0PinConfig(I2C0_STANDARD_FAST_MODE);

}

Status ciaaI2CWrite(uint8_t sl_addr, uint8_t * buffer, int len)
{
	I2CM_XFER_T xfer;

	xfer.slaveAddr = sl_addr;
	xfer.txBuff = buffer;
	xfer.txSz = len;
	xfer.rxBuff = 0;
	xfer.rxSz = 0;
	xfer.options = 0;
	xfer.status = 0;

	Chip_I2CM_XferBlocking(LPC_I2C0, &xfer);

	if(xfer.status == I2CM_STATUS_OK) return SUCCESS;
	else return ERROR;

}

Status ciaaI2CRead(uint8_t sl_addr, uint8_t * buffer, int len)
{
	I2CM_XFER_T xfer;

	xfer.slaveAddr = sl_addr;
	xfer.txBuff = 0;
	xfer.txSz = 0;
	xfer.rxBuff = buffer;
	xfer.rxSz = len;
	xfer.options = 0;
	xfer.status = 0;

	Chip_I2CM_XferBlocking(LPC_I2C0, &xfer);

	if(xfer.status == I2CM_STATUS_OK) return SUCCESS;
	else return ERROR;
}
