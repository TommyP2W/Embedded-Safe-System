/*
 ****************************************************************************************************
 Author: Chaya Punnasri, 100386454
 Date of creation: 25 April 2024
 Name: servo.h
 Description: It provides function prototypes to be used in other c file for a further implementation
 ****************************************************************************************************
 */

void PA15_Init();
void TIM2_Init(void);
void opened_angle(void);
void closed_angle(void);

// link to the function in GLCD_touch.c
void SystemClock_Config(void);