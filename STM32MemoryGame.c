/**
  *****************************************************************************
  *  STM32L4: Memory Game
  *
  *      This is a customizable-sequence memory game. The servo-motor will move
  *      in a predefined sequence between 3 locations and go back to starting
  *      position. After that, the sequence-entering can be started using the
  *      central button. The Red LED will light up and if the correct sequence
  *      on the joystick is entered, then It will turn green. The sequence entering
  *      can be restarted using the central button.
  *
  *
  *      @file      STM32MemoryGame.c
  *      @author    Dora Avun
  *      @date      2021-05-07
  *****************************************************************************
  */

#include "stm32l476xx.h" //including the stm32l4 library
int main(void)
{

	//Initialize Timer5 CH1
	Init_Timer_5();

//    Init_Timer4_CH2();

	//Initialize Servo
	Init_Servo();

	//Initialize Joystick
	Joystick_Init();

	//Initialize Green LED
	Green_LED_INIT();
	//Initialize Red LED
	Red_LED_INIT();

	//Light Up Green LED
	Turn_On_Green_LED();

	uint32_t mask_C = 1UL;    //to get only pin 0, 1 unsigned long.
	uint32_t mask_L = 1UL<<1; // pin 1
	uint32_t mask_R = 1UL<<2; // pin 2
	uint32_t mask_U = 1UL<<3; // pin 3
	uint32_t mask_D = 1UL<<5; // pin 5

	int steps = 0; //steps to progress the state machine
	int enable = 0;//to ensure we can start/restart the input sequence.

	Servo_180_Degrees();
	waitfor(1000);
	Servo_90_Degrees();
	waitfor(1000);
	Servo_0_Degrees();
	waitfor(1000);
	Servo_90_Degrees();
	waitfor(1000);
	Servo_180_Degrees();
	waitfor(1000);
	Servo_0_Degrees();
	waitfor(1000);
	Servo_90_Degrees();
	waitfor(1000);
	Servo_180_Degrees();
	waitfor(1000);

	while (1) //to keep the program running.
	{
		uint32_t input_C = (GPIOA->IDR & mask_C) == mask_C; //get value from IDR for pin 0
		uint32_t input_L = (GPIOA->IDR & mask_L) == mask_L; //get value from IDR for pin 1
		uint32_t input_R = (GPIOA->IDR & mask_R) == mask_R; //get value from IDR for pin 2
		uint32_t input_U = (GPIOA->IDR & mask_U) == mask_U; //get value from IDR for pin 3
		uint32_t input_D = (GPIOA->IDR & mask_D) == mask_D; //get value from IDR for pin 5


		switch(steps)
		{
		   case 0 :
			  if (input_C == 1)
			  {
				  enable = 1;
			  	  Turn_Off_Green_LED();
			  	  Turn_Off_Red_LED();
			  	  waitfor(500);
			  }

			  else if (enable == 1)
			  {
				  if (input_R == 1)
				  {
					  steps ++;
				  	  waitfor(500);
				  }
				  else if ((input_D == 1) || (input_U == 1) || (input_L == 1)) // if wrong button is pressed
				  {
					  steps = 0; // go back to first state
					  enable = 0;
					  Turn_On_Red_LED();
					  waitfor(500);
				  }
			  }
		   case 1 :
			  if (input_D == 1)
			  	  {
					  steps ++;
					  waitfor(500);
				  }
			  else if ((input_R == 1) || (input_U == 1) || (input_L == 1))// if wrong button is pressed
				  {
					  steps = 0;// go back to first state
					  enable = 0;
					  Turn_On_Red_LED();
					  waitfor(500);
				  }

		   case 2 :
			   if (input_R == 1)
			  	  {
					  steps ++;
					  waitfor(500);
				  }
			  else if ((input_D == 1) || (input_U == 1) || (input_L == 1))// if wrong button is pressed
				  {
					  steps = 0;// go back to first state
					  enable = 0;
					  Turn_On_Red_LED();
					  waitfor(500);
				  }
		   case 3 :
			   if (input_U == 1)
			  	  {
					  steps ++;
					  waitfor(500);
				  }
			  else if ((input_D == 1) || (input_R == 1) || (input_L == 1))// if wrong button is pressed
				  {
					  steps = 0;// go back to first state
					  enable = 0;
					  Turn_On_Red_LED();
					  waitfor(500);
				  }

		   case 4 :
			   if (input_D == 1)
			  	  {
					  steps ++;
					  waitfor(500);
				  }
			  else if ((input_R == 1) || (input_U == 1) || (input_L == 1))// if wrong button is pressed
				  {
					  steps = 0;// go back to first state
					  enable = 0;
					  Turn_On_Red_LED();
					  waitfor(500);
				  }

	       case 5 :
			   if (input_R == 1)
			  	  {
					  steps ++;
					  waitfor(500);
				  }
			  else if ((input_D == 1) || (input_U == 1) || (input_L == 1))// if wrong button is pressed
				  {
					  steps = 0;// go back to first state
					  enable = 0;
					  Turn_On_Red_LED();
					  waitfor(500);
				  }

		   case 6 :
			   if (input_C == 1)
				   {
					   enable = 0;
					   Turn_On_Green_LED();
					   Turn_Off_Red_LED();
					   waitfor(500);
				   }
			   else if ((input_D == 1) || (input_U == 1) || (input_C == 1))// if wrong button is pressed
				  {
					  steps = 0;// go back to first state
					  enable = 0;
					  Turn_On_Red_LED();
					  waitfor(500);
				  }
		}
	}
}

void waitfor(uint32_t ms) //Debouncer with 2 for loops for precise response
{
	uint32_t i,j;
	for(i = 0; i < ms; i++)
		for(j = 0; j < 255; j++);
}

void Turn_On_Green_LED(void) //turns on the LED
{
	GPIOE->ODR |= GPIO_ODR_ODR_8;  //pin 8 is Green LED
}

void Turn_Off_Green_LED(void) //turns on the LED
{
	GPIOE->ODR &= ~GPIO_ODR_ODR_8;  //pin 8 is Green LED
}

void Turn_On_Red_LED(void) //turns on the LED
{
	GPIOB->ODR |= GPIO_ODR_ODR_2;  //pin 8 is Green LED
}

void Turn_Off_Red_LED(void) //turns on the LED
{
	GPIOB->ODR &= ~GPIO_ODR_ODR_2;  //pin 8 is Green LED
}

void Green_LED_INIT() //Initialize the Green LED
{
	//GREEN LED IS AT PE8. So we need GPIOE and shift our values by 2*8.

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;//turn on clock for port E (0x48001000)

	GPIOE->MODER &= ~(3U<<16);  //clear moder value  (2*8)
	GPIOE->MODER |= 1U<<16; 	// set mode to output (01).

	GPIOE->PUPDR &= ~(3U<<16);  //no pull-up or down. (11) = reset
}

void Red_LED_INIT() //Initialize the Red LED PB2
{
	//RED LED IS AT PB2. So we need GPIOB and shift our values by 2*2.

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;//turn on clock for port B

	GPIOB->MODER &= ~(3U<<4);  //clear moder value  (2*2)
	GPIOB->MODER |= 1U<<4; 	// set mode to output (01).

	GPIOB->PUPDR &= ~(3U<<4);  //no pull-up or down. (11) = reset
}

void Joystick_Init()
{
	//JOYSTICK:  PA0(Center) , PA1(LEFT), PA5(DOWN), PA2(RIGHT), and PA3(UP)

	// Center Button:
    // GPIO Configuration
    RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOAEN;

    // Center Button:
    // GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
    GPIOA->MODER &= ~(3U);

    // GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
    // We do not need to pull down PA.0 because it has already been pulled down externally. Take a look at the schematics.
    GPIOA->PUPDR &= ~(3UL);


    // Left Button:
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3UL << (2*1));             // input state  (00)
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR &= ~(3UL << (2*1));                     //clear bit
	GPIOA->PUPDR |=  (2UL << (2*1));                    // pull down (10)

	// Right Button:
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3U << 2*2);               // input state  (00)
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR &= ~(3UL << 2*2);              //clear bit
	GPIOA->PUPDR |= (2UL << (2*2));            // pull down (10)

	// UP Button:
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3U << 2*3);              // input state  (00)
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR &= ~(3UL << 2*3);              //clear bit
	GPIOA->PUPDR |=  (2UL << (2*3));            // pull down (10)

	// Down Button:
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOAEN;
	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3U << 2*5);            // input state  (00)
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR &= ~(3UL << 2*5);                //clear bit
	GPIOA->PUPDR |=  (2UL << (2*5));             // pull down (10)

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


//	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //Initialize GPIOA clock

//	GPIOB->MODER &= ~(3U << 14); // Clear Bits
//	GPIOB->MODER |= 2U << 14; //Alternate Function
//
//	GPIOB->AFR[0] &= ~(0xF); // Clear AF
//	GPIOB->AFR[0] |= 2U ;     // AF2 = TIM5_CH1
//
//	GPIOB->OSPEEDR &= ~3U << 14; //Speed Mask
//	GPIOB->OSPEEDR |= 3U << 14;  //Very High Speed
//
//	GPIOB->PUPDR &= ~3U << 14;   // NO PUPD (00, reset)
}

void Init_Timer_5() //CH1
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;  // Enable timer Clock

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

//void Init_Timer4_CH2(){
//    //Enable the clock
//    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
//    //Counting direction: Up counting (0...ARR)
//    TIM4->CR1 &= ~TIM_CR1_DIR;
//    //Desired base frequency: 100 Hz
//        //Default System Clock: 4 MHz
//        //PSC: 39
//    TIM4->PSC = 39;
//        //ARR: ???   999 = 4 M/(1+39) -1
//    TIM4->ARR = 1999;
//    //Set output compare mode for ch2: PWM mode 1
//        //Clear current value of output compare mode for ch2
//    TIM4->CCMR1 &= ~TIM_CCMR1_OC2M;
//        // Set output compare mode for ch1: 0110, for PWM Mode 1
//    TIM4->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
//    //Enable pre-load
//    TIM4->CCMR1 |= TIM_CCMR1_OC2PE;
//    //Output polarity: 0, active high, 1, active low
//    TIM4->CCER &= ~TIM_CCER_CC2P;
//    //Enable output for ch2
//    TIM4->CCER |= TIM_CCER_CC2E;
//    //Main output enable (MOE): 1, Enable
//    TIM4->BDTR |= TIM_BDTR_MOE;
//    //Output Compare Register for channel 2 (CCR1): 500
//    TIM4->CCR2 = 1000;
//    //Enable TIM1 counter
//    TIM4->CR1 |= TIM_CR1_CEN;
//}


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





