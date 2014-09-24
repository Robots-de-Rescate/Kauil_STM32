#include "timers.h"

void (*func[10])(void);
int tim[10];
int contadores[10];
int pos = 0;

void timer3Config(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //TIM3 clock enable
	TIM3->PSC = 199;
	TIM3->ARR = 360;
	TIM3->CR1 = TIM_CR1_CEN; //Enable timer
	
	TIM3->DIER |= 1<<0; //Enable interrupt
	NVIC->ISER[0] |= 1 << 29;	//Enable TIM3 interrupt in NVIC
}


void setTimer(void (*function)(void), int timer)
	{
		func[pos] = function;
		tim [pos] = timer;
		pos ++;	
	}

	
void TIM3_IRQHandler (void) 
	{
	if(TIM3->SR & TIM_SR_UIF) 
		{
		TIM3->SR &= ~TIM_SR_UIF; 
		contadores[0] ++;
		if(contadores[0] == tim[0])
			{
			func[0]();
			contadores[0] = 0;
			}
		}
	}