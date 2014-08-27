#include "main.h"


extern __IO uint8_t Receive_Buffer[64];
extern __IO  uint32_t Receive_length ;
extern __IO  uint32_t length ;
uint8_t Send_Buffer[64];
uint32_t packet_sent=1;
uint32_t packet_receive=1;

long unsigned SysTickCountl;

  RCC_ClocksTypeDef RCC_Clocks;
__IO uint32_t TimingDelay = 0;
__IO uint32_t UserButtonPressed = 0;
__IO float HeadingValue = 0.0f;  

	
int main( void )
{
	//float arreglo[3] = {1.25,0.5,0.25};
	float *heading_ptr;
	float heading;
	Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();
	
	/* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

  Demo_CompassConfig();
	
  while (1)
  {
		
    if (bDeviceState == CONFIGURED)
    {
      CDC_Receive_DATA();
			
      /*Check to see if we have data yet */
      if (Receive_length  != 0)
      {
        if (packet_sent == 1)
				heading = readMag();
				//heading = 450.5f;
				heading_ptr = &heading;
				CDC_Send_DATA ((unsigned char*)heading_ptr,4);
        Receive_length = 0;
      }
    }
  }
}

void Delay(__IO uint32_t nTicks)
{
	//long unsigned endTB = SysTickCount+nTime_ms;
	//while( SysTickCount < endTB );
	
	TimingDelay = nTicks;
	while(TimingDelay != 0);
}
