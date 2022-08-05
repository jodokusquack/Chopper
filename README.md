# Chopper
Code for running an Arduino Powered Light Chopper

## Goal
I wanted to build a Chopper that can block a light source in regular intervals.
The Chopper works quite well in the current configuration; however, it has some limitations.
E.g. the interval in which the shutter is closed is always the same length as the interval where the shutter is open.
I believe this can be enhanced by changing the code, thereby making the use more flexible.
I just haven’t gotten around to it yet.
I would also at some point have the ability to change the interval after some time period.
(E.g. have an interval of 5 sec for a running time of 30 min and then switch to an interval of 120 sec automatically.)
Pull request welcome! :)

## Function
Basically, we set a time interval using 4 of the 6 buttons.
The 4 buttons increase or decrease the interval by 1 or 10 sec each. The other two buttons are for activating the automatic chopping and for manually changing the state of the shutter.

The Display shows the current state of the Chopper (If its running or not), the current state of the shutter (If its open or closed) and the current interval or timer (depending on whether the Chopper is active or not).

The capacitors which connect over the Push Buttons are there to make the signal more stable and exact.
Basically the problem with cheap buttons like these is, that when you press it the signal doesn’t go from High to Low instantly, rather the signal starts to bounce.
The Arduino then thinks that you have pressed the button several times in a row.
The Capacitor smoothes the signal and should in theory eliminate the bounce.
Alternatively, you can also eliminate the bounce on the software side, by deactivating the button for 1 sec after one change.
More info on this topic can be found in this very helpful post:  http://gammon.com.au/switches.

Finally, the power supply: I have opted to power the Arduino and the servo separately, because I read that the current draw of the Servo motor can reach over 1 A e.g. if it gets blocked.
The Arduino Nano can only safely supply 500 mA from its 5V output, so this could theoretically damage the Arduino.
However, I have also seen people drive a Servo directly from the 5V output, maybe it can handle it short term but not long term?
I have opted to be on the safe side and power the motor directly from the USB Charger.
(The display can safely be powered from the Arduino 5V supply)
Always connect all the grounds together!

## Schematic
![Chopper_bb](https://user-images.githubusercontent.com/53518741/183160555-cb0103a4-d5f7-46e2-9240-a2c968f5866f.png)

## Code Comments
The code uses the “Servo” library to control the Servo motor and the “U8g2lib” library for controlling the display.
You will need to download the second library.
The first one might be included with the Arduino IDE.
If you connect the pins the exact same way that I did in the schematic, it should work right away.
You can of course use different pins, however, then you will have to change them in the code.
The Arduino Nano has internal Pullup resistors on some pins so I used pins with this in order to avoid “floating” inputs from the buttons.

(Small note: If you use the Arduino IDE to upload the code to the Nano, you will have to remove the first line, which says “#include <Arduino.h>”. This line is only required if you use another program to upload the code (e.g. PlatformIO on VSCode))

Fun Fact: The code used the millis() function which gives back the amount of milliseconds that the Arduino has been on.
This number will overflow after about 50 days.
I don’t know what happens then, so I can only guarantee the error-free operation for 49 days continuously 😊.
