#include <stm32f30x.h>
#include "string.h"
#include "delay.h"
#include "stdio.h"

volatile int32_t rightEncoderTicks=0, tick2=0, contador_tiempo=0,x;
uint16_t ADC1ConvertedValuebatt1 = 0, ADC1ConvertedValuebatt2 = 0, ADC1ConvertedValuebatt3 = 0, ADC1ConvertedValuebatt4 = 0; 
uint16_t ADC1ConvertedVoltagebatt1 = 0, ADC1ConvertedVoltagebatt2 = 0, ADC1ConvertedVoltagebatt3 = 0, ADC1ConvertedVoltagebatt4 = 0; 
uint16_t calibration_value = 0;


void ADC1_configure (void)
{
		// At this stage the microcontroller clock tree is already configured 
		RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV2; // Configure the ADC clock 
		RCC->AHBENR |= RCC_AHBENR_ADC12EN; // Enable ADC1 clock 
		//ADC configuration
		ADC1->CFGR |= ADC_CFGR_CONT; // ADC_ContinuousConvMode_Enable 
		ADC1->CFGR &= ~ADC_CFGR_RES; // 12-bit data resolution 
		ADC1->CFGR &= ~ADC_CFGR_ALIGN; // Right data alignment
}


void Timer7_configure(void)//configuration for an interrupt of 100ms
{
	RCC->APB1ENR|=RCC_APB1ENR_TIM7EN;//enable clock for TIM7
	TIM7->CR1&=~0x00000001;//Disable TIM7 interrupt
	TIM7->PSC=54;//prescaler=55, PSC=prescaler-1
	TIM7->ARR=65454;//preload=65455, ARR=preload-1, 
	NVIC_EnableIRQ(TIM7_IRQn);
	TIM7->DIER|=0x01;//Enable TIM7 interrupt (update interrupt enable)
	TIM7->CR1|=0x01;//TIM7 enable (counter enable)
}

void EXTI_configure(void)
{
	RCC->APB2ENR|= 1;//eneables syscfg clock
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PB;//configure syscfg register that enables multiplexed PB3 to enter EXTI3
	SYSCFG->EXTICR[1] |= SYSCFG_EXTIRCR_EXTI4_PB;//configure syscfg register that enables multiplexed PB4 to enter EXTI4
	EXTI->IMR |= (1<<3|1<<4);//interrupt mask register of exti3 and exti4 set as unmask so that it cant be ignore by processor
	EXTI->RTSR |= (1<<3|1<<4);//set the interruption as rising trigger for exti3 and exti4
	NVIC->ISER[0] |= 1<<9;//enable nvic set register for exti 3
	NVIC->ISER[0] |= 1<<10;//enable nvic set register for exti 4	
	
}

void TIM7_IRQHandler(void)
{
	
	TIM7->SR&=~0x00000001;//clear interrupt flag (update interrupt flag)
	rightEncoderTicks=0;//resets the count in rightEncoderTicks
	contador_tiempo++;
	if (contador_tiempo==100)//Tim 7 interrupt occurs every 0.1 secs, so: 0.1 seconds*3000=300sec=5min, therefore this "if" happens every 5 min 
	{	
      volatile unsigned short i=0, j=0;  		
			contador_tiempo=0;
						
			 
				for(i=0; i<=1; i++)
				{	
					for(j=0; j<=1; j++)
					{
						GPIOC->ODR=((j<<2)|(i<<3));
						delaybyms(3000);
						switch(i){
							case 0:
								if(j==0){
										x=1;
										/* Calibration procedure */ 
										ADC1->CR &= ~ADC_CR_ADVREGEN; 
										ADC1->CR |= ADC_CR_ADVREGEN_0; // 01: ADC Voltage regulator enabled 
										delaybyus(10);// Insert delay equal to 10 탎 
										ADC1->CR &= ~ADC_CR_ADCALDIF; // calibration in Single-ended inputs Mode. 
										ADC1->CR |= ADC_CR_ADCAL; // Start ADC calibration 
										// Read at 1 means that a calibration in progress. 
										while (ADC1->CR & ADC_CR_ADCAL); // wait until calibration done 
										calibration_value = ADC1->CALFACT; // Get Calibration Value ADC1 
										/*ADC1 regular channel7 configuration*/
										ADC1->SQR1 |= ADC_SQR1_SQ1_2 | ADC_SQR1_SQ1_1 | ADC_SQR1_SQ1_0; // SQ1 = 0x07, start converting ch7 
										ADC1->SQR1 &= ~ADC_SQR1_L; // ADC regular channel sequence length = 0 => 1 conversion/sequence 
										ADC1->SMPR1 |= ADC_SMPR1_SMP7_1 | ADC_SMPR1_SMP7_0; // = 0x03 => sampling time 7.5 ADC clock cycles 
										ADC1->CR |= ADC_CR_ADEN; // Enable ADC1 
										while(!ADC1->ISR & ADC_ISR_ADRD); // wait for ADRDY 
										ADC1->CR |= ADC_CR_ADSTART; // Start ADC1 Software Conversion 
										while(!(ADC1->ISR & ADC_ISR_EOC)); // Test EOC flag(waiting end of conversion)
										ADC1ConvertedValuebatt1 = ADC1->DR; // Get ADC1 converted data 
										ADC1ConvertedVoltagebatt1 = (ADC1ConvertedValuebatt1 *3300)/4096; // Compute the voltage 
										//stopping conversion
										ADC1->CR |= ADC_CR_ADSTP;//stop conversion
										while(ADC1->CR & ADC_CR_ADSTP);//wait for ADC stopping conversion
								}
								else if(j==1){
										x=2;
										/* Calibration procedure */ 
										ADC1->CR &= ~ADC_CR_ADVREGEN; 
										ADC1->CR |= ADC_CR_ADVREGEN_0; // 01: ADC Voltage regulator enabled 
										delaybyus(10);// Insert delay equal to 10 탎 
										ADC1->CR &= ~ADC_CR_ADCALDIF; // calibration in Single-ended inputs Mode. 
										ADC1->CR |= ADC_CR_ADCAL; // Start ADC calibration 
										// Read at 1 means that a calibration in progress. 
										while (ADC1->CR & ADC_CR_ADCAL); // wait until calibration done 
										calibration_value = ADC1->CALFACT; // Get Calibration Value ADC1 
										/*ADC1 regular channel7 configuration*/
										ADC1->SQR1 |= ADC_SQR1_SQ1_2 | ADC_SQR1_SQ1_1 | ADC_SQR1_SQ1_0; // SQ1 = 0x07, start converting ch7 
										ADC1->SQR1 &= ~ADC_SQR1_L; // ADC regular channel sequence length = 0 => 1 conversion/sequence 
										ADC1->SMPR1 |= ADC_SMPR1_SMP7_1 | ADC_SMPR1_SMP7_0; // = 0x03 => sampling time 7.5 ADC clock cycles 
										ADC1->CR |= ADC_CR_ADEN; // Enable ADC1 
										while(!ADC1->ISR & ADC_ISR_ADRD); // wait for ADRDY 
										ADC1->CR |= ADC_CR_ADSTART; // Start ADC1 Software Conversion 
										while(!(ADC1->ISR & ADC_ISR_EOC)); // Test EOC flag(waiting end of conversion)
										ADC1ConvertedValuebatt2 = ADC1->DR; // Get ADC1 converted data 
										ADC1ConvertedVoltagebatt2 = (ADC1ConvertedValuebatt2 *3300)/4096; // Compute the voltage 
										//stopping conversion
										ADC1->CR |= ADC_CR_ADSTP;//stop conversion
										while(ADC1->CR & ADC_CR_ADSTP);//wait for ADC stopping conversion
								}
								break;
								case 1:
								if(j==0){
										x=3;
										/* Calibration procedure */ 
										ADC1->CR &= ~ADC_CR_ADVREGEN; 
										ADC1->CR |= ADC_CR_ADVREGEN_0; // 01: ADC Voltage regulator enabled 
										delaybyus(10);// Insert delay equal to 10 탎 
										ADC1->CR &= ~ADC_CR_ADCALDIF; // calibration in Single-ended inputs Mode. 
										ADC1->CR |= ADC_CR_ADCAL; // Start ADC calibration 
										// Read at 1 means that a calibration in progress. 
										while (ADC1->CR & ADC_CR_ADCAL); // wait until calibration done 
										calibration_value = ADC1->CALFACT; // Get Calibration Value ADC1 
										/*ADC1 regular channel7 configuration*/
										ADC1->SQR1 |= ADC_SQR1_SQ1_2 | ADC_SQR1_SQ1_1 | ADC_SQR1_SQ1_0; // SQ1 = 0x07, start converting ch7 
										ADC1->SQR1 &= ~ADC_SQR1_L; // ADC regular channel sequence length = 0 => 1 conversion/sequence 
										ADC1->SMPR1 |= ADC_SMPR1_SMP7_1 | ADC_SMPR1_SMP7_0; // = 0x03 => sampling time 7.5 ADC clock cycles 
										ADC1->CR |= ADC_CR_ADEN; // Enable ADC1 
										while(!ADC1->ISR & ADC_ISR_ADRD); // wait for ADRDY 
										ADC1->CR |= ADC_CR_ADSTART; // Start ADC1 Software Conversion 
										while(!(ADC1->ISR & ADC_ISR_EOC)); // Test EOC flag(waiting end of conversion)
										ADC1ConvertedValuebatt3 = ADC1->DR; // Get ADC1 converted data 
										ADC1ConvertedVoltagebatt3 = (ADC1ConvertedValuebatt3 *3300)/4096; // Compute the voltage 
										//stopping conversion
										ADC1->CR |= ADC_CR_ADSTP;//stop conversion
										while(ADC1->CR & ADC_CR_ADSTP);//wait for ADC stopping conversion
								}
								else if(j==1){
										x=4;
										/* Calibration procedure */ 
										ADC1->CR &= ~ADC_CR_ADVREGEN; 
										ADC1->CR |= ADC_CR_ADVREGEN_0; // 01: ADC Voltage regulator enabled 
										delaybyus(10);// Insert delay equal to 10 탎 
										ADC1->CR &= ~ADC_CR_ADCALDIF; // calibration in Single-ended inputs Mode. 
										ADC1->CR |= ADC_CR_ADCAL; // Start ADC calibration 
										// Read at 1 means that a calibration in progress. 
										while (ADC1->CR & ADC_CR_ADCAL); // wait until calibration done 
										calibration_value = ADC1->CALFACT; // Get Calibration Value ADC1 
										/*ADC1 regular channel7 configuration*/
										ADC1->SQR1 |= ADC_SQR1_SQ1_2 | ADC_SQR1_SQ1_1 | ADC_SQR1_SQ1_0; // SQ1 = 0x07, start converting ch7 
										ADC1->SQR1 &= ~ADC_SQR1_L; // ADC regular channel sequence length = 0 => 1 conversion/sequence 
										ADC1->SMPR1 |= ADC_SMPR1_SMP7_1 | ADC_SMPR1_SMP7_0; // = 0x03 => sampling time 7.5 ADC clock cycles 
										ADC1->CR |= ADC_CR_ADEN; // Enable ADC1 
										while(!ADC1->ISR & ADC_ISR_ADRD); // wait for ADRDY 
										ADC1->CR |= ADC_CR_ADSTART; // Start ADC1 Software Conversion 
										while(!(ADC1->ISR & ADC_ISR_EOC)); // Test EOC flag(waiting end of conversion)
										ADC1ConvertedValuebatt4 = ADC1->DR; // Get ADC1 converted data 
										ADC1ConvertedVoltagebatt4 = (ADC1ConvertedValuebatt4 *3300)/4096; // Compute the voltage 
										//stopping conversion
										ADC1->CR |= ADC_CR_ADSTP;//stop conversion
										while(ADC1->CR & ADC_CR_ADSTP);//wait for ADC stopping conversion
								}
								break;
						}			
					}
				}
	}
	
}
void EXTI3_IRQHandler (void)
{
	EXTI->PR|=(1<<3);	//clear the pending bit of the interrupt
	if(GPIOB->IDR&(1<<4))//reads the value in channel B 
	rightEncoderTicks=rightEncoderTicks+1;//B leads A (moving positive way)
	else
	rightEncoderTicks=rightEncoderTicks-1;//A leads B (moving negative way)
	
}
void EXTI4_IRQHandler (void)
{
		EXTI->PR|=1;	
		tick2=1+tick2;
}


	void PB3_PB4_configure(void)//GPIOs for EXTI 3 and 4
{
	//Input mode implicit if MODER configuration isn't configure 
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;//enable B port clock
	GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR3_0 | GPIO_PUPDR_PUPDR4_0); //PB3 y PB4 as PU
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR3_0 | GPIO_OSPEEDER_OSPEEDR4_0); //PB3 y PB4 high speed 50MHz
}

void PC1_PC2_PC3_configure(void)//GPIOs for ADC reading (PC1) and for decoder (PC2 & PC3)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // GPIOC Periph clock enable 
	GPIOC->MODER |= 3 << (1*2); // Configure ADC Channel7 as analog input 
	GPIOC->MODER |= ((1 << 4)|(1<<6));//output mode for PC2 and PC3
	GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR2_0 | GPIO_OSPEEDER_OSPEEDR3_0); //PC2 and PC3 high speed 50MHz
	
}
