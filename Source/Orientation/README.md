**Functionality of this library**
----------------------------
This module contains the libraries provided by ST to communicate to the Magnetometer, Accelerometer and Gyroscope of
the STM32F3Discovery board. It provides functions that initialize the devices and read or write data to them.

Currently we just read the magnetometer and take the X an Y coordinates to get the heading, and send it using the
timers module every 170ms to ROS via USB.
