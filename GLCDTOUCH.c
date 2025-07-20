/*
	 ################################
	 Principle Author:

	 Tommy Peach, 100358179

	 File name:

	 GLCDTOUCH.C

	 Description:

	 GLCDTOUCH.C handles logic surrounding unlocking, resetting and setting the pin for the system. It also handles transitions between screens defined in Screens.c.
	 GLCDTOUCH.C also handles 2FA for the UART/USART bluetooth communication.
   ################################
 */
        #include "stm32f7xx_hal.h"
        #include "GLCD_Config.h"
        #include "Board_GLCD.h"
        #include "Board_Touch.h"
		#include <string.h>
		#include "buzzer.h"
		#include "keypad.h"
		#include "screens.h"
		#include "led_main.h"
		#include "servo.h"
		#include "tracking.h"
		#include "led_main.h"
		#include "interrupt.h"
		#define wait_delay HAL_Delay
        extern GLCD_FONT GLCD_Font_16x24; // Externally including font for GLCD text
		int pin[8] = {1,2,3,4,5,6,7,8}; // Pin array that holds up to 8 integers, used for multiple functions. Preset for testing purposes.
		char status[6] = "Locked";        // Current status for the system, set to locked and is displayed on the home screen
		int attempts = 0;                 // Records the amount of attempts the user has made to enter the currently set pin
		extern int ScreenState;           // Externally included variable from Screens.c, this variable is used to check the current screen the user is interacting with
		TOUCH_STATE tsc_state;            // Stores the current TOUCH_STATE
		UART_HandleTypeDef Bluetooth_UARTHandle;  // Handle for enabling UART bluetooth communication with HC-05
		uint8_t RxBuffer;                 // Buffer for holding received data from bluetooth communication


#ifdef __RTX
extern uint32_t os_time;
uint32_t HAL_GetTick(void) {
	return os_time;
}
#endif
// Function for initialising UART, it initialises RX and TX ports
void init_uart(void){  // Initialising UART capabilities
	GPIO_InitTypeDef GPIO_InitStructure;

    // Initialising RX port for D0
	GPIO_InitStructure.Pin = GPIO_PIN_7;            // Pin 7
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;      // Push Pull config
	GPIO_InitStructure.Alternate = GPIO_AF8_USART6; // Initialising USART6 as it is mapped to pin D0 on board
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);      // Initialising the pin on port C
    // Initialising TX port for D1
	GPIO_InitStructure.Pin = GPIO_PIN_6;            // PIN 6
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;      // Push Pull config
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);      // Initialising the pin on port C
    // Configuring the bluetooth handle
	Bluetooth_UARTHandle.Instance        = USART6;
	Bluetooth_UARTHandle.Init.BaudRate   = 9600;    // Setting BaudRate to 9600, this is because the chip I bought online recommends/requires 9600
	Bluetooth_UARTHandle.Init.WordLength = UART_WORDLENGTH_8B;
	Bluetooth_UARTHandle.Init.StopBits   = UART_STOPBITS_1;
	Bluetooth_UARTHandle.Init.Parity     = UART_PARITY_NONE;
	Bluetooth_UARTHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	Bluetooth_UARTHandle.Init.Mode       = UART_MODE_TX_RX;
	HAL_UART_Init(&Bluetooth_UARTHandle);           // Initialising the handle
}


void SystemClock_Config(void) {  // Configuring System Clock
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

// Function to delay the system for 1 second.
void delay(){
    HAL_Delay(1000);
}

// 2FA verification method, allows user to respond on compatible bluetooth device to confirm unlock function
bool verify(){
    init_uart();  // Initialising uart
    RxBuffer = 0; // Resetting RxBuffer each time to avoid potential issues
    GLCD_DrawString(56, 100, "Please confirm on device.");

    HAL_UART_Transmit(&Bluetooth_UARTHandle, (uint8_t *)"Confirm", 7, HAL_MAX_DELAY);  // Prints "Confirm" on connected bluetooth terminal
    HAL_UART_Receive(&Bluetooth_UARTHandle, &RxBuffer, 1, HAL_MAX_DELAY);              //  Takes response from bluetooth terminal and stores in RxBuffer
		
			
    if (RxBuffer != 'y'){ // If the buffer is not equal to 'y'
        return false;     // Verification failed
    }
    // If verified
    buzz_on();  // buzzer on
    delay();    // prolong buzzing
    buzz_off(); // turn buzzer off
    return true;// return successful
			
}
// Unlock function, checks a temporary array filled with Membrane Keypad inputs and compares to the current stored pin
bool unlock(){
    int attempt[8];  // attempt array, this stores the attempt that the user will input on the membrane keypad
    int i = 0;       // Loop variable which is incremented
    int spacing = 37;// This is used to space the asteriks inline with the drawn _'s.
    while(i<8){
        int Membrane = getInput();	// Getting input from the membrane keypad
        if (ScreenState != 2){      // If its not the unlock screen, break. The reason being is because this function should not run on any other screens
            break;
        }

        Touch_GetState(&tsc_state); // Getting the touch state
        if(tsc_state.pressed){
            CheckCoords(tsc_state.x, tsc_state.y);		// If CheckCoords is true, the ScreenState will change and the function will break based on the above if conditional
            tsc_state.pressed = 0;                              // I'm resetting the touch state here, just to avoid potential errors further into the program
        }

        if(Membrane!=-1){		            // If the membrane detects a valid input
            spacing+=42;
            GLCD_DrawString(spacing, 150, "*");  	// Draw '*' for each input
            attempt[i] = Membrane; 	        // store the input into index i of temporary array attempt
            i++;                            // increment
            delay();                        // delay inputs using the delay function, prevents rapid inputs from one press of button
        }
    }
			
				
    for (i = 0; i < 8; i++){
        if (attempt[i] != pin[i]){          // Checking the array of inputs against current stored pin
            return false;                   // If one attempt is wrong, return false. Makes it more efficient as the worst case time complexity is (O(n)).
        }
    }
    if (verify() != true){                  // If the unlock is not verified return false
        return false;
    }
    return true;                            // If all loops continue, return true
					
}
/*
 * Confirm function, this function takes in a temporary attempt array from a previous function and checks for discrepancies between the two arrays.
 *
 */
bool confirm(int attempt[]){
    int temp_pin[8];    // Temporary pin array to store confirmation attempt
    int i = 0;          // Int that will be used to increment loops
    int spacing = 37;   // Used for spacing asteriks that will be printed each input

    while(i<8){
        int Membrane = getInput();  // Getting keypad input
        Touch_GetState(&tsc_state); // Getting the current touch state

        if(tsc_state.pressed){
            CheckCoords(tsc_state.x, tsc_state.y); // CheckCoords to check for back button press
            tsc_state.pressed = 0;                       // reset tsc_state to avoid potential errors
        }

        if (ScreenState != 5 && ScreenState != 6){     // If the screen is not on the confirmation screens, break
            break;
        }

        if(Membrane!=-1){                   // If valid Membrane Keypad input
            spacing+=42;
            GLCD_DrawString(spacing, 150, "*");
            temp_pin[i] = Membrane;         // store the current membrane input into temporary array
            i++;                            // increment i
            delay();                        // delay
        }
    }

    for (i =0; i < 8; i++){
        if (attempt[i] != temp_pin[i]){           // Checking the inputs against the array passed into the confirm function
            return false;                   // returns false if any input does not equal the array elements passed in the function
        }
    }
    return true;                            // If all loops finish, return true
}
	
bool resetSetPin(){
    int temp_pin[8];    // Storing a temporary input
    int i = 0;          // Int to increment loops
    int spacing = 37;   // Int used to apply spacing to asteriks inputted

    while(i<8){
        int Membrane = getInput();

        if (ScreenState != 4){  // If the user is not on the resetSetScreen
            return false;
        }
        Touch_GetState(&tsc_state);

        if(tsc_state.pressed){
            CheckCoords(tsc_state.x, tsc_state.y);
            tsc_state.pressed = 0;

        }
        if(Membrane!=-1){
            spacing+=42;
            GLCD_DrawString(spacing, 150, "*");
            temp_pin[i] = Membrane;
            i++;
            delay();
        }
    }

    GLCD_ClearScreen(); // Clearing the screen for the next screen
    ScreenState = 5;    // Setting the ScreenState to be the ScreenState of the confirm screen, essential for not immediately returning false
    confirmScreen();

    while (ScreenState == 5){	    // While user is on the confirm screen
        if (confirm(temp_pin)){     // If confirm returns ture
            for (i = 0; i < 8; i++){
            pin[i] = temp_pin[i];   // Store new pin into pin global variable
            }
            return true;            // return true
        } else {
            if (ScreenState != 5){  // If not on the confirm screen, return false
                return false;
            }
            GLCD_ClearScreen();     // Clear the screen
            confirmScreen();	    // Go back to the confirm screen, this will also call the confirm function again
            GLCD_DrawString(72, 100, "Incorrect, try again.");     // Giving the user feedback
        }
    }
}
			
bool setPin(){
    int temp_pin[8]; // Temporary pin to store input
    int i = 0;
    int spacing = 37;

    while(i<8){
        int Membrane = getInput();

        Touch_GetState(&tsc_state);

        if(tsc_state.pressed){
            CheckCoords(tsc_state.x, tsc_state.y);
            tsc_state.pressed = 0;
        }

        if(Membrane!=-1){
            spacing+=42;
            GLCD_DrawString(spacing, 150, "*");

            temp_pin[i] = Membrane; // storing the inputs into the temporary array
            i++;                    // Incrementing loop
            delay();
        }
    }

    GLCD_ClearScreen();             // Clearing the screen
    ScreenState = 6;                // Setting the screen to the confirm screen for setting a new pin
    setPinConfirmScreen();
    while (ScreenState == 6){       // avoids the function from carrying over to the next screen
        if (confirm(temp_pin)){
            for (i = 0; i < 8; i++){
                pin[i] = temp_pin[i];   // if confirmed, store in global pin array
            }
            return true;
        } else {
            if (ScreenState != 6){   // If not on correct screen, return false
                return false;
            }
            GLCD_ClearScreen();      // Clearing the screen, this gets rid of previous asteriks
            setPinConfirmScreen();   // Go to the same screen
            GLCD_DrawString(72, 100, "Incorrect, try again."); // User feedback
        }
    }
}

bool resetPin(){
    int attempt[8];
    int i = 0;
    int spacing = 37;

    while(i<8){
        int Membrane = getInput();
        if (ScreenState != 3){      // If not on the reset screen, return false
            break;
        }

        Touch_GetState(&tsc_state);
        if(tsc_state.pressed){
                CheckCoords(tsc_state.x, tsc_state.y);
                tsc_state.pressed = 0;
        }

        if(Membrane!=-1){
            spacing+=42;
            GLCD_DrawString(spacing, 150, "*");
            attempt[i] = Membrane;
            i++;
            delay();
        }
    }

    for (i = 0; i < 8; i++){
        if (attempt[i] != pin[i]){
            return false;
        }
    }

    return true;
}

// This function will reset the attempts global variable attempts and return the value. (Returning the value seemed to work whereas if I did not the return the value it did not work.)
int attemptsReset(){
    attempts = 0;
    return attempts;
}
// Main function for changing between screens and calling the relevant functions for those screens
void changeScreen(int a){

    GLCD_SetFont(&GLCD_Font_16x24); // Setting the font for all the text
    photoresistor_main();           // Calls function from photoresistor.c. This function controls dark mode vs light mode
    GLCD_ClearScreen();             // Clearing the screen
    delay();                        // Delaying, reduces screen flickering

    switch(a){
        case 0:                     // a = 0
            ScreenState = 0;        // ScreenState = 0, which is representative of the setPin screen
            setPinScreen();

            while (ScreenState == 0 || ScreenState == 6){  // While at the setPinScreen or the setPinConfirm screen
                if (setPin()){                             // Calling the setPin function
                    changeScreen(1);                    // if it returns true, change to homescreen
                }
            }
            break;
        case 1:
            ScreenState = 1;         // Represents home screen
            hscreen();               // home screen
            GLCD_DrawString(192, 0*24, "Locked"); // Printing status for home screen
            break;

        case 2:
            ScreenState = 2;        // Represents unlock screen
            unlockScreen();
            attemptsReset();        // Resetting the attempts at the start, this prevents attempts carrying over from other screens or previous attempts

            while (ScreenState == 2){
                GLCD_ClearScreen();
                unlockScreen();     // This is essential, as it will reset asteriks from previous attempts
                attempts++;
                timed_delay(attempts); // timed_delay is defined in interrupt.c

                if (unlock()){      // if the unlock function returns true
                    changeScreen(4); // change to unlocked screen
                }
            }
            break;
        case 3:
            ScreenState = 3;    // Represents reset Screen
            attemptsReset();    // Resetting attempts to avoid carry over

            resetScreen();

            while (ScreenState == 3 || ScreenState == 4 || ScreenState==5){ //  While on resetScreen or any of the confirm screens associated with resetting pin
                GLCD_ClearScreen();
                resetScreen();
                attempts++;
                timed_delay(attempts);

                if (resetPin()){
                    GLCD_ClearScreen();
                    ScreenState=4;          // represents screen to set after resetting

                    resetSetScreen();       // screen to set pin after resetting
                    if(resetSetPin()) {
                        changeScreen(1); // return to home screen
                    }
                }

            }

            break;

        case 4:
            ScreenState = 7;    // Represents unlocked screen
            opened_angle();     // calls opened_angle() from servo.c, which turns servo by 90 degrees
            HAL_Delay(3000);    // delay, allows time for user to open the door
            while (HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_3) == GPIO_PIN_SET){   // This turns out to be the opposite, whenever the tracking sensor does not detect something, the pin for the sensor is set
                turnOnGreen();  // from led_main.c, turns led green
                lockedScreen();
                HAL_Delay(3000); // delay for 3 seconds
                if (HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_3) != GPIO_PIN_SET){   // If the tracking sensor detects something, break out of while loop
                    break;
                }
            }
            closed_angle();     // servo.c function, turns servo 90 degrees opposite of what it was just turned
            turnOnRed();        // turn on red led
            changeScreen(1); // go to home screen
            break;

        default:
            break;
    }
}

