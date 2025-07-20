/*
 ***********************************************************************************
 Author: Chaya Punnasri, 100386454
 Date of creation: 30 April 2024
 Name: led_main.c
 Description: Initialise and set/reset selected ports and pins for the use of an LED
 ***********************************************************************************
 */

#include <stdio.h> //include the library for a standard input and output
#include "stm32f7xx_hal.h" //include the library for HAL
#include "stm32f7xx_hal_gpio.h" // include the HAL library for a GPIO use
#include "led_main.h" // include the header file called led_main.h

// LED Red and Green gpio initialisation
void initialise_led(void){

    // this is for pin and port of green
	GPIO_InitTypeDef gpio; // initialise the type definition
	__HAL_RCC_GPIOI_CLK_ENABLE(); // enable clock for port I
	gpio.Pin = GPIO_PIN_0; // using pin 0
	gpio.Mode = GPIO_MODE_OUTPUT_PP; // output mode
	gpio.Pull = GPIO_NOPULL; // no pull for the GPIO output
	gpio.Speed = GPIO_SPEED_HIGH; // set the speed to high
	HAL_GPIO_Init(GPIOI, &gpio); // initialise port I pin 0

    // this is for pin and port of red
    __HAL_RCC_GPIOG_CLK_ENABLE(); // enable the clock for port G
	gpio.Pin = GPIO_PIN_7; // using pin 7
	gpio.Mode = GPIO_MODE_OUTPUT_PP; //output mode
	gpio.Pull = GPIO_NOPULL; // no pull for the GPIO output
	gpio.Speed = GPIO_SPEED_HIGH; // set the speed to high
	HAL_GPIO_Init(GPIOG, &gpio); // initialise port G pin 7
}

// function to set or clear the selected data port bit of a red colour
void turnOnRed(void){

    // set port G pin 7
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET);
    // clear/reset port I pin 0
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_RESET);
}

// function to set or clear the selected data port bit of a green colour
void turnOnGreen(void){

    // set port I pin 0
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET);
    // clear/reset port G pin 7
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET);
}


