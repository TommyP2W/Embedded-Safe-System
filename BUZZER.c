/*
	 ################################
	 Principle Author:

	 Tommy Peach, 100358179

	 File name:

	 BUZZER.c

	 Description:

     BUZZER.c initialises the buzzer, and includes functions for setting and resetting the pin for the buzzer

     ################################
 */

#include <stdio.h>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
#include "buzzer.h"

GPIO_InitTypeDef gpio_buzz;
	
// Function to initialise the buzzer
void init_buzzer(){
	__HAL_RCC_GPIOH_CLK_ENABLE(); // GPIO port H
	
	gpio_buzz.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_buzz.Pull = GPIO_NOPULL;
	gpio_buzz.Speed = GPIO_SPEED_HIGH;
	gpio_buzz.Pin = GPIO_PIN_6;     // Pin 6
	
	HAL_GPIO_Init(GPIOH, &gpio_buzz); // Initialise the pin

}
// Function to turn on the buzzer
void buzz_on(){
		HAL_GPIO_WritePin(GPIOH, gpio_buzz.Pin, GPIO_PIN_SET);
}

// Function to turn off the buzzer
void buzz_off(){
	HAL_GPIO_WritePin(GPIOH, gpio_buzz.Pin, GPIO_PIN_RESET);
}