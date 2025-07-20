/*
	 ################################
	 Principle Author:

	 Tommy Peach, 100358179

	 File name:

	 screens.c

	 Description:

     Screens.c handles the screens, specifically drawing the elements on each screen, as well as handling the transition between screens through CheckCoords.
     From the stm website, I was able to determine that the resolution of the screen was 480x272 pixels, so this is the number the calculations were based on for centering the text.

     Calculations for centering text goes as follows:

     (16 pixels (size of the font width) * (number of letters)) / 2) - 240.
     This calculation takes the midpoint of the sentence/text and minuses half of the screen size to find the point where the text should start.

     ################################
 */
#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "GLCDTOUCH.h"
int ScreenState = 0; // Initialising ScreenState, 0 represents the first screen which is the set pin screen

/* CheckCoords checks the coordinates that a user has pressed on the screen, and passes these values into if statements.
 * The conditions inside of the if statements make sure that the correct coordinates are pressed for the back buttons on each screen.
 * It also checks for the "Unlock" and "Reset Pin" buttons on the home screen.
 * Each if statement includes the ScreenState for which screens these conditions should apply for. This avoids changing the screen for unintended screens.
 */

void CheckCoords(int x, int y){
    // If statement to transition back to the home screen
    if ((x > 0 && x < 70) && (y > 0 && y < 25) && (ScreenState == 2 || ScreenState == 3
        || ScreenState == 4 || ScreenState==5 )){
        changeScreen(1);
    }
    // If statement for transition to the unlock screen
    if ((x < 288 && x > 192) && (y < 140 && y > 116) && ScreenState == 1){
        changeScreen(2);
    }
    // If statement for transition to the reset pin screen
    if ((x < 312 && x > 168) && (y < 92 && y > 68) && ScreenState == 1){
        changeScreen(3);
    }
    // Transition for the set pin confirmation screen, if the user wants to change the pin on the first time setting
    if ((x > 0 && x < 70) && (y > 0 && y < 25) && (ScreenState == 6)){
        changeScreen(0);
    }
}


void unlockScreen(){
			int x = 37;
			int i = 0;
			GLCD_DrawString(192, 0*24, "UNLOCK");
			GLCD_DrawString(104 , 3*24, "Input Combination");
			GLCD_DrawString(0*24, 0*24, "Back");
			GLCD_DrawRectangle(0*24, 0*24, 65, 25); // Drawing back button box
	        // Drawing input boxes
			for (i = 0; i <= 7; i++) {
                x += 42;
                GLCD_DrawString(x, 136, "_");
            }
	}

void resetScreen(){
    int x = 37;
    int i = 0;
    GLCD_DrawString(168, 0*24, "Reset Pin");
    GLCD_DrawString(40 , 3*24, "Input current combination");
    GLCD_DrawString(0*24, 0*24, "Back");
    GLCD_DrawRectangle(0*24, 0*24, 65, 25); // Drawing back button box

    // Drawing input boxes
    for (i = 0; i <= 7; i++){
        x += 42;
        GLCD_DrawString(x, 136, "_");
    }
}

void hscreen(){
	GLCD_DrawString(192, 116, "Unlock");
	GLCD_DrawString(168, 68, "Reset Pin");
}


void lockedScreen(){
		GLCD_DrawString(72, 4*24, "Please close door to");
		GLCD_DrawString(40, 5*24, "automatically lock system");
}

void confirmScreen(){
	int x = 37;
	int i = 0;
	GLCD_DrawString(152, 0*24, "Confirm Pin");
	GLCD_DrawString(88, 3*24, "Confirm combination");
	GLCD_DrawString(0*24, 0*24, "Back");
	GLCD_DrawRectangle(0*24, 0*24, 65, 25);
    // Drawing input boxes
	for (i = 0; i <= 7; i++){
		x += 42;	
		GLCD_DrawString(x, 136, "_");
	}
}
void resetSetScreen(){
	int x = 37;
	int i = 0;
	GLCD_DrawString(168, 0*24, "Reset Pin");
	GLCD_DrawString(72, 3*24, "Input new combination");
	GLCD_DrawString(0*24, 0*24, "Back");
	GLCD_DrawRectangle(0*24, 0*24, 65, 25);

    // Drawing input boxes
	for (i = 0; i <= 7; i++){
		x += 42;	
		GLCD_DrawString(x, 136, "_");
	}
}
void setPinScreen(){
	int x = 37;
	int i = 0;
	GLCD_DrawString(184, 0*24, "Set pin");
	GLCD_DrawString(104, 3*24, "Input combination");
    // Drawing input boxes
	for (i = 0; i <= 7; i++){
		x += 42;	
		GLCD_DrawString(x, 136, "_");
	}
	ScreenState = 0;
}
void setPinConfirmScreen(){
	int x = 37;
	int i = 0;
	GLCD_DrawString(184, 0*24, "Set pin");
	GLCD_DrawString(104, 3*24, "Confirm combination");
	GLCD_DrawString(0*24, 0*24, "Back");
	GLCD_DrawRectangle(0*24, 0*24, 65, 25);
    // Drawing input boxes
	for (i = 0; i <= 7; i++){
		x += 42;	
		GLCD_DrawString(x, 136, "_");
	}
}