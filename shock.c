/*
	 ################################
	 Principle Author:

	 Tommy Peach, 100358179

	 File name:

	 shock.c

	 Description:

     shock.c is responsible for initialising the shock sensor. It also sets the priority for the interrupt for the shock sensor and enables the handler for the shock sensor.

     ################################
 */
#include <stdio.h>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
#include "shock.h"
void init_shock(void){

	GPIO_InitTypeDef gpio;	
	__HAL_RCC_GPIOG_CLK_ENABLE(); 
	gpio.Pin = GPIO_PIN_6;      // Setting relevant pin 6 for GPIOG

    /* Interrupt mode, detects interrupt if the voltage goes low to high.
     * This seems to make sense as after looking up how the shock sensor works, it seems once the inside coil touches the outer casing it will return high.
     * This mode was found in stm32f7xx_hal_gpio.h.
     */
	gpio.Mode = GPIO_MODE_IT_RISING;

	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOG, &gpio);

    /* Setting the interrupt priority, 0 being the highest priority as stated in lecture notes.
     * According to the reference manual (https://learn.uea.ac.uk/bbcswebdav/pid-4513906-dt-content-rid-29049770_1/xid-29049770_1, page 296)
     * and the vector table in startup_stm32f746xx.s,
     * EXTI9_5_IRQn seems to be the interrupt service handler for pins 5 - 9, as shock is pin 6.
     */
     HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
     // Enabling the interrupt service
     HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	
}