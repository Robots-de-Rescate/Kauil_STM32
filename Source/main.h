#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include <stdio.h>
#include "stm32f3_discovery_lsm303dlhc.h"
#include "stm32f3_discovery_l3gd20.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "math.h"

void Demo_CompassConfig(void);
float readMag(void);
void Demo_CompassReadMag (float* pfData);
extern long unsigned SysTickCount;
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
void Demo_CompassReadAcc(float* pfData);
