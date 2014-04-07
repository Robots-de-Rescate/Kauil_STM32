#include "main.h"
#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include "stm32f3_discovery_lsm303dlhc.h"
#include "stm32f3_discovery_l3gd20.h"

#include "imu_util.h"
#include "imu_devs.h"
#include "MadgwickAHRS.h"
//#include "MahonyAHRS.h"
#include "MadgwickFullAHRS.h"

extern __IO uint8_t Receive_Buffer[64];
extern __IO  uint32_t Receive_length ;
extern __IO  uint32_t length ;
uint8_t Send_Buffer[64];
uint32_t packet_sent=1;
uint32_t packet_receive=1;

long unsigned SysTickCountl;
static __IO uint32_t TimingDelay = 0;
	
int main( void )
{
	float arreglo[3] = {1.25,0.5,0.25};
	
//	long unsigned tb;
//  float quaternion[4] = {1,0,0,0};
//	RCC_ClocksTypeDef RCC_Clocks;
	
	
	Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();

//  // Madgwick filter coeff (orig 0.3 but increased for faster initial convergence)
//  madgwickBeta=0.5f;

//  // initial sample timestamp
//  tb = SysTickCount;
//	
//  RCC_GetClocksFreq(&RCC_Clocks);

//  /* SysTick end of count event each 1ms */
//  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

//  InitAccAndMag();
//  InitGyro();
//	
//	  // Madgwick filter coeff (orig 0.3 but increased for faster initial convergence)
//  madgwickBeta=0.5f;

//  // initial sample timestamp
//  tb = SysTickCount;
  
  while (1)
  {
//		float a[3],m[3],g[3];
//    long unsigned tb2;
//    float samplePeriod;
//		float ypr[3];
		
    if (bDeviceState == CONFIGURED)
    {
      CDC_Receive_DATA();
//			// read all the sensors
//    sampleSensors(a,m,g);

//    // get time of acquisition
//    tb2 = SysTickCount;

//    // sample period is time elapsed since previous sampling of sensors
//    samplePeriod=0.001f*(tb2-tb);

//    // update timebase for next time
//    tb = tb2;

//    //g[0]=g[1]=g[2]=0;
//    //MadgwickAHRSupdateIMU( g, a, samplePeriod, quaternion );
//    //MadgwickAHRSupdate( g, a, m, samplePeriod, quaternion );

//    MadgwickFullAHRSUpdate( g, a, m, samplePeriod, quaternion );
//    // it's < 1ms to compute the above!
//		
//			imuQuaternionToYawPitchRoll(quaternion,ypr);
//			imuRadToDegV3(ypr);
  
  // Commented by Carbajal 31/10/2013
  //	printf("%4.0f,%4.0f,%4.0f,YPR %3dHz\r\n", ypr[0],ypr[1],ypr[2], (int)(1.0f/samplePeriod));
  
			//Delay(5);
			
      /*Check to see if we have data yet */
      if (Receive_length  != 0)
      {
        if (packet_sent == 1)
        //CDC_Send_DATA ((unsigned char*)ypr,6);
				CDC_Send_DATA ((unsigned char*)arreglo,12);
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

void sampleSensors(float a[], float m[], float g[])
{
  // status has lower 3 bits to remember which sensor are already read
  unsigned status=0;
  while( (status & 0x7) != 0x7 )
  {
    if( (status & 0x1) == 0 && (LSM303DLHC_AccGetDataStatus() & 0x8) != 0 )
    {
      ReadAccelerometer(a);
      status |= 0x1;
    }
    if( (status & 0x2) == 0 && (LSM303DLHC_MagGetDataStatus() & 0x1) != 0 )
    {
      ReadMagnetometer(m);
      status |= 0x2;
    }
    if( (status & 0x4) == 0 && (L3GD20_GetDataStatus() & 0x8) != 0 )
    {
      ReadGyro(g);
      status |= 0x4;
    }
  }
}
