#include <Encoders.h>
#include <ROS_USB.h>


/* Variables ---------------------------------------------------------*/
volatile int32_t rightEncoderTicks, leftEncoderTicks;
struct encoders_ticks{
    int right;
    int left;
};

void EXTI_configure(void)
{
	RCC->APB2ENR|= 1;//eneables syscfg clock
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PB;//configure syscfg register that enables multiplexed PB8 to enter EXTI8
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PB;//configure syscfg register that enables multiplexed PB9 to enter EXTI9
	EXTI->IMR |= (1<<8|1<<9);//interrupt mask register of exti8 and exti9 set as unmask so that it cant be ignore by processor
	EXTI->RTSR |= (1<<8|1<<9);//set the interruption as rising trigger for EXTI8 and EXTI9
	NVIC->ISER[0] |= 1<<23;//enable nvic set register from EXTI 5 to EXTI 9

}

void EXTI9_5_IRQHandler (void)//interrupt for right & left encoders
{
	if (EXTI->PR &(1<<8))//interrupt for right encoder
	{
		EXTI->PR|=(1<<8);	//clear the pending bit of the interrupt
		if(GPIOF->IDR&(1<<9))//reads the value in channel B of the encoder connected to PF9 
		rightEncoderTicks=rightEncoderTicks+1;//B leads A (moving positive way)
		else
		rightEncoderTicks=rightEncoderTicks-1;//A leads B (moving negative way)
	}
	
	if (EXTI->PR &(1<<9))//interrupt for left encoder
	{
		EXTI->PR|=(1<<9);	//clear the pending bit of the interrupt
		if(GPIOF->IDR&(1<<10))//reads the value in channel B of the encoder connected to PF10 
		leftEncoderTicks=leftEncoderTicks+1;//B leads A (moving positive way)
		else
		leftEncoderTicks=leftEncoderTicks-1;//A leads B (moving negative way)
	}
}


	void PB8_PB9_PF9_PF10configure(void)//GPIOs for EXTI 8 and 9 as well as digital input PF9 and PF10 for channel B on encoders
{
	//Input mode implicit if MODER configuration isn't configure
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;//enable B port clock
	RCC->AHBENR |= RCC_AHBENR_GPIOFEN;//enable F port clock
	GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR8_0 | GPIO_PUPDR_PUPDR9_0); //PB8 & PB9 as Pull Up
	GPIOF->PUPDR |= (GPIO_PUPDR_PUPDR9_0 | GPIO_PUPDR_PUPDR10_0); //PF9 & PF10 as Pull Up
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8_0 | GPIO_OSPEEDER_OSPEEDR8_1 | GPIO_OSPEEDER_OSPEEDR9_0 | GPIO_OSPEEDER_OSPEEDR9_1); //PB8 & PB9 high speed 50MHz
  GPIOF->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR10_0 | GPIO_OSPEEDER_OSPEEDR10_1 | GPIO_OSPEEDER_OSPEEDR9_0 | GPIO_OSPEEDER_OSPEEDR9_1); //PF9 & PF10 high speed 50MHz
}


struct encoders_ticks read_encoders()
{
    struct encoders_ticks result;
    result.right = rightEncoderTicks;
    result.left = leftEncoderTicks;
		rightEncoderTicks=0;
		leftEncoderTicks=0;
    return result;
}

void start_encoder_counter (void)
{
	PB8_PB9_PF9_PF10configure();
	EXTI_configure();

}

void send_data_encoders(void)
{
	
	sendROSData(80, rightEncoderTicks, leftEncoderTicks, 0.0);
	rightEncoderTicks=0;
	leftEncoderTicks=0;
}
