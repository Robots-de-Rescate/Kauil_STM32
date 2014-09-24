**Functionality of this library**
----------------------------
At first glance, this library was created for acquiring multiple data sensors at different frequencies by only using one Timer.

This Library is helpful for stop using one timer per each function that needs to be executed at a certain amount of time. By only using one timer, other timers will be available for other purposes. The main goal is to do not waste resources of the microcontroller.
	

**When to use this library?**
--------------------------
When you have a function that is needed to be executed at a certain frequency.


**How to use it.**
-------------
Once you have defined the time and a void type function with no arguments, use the function **setTimer(arg #1, arg#2);**

Example:

<pre><code>
       //arg#1: Void type function to be executed that receives no arguments.
       //arg#2: Integer that indicate the amount of time in miliseconds 
       setTimer(encoders, 1000);
</code></pre>

The function encoders is going to be executed every second.



