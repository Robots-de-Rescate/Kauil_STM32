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


	
int main( void )
{

	float *heading_ptr;
	float heading;
	float compas;
	
	
	//Lineas de prueba
	//void (*function)(void) = &sendData;
	//
	
	
	Set_System();
  USB_init();
	timer3Config();
	
		
	/* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

  Demo_CompassConfig();
	
	setTimer(sendData,2000);
	while (1);
}

////funcion de prueba
void sendData(void){
	sendROSData('c',12,12,1.0);
}
///LIneas de prueba


void Delay(__IO uint32_t nTicks)
{
	//long unsigned endTB = SysTickCount+nTime_ms;
	//while( SysTickCount < endTB );
	
	TimingDelay = nTicks;
	while(TimingDelay != 0);
}




