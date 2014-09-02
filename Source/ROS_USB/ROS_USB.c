/* Includes ------------------------------------------------------------------*/
#include "ROS_USB.h"


/* Variables ---------------------------------------------------------*/
extern __IO uint8_t Receive_Buffer[64];
extern __IO  uint32_t Receive_length ;
extern __IO  uint32_t Send_length ;
uint32_t packet_sent = 0;
uint32_t packet_receive = 0;
uint8_t *ROSDataValuesPtr;


struct ROSDataDef //Definition of the structure for sending the data to ROS 
{
			char ID;
			int data1;
			float data2;
			
};

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Initialization of the USB (USB_init)
* Description    : Summarize all functions needed for the USB initialization in one function for having less code on main.c
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_init(void){
	Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();
	}

/*******************************************************************************
* Function Name  : (sendROSData)
* Description    : This function prepares the data to be sent by USB to ROS
* Input          : ID of the sensor data to be sent (first argument), Sensors  data (Second and third argument)
* Return         : None.
*******************************************************************************/

void sendROSData(char ID, int data1, float data2)
	{
		struct ROSDataDef ROSDataValues;//Initialization of the srtructure
		ROSDataValuesPtr = (uint8_t *) &ROSDataValues; //Asignation of the structure pointer
		
		//Asignment of the values of the function's arguments on the struct
		ROSDataValues.ID = ID;
		ROSDataValues.data1 = data1;
		ROSDataValues.data2 = data2;
		
		if(bDeviceState == CONFIGURED)
		{
			CDC_Send_DATA(ROSDataValuesPtr,9); // 9 bytes is the length of the struct
		}
	}
	
	