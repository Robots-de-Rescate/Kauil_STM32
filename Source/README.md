Source code
==========

This folder has all the source code of the firmware, every folder provides a reusable module of code that should provide a specific functionality to the robot.

Files that are not in any folder provide configuration and function definitions common to the whole system like system clock, interrupts and enabled peripherals, these configuration files are provided by the manufacturer; for more information about them please refer to "Description of STM32F30xx/31xx Standard Peripheral Library" ([UM1581](http://www.st.com/st-web-ui/static/active/en/resource/technical/document/user_manual/DM00068049.pdf)) page 28.

Because the idea is to write reusable code, it is expected that any new module will be added like a new folder with a meaningful name and not as spare files.

The folders and files that are found here are described next, if a new module is created, be sure to add its description here too:

<dl>
	<dt>README.md</dt>
	<dd>This file.</dd>
	<dt>README.img</dt>
	<dd>A folder with the images from this README.</dd>
	<dt>main.c/.h</dt>
	<dd>This is the main execution loop, it should contain just a global behavior of the system, any specific functionality should be abstracted as a new module.</dd>
	<dt>STM32F30x_StdPeriph_Driver</dt>
	<dd>Its ST's standard peripheral library which enables higher level functions to interact with the peripherals, the full documentation for it is the "Description of STM32F30xx/31xx Standard Peripheral Library" (<a href="http://www.st.com/st-web-ui/static/active/en/resource/technical/document/user_manual/DM00068049.pdf">UM1581</a>).</dd>
	<dt>STM32_USB-FS-Device_Driver</dt>
	<dd>This is the USB device library, it contains the basic middleware used to use the user USB port from the board. This is the main communication channel between the board and the system on board of the robot. The full documentation about it is "STM32 USB-FS-Device development kit" (<a href="http://www.st.com/st-web-ui/static/active/en/resource/technical/document/user_manual/CD00158241.pdf">UM0424</a>). </dd>
	<dt>USB_ApplicationInterface</dt>
	<dd>This folder has the Application layer required by the USB device library as explained in <a href="http://www.st.com/st-web-ui/static/active/en/resource/technical/document/user_manual/CD00158241.pdf">UM0424</a> page 21, right now it uses the one used in the VirtualComport_Loopback example found in page 53.</dd>
	<dt>Orientation</dt>
	<dd>This module uses libraries provided for the development board to provide an absolute orientation of the robot by using the magnetometer.</dd>
	<dt>Encoders</dt>
	<dd>Reads the quadrature encoders to enable the calculation of the odometry.</dd>
	<dt>Misc</dt>
	<dd>This folder is used to store small snippets of code that are not complete module, adding code here must not be a common practice.</dd>
	<dt>Ros_USB</dt>
	<dd>This directory contains the library that enable the comunication between the microcontroller and ROS. Open Kauil_STM32/Source/Ros_USB/README.md in order to have a more detail description.</dd>
	<dt>Timers</dt>       
 	 <dd>This directory contains the library that enables the configuration of the Timer 3,at the library it is also dfefined a function that calls a specified function (argument #1) in a specified time (argument #2). Open Kauil_STM32/Source/Timers/README.md for a more detail information of the library.</dd>


</dl>

