/*
 ***************************************************************************************
 Author: Chaya Punnasri, 100386454
 Date of creation: 5 May 2024
 Name: photoresistor.c
 Description: Initialise the system clock configuration for the RCC and the PWR which
 will be used in an analogue signal generation, after that the signal is converted into
 the digital signal
 ***************************************************************************************
 */

#include <stdio.h>
#include "stm32f7xx_hal.h"
#include "GLCD_Config.h" // include the GLCD_config.h
#include "Board_GLCD.h" // include the Board_GLCD.h
#include "Board_Touch.h" // include Board_Touch.h
#include "photoresistor.h" // include the photoresistor.h
#define wait_delay HAL_Delay
#include <math.h> // include the use of a math library

// calling the function from GLCD_Touch.h
extern GLCD_FONT GLCD_Font_16x24;


/*
* System Clock Configuration
*/

void SystemClock_Config_Photoresistor(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();
	/* The voltage scaling allows optimizing the power
	consumption when the device is clocked below the
	maximum system frequency. */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/* Enable HSE Oscillator and activate PLL
	with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	/* Select PLL as system clock source and configure
	the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | 
	RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

// initialise the use of analogue to digital convertor
ADC_HandleTypeDef hadc;

void MX_ADC_Init(void)
{
    ADC_ChannelConfTypeDef sConfig;
 
	/* Enable ADC CLOCK For every ADC */
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_ADC2_CLK_ENABLE();
	__HAL_RCC_ADC3_CLK_ENABLE();

	/* Configure the global features of the ADC (Clock, Resolution, Data Alignment and number
	of conversion) */		
	hadc.Instance = ADC3; // choosing ADC3 as we are using the port A pin 0
	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc.Init.Resolution = ADC_RESOLUTION_10B;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.NbrOfConversion = 1;
    hadc.Init.ScanConvMode = ENABLE;
	hadc.Init.ContinuousConvMode = ENABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	HAL_ADC_Init(&hadc);
	//configure channal
	sConfig.Rank = 1;
	sConfig.Channel = ADC_CHANNEL_0; // choosing to use a channel 0 because it is compatible for port A pin 0
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
}

// initialise a GPIO for analog
GPIO_InitTypeDef GPIO_InitStruct;
void MX_GPIO_Init(void){
    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable clock for port A
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG; // configure to analog input mode
    GPIO_InitStruct.Pin = GPIO_PIN_0; // pin 0
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
	


// control LCD brightness
void ctrl_brightness (float adc_read){
	float V_pwr=5.0; //configure the V_pwr
    float V_sense= (adc_read * V_pwr)/1024; //calculate the V_sense which 1024 is the reading range
    float brightness=V_sense/V_pwr; // calculate the ratio of brightness
  
    if(brightness > 0.2)	{
        // toggle the dark mode
			GLCD_SetBackgroundColor(GLCD_COLOR_BLACK);
			GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
			GLCD_ClearScreen();
		}
		else {
            // toggle the light mode
			GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
			GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
			GLCD_ClearScreen();
		
		}
}

// main loop for a photoresistor
int photoresistor_main(void){

    uint16_t adc_read; // declare a variable for the ADC reading
    adc_read = HAL_ADC_GetValue(&hadc);
	SystemClock_Config_Photoresistor(); //Config Clocks
	MX_GPIO_Init();
	MX_ADC_Init();
		// read ADC value 
		ctrl_brightness(adc_read);
		wait_delay(1);
}
