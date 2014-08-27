#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include <stdio.h>
#include "orientation.h"
#include "usb_lib.h"
#include "hw_config.h"

extern long unsigned SysTickCount;
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
