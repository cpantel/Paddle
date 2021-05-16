/*
 * ciaaAIN.c
 *
 *  Created on: Jun 24, 2014
 *      Author: pablo
 */

#include "ciaaAIN.h"

uint16_t current_in[4];

void ADC0_IRQHandler(void)
{
	if(Chip_ADC_ReadStatus(LPC_ADC0, ADC_CH1, ADC_DR_DONE_STAT)==SET)
	{
		Chip_ADC_ReadValue(LPC_ADC0, ADC_CH1, current_in);
	}
	if(Chip_ADC_ReadStatus(LPC_ADC0, ADC_CH2, ADC_DR_DONE_STAT)==SET)
	{
		Chip_ADC_ReadValue(LPC_ADC0, ADC_CH2, current_in+1);
	}
	if(Chip_ADC_ReadStatus(LPC_ADC0, ADC_CH3, ADC_DR_DONE_STAT)==SET)
	{
		Chip_ADC_ReadValue(LPC_ADC0, ADC_CH3, current_in+2);
	}
	if(Chip_ADC_ReadStatus(LPC_ADC0, ADC_CH4, ADC_DR_DONE_STAT)==SET)
	{
		Chip_ADC_ReadValue(LPC_ADC0, ADC_CH4, current_in+3);
	}
}

void ciaaAINInit(void)
{
	ADC_CLOCK_SETUP_T clk = {200000, 10, true};

	Chip_ADC_Init(LPC_ADC0, &clk);

	Chip_ADC_EnableChannel(LPC_ADC0, ADC_CH1, ENABLE);
	Chip_ADC_EnableChannel(LPC_ADC0, ADC_CH2, ENABLE);
	Chip_ADC_EnableChannel(LPC_ADC0, ADC_CH3, ENABLE);
	Chip_ADC_EnableChannel(LPC_ADC0, ADC_CH4, ENABLE);

	Chip_ADC_Int_SetChannelCmd(LPC_ADC0, ADC_CH1, ENABLE);
	Chip_ADC_Int_SetChannelCmd(LPC_ADC0, ADC_CH2, ENABLE);
	Chip_ADC_Int_SetChannelCmd(LPC_ADC0, ADC_CH3, ENABLE);
	Chip_ADC_Int_SetChannelCmd(LPC_ADC0, ADC_CH4, ENABLE);

	Chip_ADC_SetBurstCmd(LPC_ADC0, ENABLE);

	NVIC_EnableIRQ(ADC0_IRQn);
}

uint16_t ciaaAINRead(uint8_t input)
{
	if(input > 3) return -1;
	return current_in[input];
}
