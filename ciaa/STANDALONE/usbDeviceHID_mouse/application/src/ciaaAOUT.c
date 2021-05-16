/*
 * ciaaAOUT.c
 *
 *  Created on: Jun 23, 2014
 *      Author: pablo
 */

#include "ciaaAOUT.h"


void ciaaAOUTInit(void)
{
	Chip_DAC_Init(LPC_DAC);

	Chip_DAC_ConfigDAConverterControl(LPC_DAC, DAC_DMA_ENA);

	Chip_DAC_UpdateValue(LPC_DAC, 0);
}

/* [in] level: Output level (0 - 100%) */
void ciaaAOUTSet(float level)
{
	if(level > 100) level = 100;

	Chip_DAC_UpdateValue(LPC_DAC, (uint32_t)((level * (float)0x3FF) / 100.0));
}
