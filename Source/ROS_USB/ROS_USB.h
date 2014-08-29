/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

/* Exported functions ------------------------------------------------------- */
void USB_init(void);
void sendROSData(char ID, int data1, float data2);
