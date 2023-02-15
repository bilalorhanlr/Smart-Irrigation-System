#include <SoftwareSerial.h>

String PHONE_NUMBER = "+90553323xxxx";


#define DTR_PIN 14
#define SHUTDOWN_PIN 5

#define RX_PIN 16
#define TX_PIN 17

#define VALVE_A_1 25
#define VALVE_A_2 26

#define VALVE_B_1 12
#define VALVE_B_2 13

#define BATTERY_PIN 35


SoftwareSerial sim800(RX_PIN, TX_PIN);

String smsStatus, senderNumber, receivedDate, msg;

int x = 0;

void setup() {
  pinMode(SHUTDOWN_PIN, OUTPUT);  
  digitalWrite(SHUTDOWN_PIN,HIGH);                        /* SHUTDOWN PIN HIGH FOR ESP to be active */
  
  pinMode(DTR_PIN, OUTPUT);                               /* DTR PIN DEFINE FOR GMS to be sleep */

  pinMode(VALVE_A_1, OUTPUT);                             
  pinMode(VALVE_A_2, OUTPUT);
  pinMode(VALVE_B_1, OUTPUT);
  pinMode(VALVE_B_2, OUTPUT);
  
  Serial.begin(115200);                                   
  Serial.println("Arduino serial initialize");
  sim800.begin(9600);
  Serial.println("SIM800L software serial initialize");
  
  
  smsStatus = "";
  senderNumber = "";
  receivedDate = "";
  msg = "";
  
  delay(7000);
  digitalWrite(DTR_PIN,LOW);                                 /* DTR PIN LOW FOR GSM to be wake up */
  sim800.println("AT");                                      /* TRASH COMMAND*/
  sim800.println("AT+CSCLK=0");                              /* IF ALL COMMAND IS OKEY GSM SLEEP*/
  delay(1000);
  sim800.print("AT+CMGF=1\r");
  delay(500);
  sim800.println("AT+CMGD=1,4");
  delay(1000);
  sim800.println("AT+CMGDA= \"DEL ALL\"");
  delay(1000);
  
}

void send_messages(String text)
{
  sim800.print("AT+CMGF=1\r");
  delay(1000);
  sim800.print("AT+CMGS=\"" + PHONE_NUMBER + "\"\r");
  delay(1000);
  sim800.print(text);
  delay(250);
  sim800.write(0x1A); // ascii code for ctrl-26 //sim800.println((char)26); //ascii code for ctrl-26
  delay(1000);
  Serial.println("SMS Sent Successfully.");
}

void extractSms(String buff) {
  unsigned int index;

  index = buff.indexOf(",");
  smsStatus = buff.substring(1, index - 1);
  buff.remove(0, index + 2);

  senderNumber = buff.substring(0, 13);
  buff.remove(0, 19);

  receivedDate = buff.substring(0, 20);
  buff.remove(0, buff.indexOf("\r"));
  buff.trim();

  index = buff.indexOf("\n\r");
  buff = buff.substring(0, index);
  buff.trim();
  msg = buff;
  buff = "";
  msg.toLowerCase();
  if ((msg[0] >= '0' && msg[0] <= '9') || (msg[0] >= 'a' && msg[0] <= 'z'))
    int x = 0;
  else
    msg = msg.substring(4, msg.length());
}

void voltage_calculation()
{
  float value =(float) analogRead(BATTERY_PIN) / 4095 * 9.42;
  int x = map(value,0,9.42,0,100);
  send_messages(String(x));
}

void loop() {
  while (sim800.available()) {
    parseData(sim800.readString());
  }
  while (Serial.available())  {
    sim800.println(Serial.readString());
  }
} //main loop ends

void parseData(String buff) {
  Serial.println(buff);

  unsigned int len, index;

  index = buff.indexOf("\r");
  buff.remove(0, index + 2);
  buff.trim();

  if (buff != "OK") {
    index = buff.indexOf(":");
    String cmd = buff.substring(0, index);
    cmd.trim();

    buff.remove(0, index + 2);

    if (cmd == "+CMTI") {
      index = buff.indexOf(",");
      String temp = buff.substring(index + 1, buff.length());
      temp = "AT+CMGR=" + temp + "\r";
      sim800.println(temp);
    }
    else if (cmd == "+CMGR") 
    {
      extractSms(buff);
      
      Serial.println(msg);
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
      
      sim800.println("AT+CMGD=1,4");
      delay(1000);
      sim800.println("AT+CMGDA= \"DEL ALL\"");
      delay(1000);
      digitalWrite(14,HIGH);
      delay(100);
      sim800.println("AT+CSCLK=2");
      delay(100);
      voltage_calculation();
      delay(1000);        
      digitalWrite(SHUTDOWN_PIN,LOW);
    }
    else {
   
      //The result of AT Command is "OK"
    }
  }
}
