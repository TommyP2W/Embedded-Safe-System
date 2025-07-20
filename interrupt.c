/*
	 ################################
	 Principle Author:

	 Tommy Peach, 100358179

	 File name:

	 interrupt.c

	 Description:

     interrupt.c handles delaying the system when too many incorrect attempts are entered. It also handles the interrupt caused by the shock sensor.

     ################################
 */

#include <stdio.h>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
#include "Board_GLCD.h"

#include "buzzer.h"
char countdown[1]; // buffer for showing seconds for delay

// Function for delaying and printing the number of seconds until countdown ends
void delay_seconds(int seconds)
{	
		int i = 0;
		for (i = seconds; i >= 0; i--){         // This for loop decrements until the integer i reaches 0
			sprintf(countdown, "Delay: %d", i); // sprintf puts the text "Delay: %d" into the countdown buffer. %d is the identifier for what data type is meant to take its place.
			GLCD_DrawString(168,9*24, countdown); // Printing the countdown on the screen
			HAL_Delay(1000);                    // Delays the program for 1 second, 1000 because it is miliseconds (1000 miliseconds = 1 second)
		}
}
// Function for checking the interrupt, the function checks for the interrupt and if there is one the callback function will execute
void interrupt_check(void)
{
        // This function will check for the shock sensor interrupt, the gpio pin is gpio_pin_6 as this is the interrupt handler vector I configured in shock.c
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
}
// Callback function for the interrupt, this function writes the pin for the buzzer, uses a for loop to delay the resetting of the buzzer pin
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

					HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET); // Setting the buzzer pin

					for (i = 0; i < 10000000; i++){                     // for loop to delay the reset of the buzzer, causing it to continue sounding
					}
					
					HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET);   // resetting the buzzer
}

// Function to delay the system, based on the amount of attempts have accumulated in GLCDTOUCH.c
void timed_delay(int wrong_attempts){

	// If two wrong attempts
    if (wrong_attempts == 2)
    {

        GLCD_DrawString(72, 100, "Incorrect, try again.");  // User feedback

        delay_seconds(5);  // Calls the delay_seconds function, wait for 5 seconds
			
    }
    else if (wrong_attempts == 3)
    {
        // Wait for 10 seconds
				GLCD_DrawString(72, 100, "Incorrect, try again.");

				delay_seconds(10);
			
    }
    else if (wrong_attempts >= 4)
    {
        // Wait for 15 seconds
				GLCD_DrawString(72, 100, "Incorrect, try again.");


				delay_seconds(15);


    }
}

