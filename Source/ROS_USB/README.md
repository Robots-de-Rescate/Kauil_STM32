**Functionality of this library**
----------------------------
This library was made for communicating the microcontroller with ROS using a simple protocol that is also specified in a ROS node for reading all the values. Go to the repository [Kauil_ROS](https://github.com/Robots-de-Rescate/Kauil_ROS) and open the README.md file in [Kauil_ROS/STM32_USB/](https://github.com/Robots-de-Rescate/Kauil_ROS/tree/master/STM32_USB) for understanding better the way ROS is going to interpret these values.

This library has also a function that packs all the functions needed for initialising the usb in one function. (**USB_init()**)

The function **receiveROSData()** is automatically called by the USB peripheral everytime ROS sends a message
so it's where actions upon received messages should be added depending on the ID of the message.

**When to use this library?**
--------------------------
When you need to send data to ROS.


**Considerations.**
-------------
If a new type of message is going to be sent, the character variable (char) that receives the function as an id of the variables to be sent with the function **sendROSData** has to be also specified in the ROS node. 

Go to the repository [Kauil_ROS](https://github.com/Robots-de-Rescate/Kauil_ROS) and edit [STM32_publisher.py](https://github.com/Robots-de-Rescate/Kauil_ROS/blob/master/STM32_USB/src/STM32_publisher.py) file in [Kauil_ROS/STM32_USB/src](https://github.com/Robots-de-Rescate/Kauil_ROS/tree/master/STM32_USB/src) for specifying the new id, crate a new message and create a new topic for publication of the data.


**How to use it.**
-------------
 1. Consider the types of data to be sent.
 2. Define a character value for identifying the data that you are going to send.
 3. Fill the function with the corresponding arguments.
 
 Example:

<pre><code>
    int sensorAValue = 100;
    int sensorBValue = 3.3;
    int sensorCValue1 = 3;
    int sensorCValue2 = 12;
    
    char idA = 'd';
    char idB = 'c';
    char idC = 'e';
    
    //Sending just sensor A value with the corresponding id
    sendROSData(idA, 0 , sensorA, 0.0);
    //Sending just sensor B value with the corresponding id
    sendROSData(idB, 0 , 0, sensorB);
    //Sending just sensor C values with the corresponding id
    sendROSData(idC, sensorCValue1, sensorCValue2, 0.0);
</code></pre>


