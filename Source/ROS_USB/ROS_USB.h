/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "MD03.h"

struct __attribute__ ((__packed__)) ROSDataDef //Definition of the structure for sending the data to ROS 
{
			char ID;
			int data1;
			int data2;
			float data3;
	
} __attribute__ ((aligned));

void USB_init(void);
void sendROSData(char ID, int data1, int data2, float data3);
void receiveROSData(struct ROSDataDef *msg  );
