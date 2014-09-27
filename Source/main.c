#include "main.h"

void timer3Config();
long unsigned SysTickCountl;

/////Funcion de prueba
void sendData(void);
//// lineas de prueba


  RCC_ClocksTypeDef RCC_Clocks;
__IO uint32_t TimingDelay = 0;
__IO uint32_t UserButtonPressed = 0;
__IO float HeadingValue = 0.0f;  
int i = 0;
uint8_t y = 0;


	
int main( void )
{

  Set_System();
  USB_init();
  timer3Config();
  MD03_Init();
	
  /* SysTick end of count event each 10ms */
  //RCC_GetClocksFreq(&RCC_Clocks);
  //SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

  Demo_CompassConfig();
	
  while (1)
	{
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

