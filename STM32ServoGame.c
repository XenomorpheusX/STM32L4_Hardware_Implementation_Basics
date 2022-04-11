/**
  *****************************************************************************
  *  FOR STM32L4
  *
  *      This program initializes ports PE8 (Green LED) and PA0 (Central joystick
  *      button) and a servo motor at PA0. I am aware that joystick and servo are
  *      initialized at the same pin therefore there are issues. However, servo and LED
  *      Functionality should be valid.
  *
  *
  *      @file      STM32ServoGame.c
  *      @author    Dora Avun
  *      @date      2021-04-23
  *****************************************************************************
  */

#include "stm32l476xx.h" //including the stm32l4 library

int ledStrength = 0;

int main(void)
{
	//Initialize Timer1 CH1N
	Init_Timer_1();

	//Initialize Timer5 CH1
	Init_Timer_5();

	//Initialize Servo
	Init_Servo();

	Servo_Action(); //Move servo to various angles

	//GREEN LED IS AT PE8. So we need GPIOE and shift our values by 2*8.

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;//turn on clock for port E (0x48001000)

	GPIOE->MODER &= ~(3U<<16);  //clear moder value  (2*8)
	GPIOE->MODER |= 2U<<16; 	// Alternate Function at '10' = 2.

	GPIOE->AFR[1] |= 1U;        //AF1 for timer 1 and channel 1.

	//Set Green LED Speed to high.
	GPIOE->OSPEEDR &= ~(3U<<16); //clear shift at 2*8
	GPIOE->OSPEEDR |= 3U<<16;    // high speed mode (11)

	GPIOE->PUPDR &= ~(3U<<16);  //no pull-up or down. (11) = reset


	//	JOYSTICK:  PA0(Center) , PA1(LEFT), PA5(DOWN), PA2(RIGHT), and PA3(UP)

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;//turn on clock for port A

	GPIOA->MODER &= ~3U;     //clear moder value (11) = reset
	GPIOA->MODER |= 1U<<2*1; // output(01)

	GPIOA->PUPDR &= ~3U; //no pull-up or down. (11) = reset

	uint32_t mask_C = 1U; //to get only pin 0, 1 unsigned long.


	int x = 1;
	while(1) // loop infinitely to dim and brighten the led
	{

		uint32_t input_C = (GPIOA->IDR & mask_C) == mask_C; //get value from IDR for pin 0
		// On/Off with toggle
		if (input_C == 1)
			{
				Servo_Action(); //Servo Action has the wait statements necessary in it.
		    }

		else if((ledStrength + x)  >= 999 || (ledStrength + x) <= 0) //keep brightness in range
		{
			x = -x; //invert value to manipulate ledStrength
		}
		ledStrength += x;
		TIM1 -> CCR1 = ledStrength;

		waitfor(1); //wait for 1 ms between brightness intervals

	}


}
void waitfor(uint32_t ms) //Debouncer with 2 for loops for precise response
{
	uint32_t i,j;
	for(i = 0; i < ms; i++)
		for(j = 0; j < 255; j++);
}

void Init_Servo()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //Initialize GPIOA clock

	GPIOA->MODER &= ~(3U); // Clear Bits
	GPIOA->MODER |= 2U; //Alternate Function

	GPIOA->AFR[0] &= ~(0xF); // Clear AF
	GPIOA->AFR[0] |= 2U;     // AF2 = TIM5_CH1

	GPIOA->OSPEEDR &= ~3U; //Speed Mask
	GPIOA->OSPEEDR |= 3U;  //Very High Speed

	GPIOA->PUPDR &= ~3U;   // NO PUPD (00, reset)
}

void Init_Timer_1() //CH1N
{
	RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN; //Enable timer 1

	TIM1 -> CR1 &= ~TIM_CR1_DIR; //direction is 0, which is counting up

	TIM1 -> PSC = 39; //prescaler ((((ClockSpeed) / ((period) / (1 / frequency))) + 0.5) - 1)

	TIM1 -> ARR = 999; //period (1000-1)

	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M; //Clear current value of output
	//Set output compare mode for ch1: 0110, for PWM mode 1
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; //01 | 10

	TIM1->CCMR1 |= TIM_CCMR1_OC1PE; //Enable pre-load

	TIM1->CCER &= ~TIM_CCER_CC1NP; //Output polarity: 0

	TIM1->CCER |= TIM_CCER_CC1NE; //Enable output for ch1N

	TIM1->BDTR |= TIM_BDTR_MOE; //Main output enable (MOE) = 1

	//Output Compare Register for channel 1 (CCR1): 500
	TIM1->CCMR1 = 500;
	//Enable TIM1 counter
	TIM1->CR1 |= TIM_CR1_CEN;

}


void Init_Timer_5() //CH1
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;  // Enable timer Clock
	//counting direction: up

	TIM5 -> PSC = 39;

	TIM5 -> ARR = 1999; //period (2000 - 1)

	TIM5->CCMR1 &= ~TIM_CCMR1_OC1M; //Clear current value of output
	//Set output compare mode for ch1: 0110, for PWM mode 1
	TIM5->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; //01 | 10

	TIM5->CCMR1 |= TIM_CCMR1_OC1PE; //Enable pre-load

	TIM5->CCMR1 |= TIM_CCER_CC1NP; //Output polarity: 0

	TIM5->CCER |= TIM_CCER_CC1E; //Enable output for ch1

	TIM5->BDTR |= TIM_BDTR_MOE; //Main output enable (MOE) = 1

	//Output Compare Register for channel 1
	TIM5->CCR1 = 1000;
	//Enable TIM1 counter
	TIM5->CR1 |= TIM_CR1_CEN;

}


void Servo_0_Degrees() //0 degrees
{
	TIM5->CCR1 = 40;
}

void Servo_90_Degrees() //90 degrees
{
	TIM5->CCR1 = 137;
}

void Servo_180_Degrees() //180 degrees
{
	TIM5->CCR1 = 235;
}

void Servo_Action()
{
	//Manipulate Servo Functionality
	Servo_90_Degrees();

	waitfor(5000);

	Servo_0_Degrees();

	waitfor(5000);

	Servo_180_Degrees();

	waitfor(5000);

}

