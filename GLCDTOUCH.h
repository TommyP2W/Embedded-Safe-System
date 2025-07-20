
/*
	 ################################
	 Principle Author:

	 Tommy Peach, 100358179

	 File name:

	 GLCDTOUCH.h

	 Description:

     GLCDTOUCH.h defines functions that will be used in other source files

     ################################
 */
 #include "stm32f7xx_hal.h"
    #include "GLCD_Config.h"
    #include "Board_GLCD.h"
    #include "Board_Touch.h"
		#include <string.h>
		#include "buzzer.h"
		#include "keypad.h"
		#include "led_main.h"
		#include "servo.h"
		#include "tracking.h"
		#include "interrupt.h"
    #define wait_delay HAL_Delay

        extern GLCD_FONT GLCD_Font_16x24;
		extern UART_HandleTypeDef Bluetooth_UARTHandle;
		extern uint8_t RxBuffer;
		extern int pin[8];
		extern TOUCH_STATE tsc_state;
		extern int ScreenState;
		extern int attempts;


void changeScreen();
_Bool verify();
void delay();
void setPin();
_Bool unlock();
void init_uart(void);
void SystemClock_Config(void);
_Bool resetPin();
_Bool confirm(int attempt[]);
void setPin();
