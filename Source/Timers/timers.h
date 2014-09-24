#include "stm32f30x.h"
extern void (*func[10])(void);
extern int tim[10];
extern int contadores[10]; 



void timer3Config(void);
void setTimer(void (*function)(void), int timer);
void TIM3_IRQHandler (void);
