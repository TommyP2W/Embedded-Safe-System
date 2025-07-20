/*
 ***************************************************************************************
 Author: Chaya Punnasri, 100386454
 Date of creation: 8 May 2024
 Name: tracking.c
 Description: The tracking sensor is being initialised and being used as an input GPIO
 ***************************************************************************************
 */

#include <stdio.h>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"

void initialise_input(void){
	// tracking sensor port initialisation
	GPIO_InitTypeDef gpio;
	__HAL_RCC_GPIOI_CLK_ENABLE(); // enable clock for base I
	gpio.Pin = GPIO_PIN_3; // pin 3
	gpio.Mode = GPIO_MODE_INPUT; // set mode to input as it needs to detect for the object moving nearby
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOI, &gpio);
}
