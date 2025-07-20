# Embedded-Safe-System
Security Safe Embedded System built for embedded systems module at university in second year. Partnered and collaborated with ([theprcy](https://github.com/theprcy)).

## Tools used for development
### Hardware
- STM32F7 Discovery Board
- GLCD Touch screen
- HC-05 Bluetooth Module
- Passive buzzer
- Tracking sensor
- Servo
- LED
- Photoresistor
- Shock sensor

### Software
- Board support: STM32F746G Discovery (v1.0.0)
- Keil uVision5 MDK-Lite (v5.4.0.0)
- ARM::CMSIS CORE (v5.6.0)
- Keil::Device:Startup
- Keil::Device:STM32CubeFramework:Classic
- Keil::Device:STM32Cube HAL: ADC, Common, Cortex, DMA, GPIO, I2C PWR, RCC, TIM, USART


## Purpose of the system
A prototype of a security safe with multiple sensors and security features to enhance protection over physical items stored in business and residential buildings. 

### The system provides:
- Common functionality seen in most modern security safes
- Additional multi-factor approach, using confirmation from bluetooth connected devices in order to reset pin and open safe.
- System interrupts which detect shock applied to the safe constantly in real time which once reaches over a specified threshold will sound an auditory alarm.
- System pin delays for multiple incorrect attempted unlocks of the system.
- Led light indication as to the status of the system (locked/unlocked)
- Quality of life light mode / dark mode.
