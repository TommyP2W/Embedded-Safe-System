
/*
	 ################################
	 Principle Author:

	 Tommy Peach, 100358179

	 File name:

     screens.h

	 Description:

	 screens.h defines functions to be used in other source files.
   ################################
 */
    #include "GLCD_Config.h"
    #include "Board_GLCD.h"
		#include "String.h"
		extern int ScreenState;
		
		void unlockScreen();
		void resetScreen();
		void homeScreen();
		void hscreen();
		void lockedScreen();
		void confirmScreen();
		void resetSetScreen();
		void setPinScreen();
		void setPinConfirmScreen();
		int CheckCoords(int x, int y);
