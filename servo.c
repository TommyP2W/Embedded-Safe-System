/*
 ***************************************************************************************
 Author: Chaya Punnasri, 100386454
 Date of creation: 25 April 2024
 Name: servo.c
 Description: The servo will be working by initialise the GPIO pin PB4 which supports
 the PWM and that pin is using TIM3 channel 1. The servo turns different angles
 ***************************************************************************************
 */

#include "stm32f7xx_hal.h" // HAL library is being used
#include "servo.h"

TIM_HandleTypeDef htim2;

void PA15_Init(){
    GPIO_InitTypeDef gpio;

    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable the clock for a base B

    gpio.Pin = GPIO_PIN_4; // pin 4
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_HIGH;
    gpio.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &gpio);
}


/**
Timer (TIM3) and Channel (CH1) initialization because that will be the function for the port PB4
**/
void TIM2_Init(void){
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_OC_InitTypeDef sConfigOC;

    //Timer configuration
    htim2.Instance = TIM3;
    htim2.Init.Prescaler = 32000; // set prescaler to 32000 and the signal 500Hz on the TIM3
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 9; // set period to 9 then it becomes 10, so the frequency will be 50 Hz cycle
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&htim2);

    //Set the timer in PWM mode
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
    HAL_TIM_PWM_Init(&htim2);

    //Channel configuration
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);

    PA15_Init();
}

// opened angle which is 90 degrees
void opened_angle(void){

    //Reset of all peripherals, initializes the Flash interface and the Systick
    HAL_Init();

    //Enable peripheral clock for TIM3
    __HAL_RCC_TIM3_CLK_ENABLE();

    //Initialize TIM3, CH1 and PB4
    TIM2_Init();

    //Start PWM on TIM3_CH1
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

    // change the CCR to 1 so the output will be high
    htim2.Instance->CCR1 = 1;

    // perform a delay for 1 second
    HAL_Delay(1000);
}

// closed angle which is 0 degree
int closed_angle(void){

    //Reset of all peripherals, initializes the Flash interface and the Systick
    HAL_Init();

    //Enable peripheral clock for TIM3
    __HAL_RCC_TIM3_CLK_ENABLE();

    //Initialize TIM3, CH1 and PB4
    TIM2_Init();

    //Start PWM on TIM3_CH1
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

    // change the CCR to 3 so the output will be low
    htim2.Instance->CCR1 = 3;

    //perform a delay for a second
    HAL_Delay(1000);
}