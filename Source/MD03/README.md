**Functionality of this library**
----------------------------
This module provides a way of controlling the speed of the motors on Kauil by abstracting the communication
and letting people use the function set_motors_speed().

**When to use this library?**
--------------------------
When you need to communicate to the MD03 boards to move Kauil.

**Considerations.**
-------------------
The I2C addresses of the MD03 boards are hardcoded on the MD03.h file as follows:

```
#define LEFT_MOTOR_ADDRESS 0xB0
#define RIGHT_MOTOR_ADDRESS 0xB2
```

**How to use it.**
-------------
Import the module MD03.h and call the function:

set_motors_speed(int32_t left_speed, int32_t right_speed)

left_speed and right_speed can take values from -255 to 255, negative values represent reverse movement, and
positive values represent forward movement.

**Example:**

```
#include MD03.h
set_motors_speed(-255, 255) //will turn left at max speed
```
