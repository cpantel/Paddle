/*
 * ciaaInputs.c
 *
 *  Created on: Jun 12, 2014
 *      Author: pablo
 */

#include "ciaaIO.h"

ciaaPin_t inputs[8] = { {0,4},{0,8},{0,9},{1,9} };

ciaaPin_t outputs[8] = { {5,0},{5,1},{5,2},{0,14},{1,11},{1,12} };

void ciaaIOInit(void)
{
	Chip_GPIO_Init(LPC_GPIO_PORT);

   /* LEDs */
   Chip_SCU_PinMux(2,0,MD_PUP,FUNC4);  /* GPIO5[0], LED0R */
   Chip_SCU_PinMux(2,1,MD_PUP,FUNC4);  /* GPIO5[1], LED0G */
   Chip_SCU_PinMux(2,2,MD_PUP,FUNC4);  /* GPIO5[2], LED0B */
   Chip_SCU_PinMux(2,10,MD_PUP,FUNC0); /* GPIO0[14], LED1 */
   Chip_SCU_PinMux(2,11,MD_PUP,FUNC0); /* GPIO1[11], LED2 */
   Chip_SCU_PinMux(2,12,MD_PUP,FUNC0); /* GPIO1[12], LED3 */

   Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<0)|(1<<1)|(1<<2),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 0,(1<<14),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 1,(1<<11)|(1<<12),1);

   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5,(1<<0)|(1<<1)|(1<<2));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 0,(1<<14));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 1,(1<<11)|(1<<12));

   /* Switches */
   Chip_SCU_PinMux(1,0,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[4], SW1 */
   Chip_SCU_PinMux(1,1,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[8], SW2 */
   Chip_SCU_PinMux(1,2,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[9], SW3 */
   Chip_SCU_PinMux(1,6,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO1[9], SW4 */

   Chip_GPIO_SetDir(LPC_GPIO_PORT, 0,(1<<4)|(1<<8)|(1<<9),0);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 1,(1<<9),0);
}

uint32_t ciaaReadInput(uint32_t inputNumber)
{
	return Chip_GPIO_ReadPortBit(LPC_GPIO_PORT,
			inputs[inputNumber].port,
			inputs[inputNumber].bit);
}

uint32_t ciaaWriteOutput(uint32_t outputNumber, uint32_t value)
{
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,
			outputs[outputNumber].port,
			outputs[outputNumber].bit,
			value);
	return 0;
}

void ciaaToggleOutput(uint32_t outputNumber)
{
	Chip_GPIO_SetPinToggle(LPC_GPIO_PORT,
			outputs[outputNumber].port,
			outputs[outputNumber].bit);
}
