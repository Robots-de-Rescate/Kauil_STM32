/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "ROS_USB.h"
#include "timers.h"

/* Exported functions ------------------------------------------------------- */
void encoders_init (void);
struct encoders_ticks read_encoders(void);
/*structure definition ------------------------------------------------------ */ 
struct encoders_ticks{
    int right;
    int left;
};
