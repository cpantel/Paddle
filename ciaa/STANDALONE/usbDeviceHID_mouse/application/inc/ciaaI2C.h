/*
 * ciaaI2C.h
 *
 *  Created on: Jun 16, 2014
 *      Author: pablo
 */

#ifndef CIAAI2C_H_
#define CIAAI2C_H_

#include "chip.h"

void ciaaI2CInit(void);
Status ciaaI2CWrite(uint8_t sl_addr, uint8_t * buffer, int len);
Status ciaaI2CRead(uint8_t sl_addr, uint8_t * buffer, int len);

#endif /* CIAAI2C_H_ */
