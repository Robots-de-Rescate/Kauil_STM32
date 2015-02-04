**Functionality of this library**
----------------------------
This library reads a pair of quadrature encoders through interrupts and registers 
a callback every 100ms using the timers module to send a message to ROS with the 
ammount of pulses measured in this period of time.

**How it works**
---------------

We register an interrupt on the channel A of the encoder, and when it happens we
check the status of channel B, this way we know if it was a forward or backward
pulse, and we keep the count of pulses untill the timers module calls `send_data_encoders()`
then we clear the count and send the message via USB.

**When to use this library?**
--------------------------
This module is not intended to be used from outside and works by itelf.
