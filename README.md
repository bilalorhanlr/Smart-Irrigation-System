# Smart-Irrigation-System

You can change the phone number to which the remaining battery and sensor information will be sent from here.

```c
String PHONE_NUMBER = "+90553323xxxx";
```

If you do not have the right to send a message, you should comment out these two lines of code.

```c
voltage_calculation();
delay(1000); 
```

The digits 0, 1, and 2 that we will send to this SIM card are the numbers we use to control the valve. These can be changed. The current usage is as follows:
- 10: valve 1 is closed.
- 11: valve 1 is open.
- 20: valve 2 is closed.
- 21: valve 2 is open.

```c
 if (msg[0] == '1')
      {
        if (msg[1] == '0')
        { 
          digitalWrite(25, HIGH);
          digitalWrite(26, LOW);
          delay (200);
          digitalWrite(25, LOW);
          digitalWrite(26, LOW);
        }
        else if (msg[1] == '1')
        { digitalWrite(25, LOW);
          digitalWrite(26, HIGH);
          delay (200);
          digitalWrite(25, LOW);
          digitalWrite(26, LOW);
        }
       }
      if (msg[0] == '2')
      {
        if (msg[1] == '0')
        { digitalWrite(12, HIGH);
          digitalWrite(13, LOW);
          delay (200);
          digitalWrite(12, LOW);
          digitalWrite(13, LOW);  
        }
        else if (msg[1] == '1')
        { digitalWrite(12, LOW);
          digitalWrite(13, HIGH);
          delay (200);
          digitalWrite(12, LOW);
          digitalWrite(13, LOW);
        }
      }
```
    
You can access the AT commands we used in the project through this PDF

[AT Commands PDF Link](https://github.com/bilalorhanlr/Smart-Irrigation-System/blob/main/AT_Commands_Reference_Guide_r0.pdf)

## Electronic

The circuit diagram is as follows:

![This is an image](https://github.com/bilalorhanlr/Smart-Irrigation-System/blob/main/Schematic.png)




