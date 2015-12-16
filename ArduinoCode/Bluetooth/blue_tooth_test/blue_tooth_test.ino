char blueToothVal;           //value sent over via bluetooth
char lastValue;              //stores last state of device (on/off)
 
#include<SoftwareSerial.h>
// MAC ADDR:
// HC-05 - mac: 30-14-11-18-21-81
// HC-06 - mac: 30-14-12-17-14-80
// Sender = HC05 = 1411
// Receiver = HC06 = 1451
SoftwareSerial mySerial(10,11);//TX IS 10, RX IS 11
 
void setup()
{
 mySerial.begin(9600);
 Serial.begin(9600); 
 pinMode(13,OUTPUT);
}
 
 
void loop()
{
  if(mySerial.available())
  {//if there is data being recieved
    blueToothVal=mySerial.read(); //read it
    Serial.println(blueToothVal);
    
  }
  if (blueToothVal=='n')
  {//if value from bluetooth serial is n
    digitalWrite(13,HIGH);            //switch on LED
    if (lastValue!='n')
      mySerial.println(F("LED is on")); //print LED is on
    lastValue=blueToothVal;
  }
  else if (blueToothVal=='f')
  {//if value from bluetooth serial is n
    digitalWrite(13,LOW);             //turn off LED
    if (lastValue!='f')
      mySerial.println(F("LED is off")); //print LED is on
    lastValue=blueToothVal;
  }
  delay(1000);
}
