#import "timers.h"


void timer3Config(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //TIM3 clock enable
	TIM3->PSC = 199;
	TIM3->ARR = 360;
	TIM3->CR1 = TIM_CR1_CEN; //Enable timer
	
	TIM3->DIER |= 1<<0; //Enable interrupt
	NVIC->ISER[0] |= 1 << 29;	//Enable TIM3 interrupt in NVIC
}