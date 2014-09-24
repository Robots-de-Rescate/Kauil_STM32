#include "main.h"


long unsigned SysTickCountl;

  RCC_ClocksTypeDef RCC_Clocks;
__IO uint32_t TimingDelay = 0;
__IO uint32_t UserButtonPressed = 0;
__IO float HeadingValue = 0.0f;  
int i, y = 0;
uint8_t * x;

	
int main( void )
{
	
  Set_System();
  //USB_init();
  MD03_Init();
    
  /* SysTick end of count event each 10ms */
  //RCC_GetClocksFreq(&RCC_Clocks);
  //SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

  //Demo_CompassConfig();
	
  while (1) {
		//sendROSData('a',12,i,12.01);
        i = MD03_Read(0xB2, 7, x, 1);
        y = *x; 

  }
}

//void Delay(__IO uint32_t nTicks)
//{
//	//long unsigned endTB = SysTickCount+nTime_ms;
//	//while( SysTickCount < endTB );
//	
//	TimingDelay = nTicks;
//	while(TimingDelay != 0);
//}

