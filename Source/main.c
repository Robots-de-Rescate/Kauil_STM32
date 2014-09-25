#include "main.h"


long unsigned SysTickCountl;

  RCC_ClocksTypeDef RCC_Clocks;
__IO uint32_t TimingDelay = 0;
__IO uint32_t UserButtonPressed = 0;
__IO float HeadingValue = 0.0f;  
int i = 0;
uint8_t y = 0;

	
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
        y = 120;
        i = MD03_Write(0xB2, 2, &y); // writing 120 to register 2
        i = MD03_Read(0xB2, 7, &y, 1); // y should be 13
        i = MD03_Read(0xB2, 2, &y, 1); // y should read 120 

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

