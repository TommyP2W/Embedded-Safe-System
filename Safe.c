/*
	 ################################
	 Principle Author:

	 Tommy Peach, 100358179

	 File name:

	 safe.c

	 Description:

     safe.c is responsible for initialising all of the components that are not already initialised and also checks for touch screen presses

     ################################
 */
#include "GLCDTOUCH.h"
#include "buzzer.h"
#include "keypad.h"
#include "photoresistor.h"
 #include "Board_Touch.h"
 #include "shock.h"
 #include "screens.h"

  int main(void){

				initializeMembranePins();
				HAL_Init(); //Init Hardware Abstraction Layer

				SystemClock_Config();
				Touch_Initialize();
				GLCD_Initialize();
				TIM2_Init();
				PA15_Init();
				initialise_input();
				initialise_led();
				MX_GPIO_Init();
				MX_ADC_Init();
				SystemClock_Config_Photoresistor();	
		    	__HAL_RCC_GPIOC_CLK_ENABLE();
			    __USART6_CLK_ENABLE();
				init_shock();
				init_buzzer();
                changeScreen(0); // go to set pin screen
	
			for(;;){
                Touch_GetState(&tsc_state);
                    if(tsc_state.pressed){
                        tsc_state.pressed = 0;
                        CheckCoords(tsc_state.x, tsc_state.y);
                        continue;
                    }
                }
}