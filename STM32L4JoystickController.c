/**
  *****************************************************************************
  *  FOR STM32L4
  *
  *      This program initializes ports PE8 (Green LED) and PA0 (Central joystick
  *      button) and checks if central joystick is activated at any time during
  *      the runtime. With the help of a debouncer, the input is used to manipulate
  *      the green LED.
  *
  *
  *      @file      STM32L4JoystickController.c
  *      @author    Dora Avun
  *      @date      2021-03-29
  *****************************************************************************
  */

#include "stm32l476xx.h" //including the stm32l4 library
int main(void)
{
	//GREEN LED IS AT PE8. So we need GPIOE and shift our values by 2*8.

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;//turn on clock for port E (0x48001000)

	GPIOE->MODER &= ~(3U<<16);  //clear moder value  (2*8)
	GPIOE->MODER |= 1U<<16; 	// set mode to output (01).

	GPIOE->PUPDR &= ~(3U<<16);  //no pull-up or down. (11) = reset

	//	JOYSTICK:  PA0(Center) , PA1(LEFT), PA5(DOWN), PA2(RIGHT), and PA3(UP)

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;//turn on clock for port A

	GPIOA->MODER &= ~3UL;     //clear moder value (11) = reset
	GPIOA->MODER |= 1UL<<2*1; // output(01)

	GPIOA->PUPDR &= ~3UL; //no pull-up or down. (11) = reset

	uint32_t mask_C = 1UL; //to get only pin 0, 1 unsigned long.

	while (1) //to keep the program running. (Shouldn't be considered an infinite loop)
	{
		uint32_t input_C = (GPIOA->IDR & mask_C) == mask_C; //get value from IDR for pin 0
		// On/Off with toggle
		if (input_C == 1)
		{
			Toggle_LED();
			waitfor(500);
		}

//      On/Off with button press.
//		if (input_C == 1)
//		{
//			Turn_On_LED();
//		}
//		else
//			Turn_Off_LED();
	}
}

void waitfor(uint32_t ms) //Debouncer with 2 for loops for precise response
{
	uint32_t i,j;
	for(i = 0; i < ms; i++)
		for(j = 0; j < 255; j++);
}

void Turn_On_LED(void) //turns on the LED
{
	GPIOE->ODR |= GPIO_ODR_ODR_8;  //pin 8 is Green LED
}

void Turn_Off_LED(void) //turns off the LED
{
	GPIOE->ODR &= ~GPIO_ODR_ODR_8; //pin 8 is Green LED
}
void Toggle_LED(void) //toggles the LED
{
	GPIOE->ODR ^= GPIO_ODR_ODR_8;  //pin 8 is Green LED
}
