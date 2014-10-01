#include "stm32f3_discovery_lsm303dlhc.h"
#include "stm32f3_discovery_l3gd20.h"
#include "math.h"
#include "timers.h"
#include "ROS_USB.h"


void compass_init(void);
void Demo_CompassReadMag (float* pfData);
void Demo_CompassReadAcc(float* pfData);
float readMag(void);
