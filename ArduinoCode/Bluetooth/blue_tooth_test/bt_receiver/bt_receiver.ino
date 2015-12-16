char blueToothVal;           //value sent over via bluetooth
char lastValue;              //stores last state of device (on/off)
int motorPin = 3;
String motorSpeed = "";
String data = "";

#include<SoftwareSerial.h>
#include<Servo.h>

// MAC ADDR:
// HC-05 - mac: 30-14-11-18-21-81
// HC-06 - mac: 30-14-12-17-14-80
// Sender = HC05 = 1411
// Receiver = HC06 = 1451
SoftwareSerial mySerial(10,11);//TX IS 10, RX IS 11
Servo myServo;
void setup()
{
 mySerial.begin(9600);
 Serial.begin(38400); 
 pinMode(13,OUTPUT);
 myServo.attach(motorPin);
}
 
int atoi2(String s){
  int value = 0;
  int sign = 1;
  int index = 0;
  if( s[index] == '+' || s[index] == '-'){
     if( s[index] == '-' ) sign = -1;
     index++;  
  }
  //Serial.println("S[index] is: ");
  //Serial.println(s.charAt(1));
  while ( s[index] >= '0' && s[index] <= '9' ){
    // Serial.println("at index: ");
   //  Serial.println(index);
     value *= 10;
     value += (int) (s[index] - '0');
     index++; 
  }
  //Serial.println("Returning: ");
  //Serial.println(value);
  return value * sign;
} 
 
 
void loop()
{
  if(mySerial.available())
  {//if there is data being recieved
    blueToothVal=mySerial.read(); //read it
    //Serial.println(blueToothVal);
    Serial.println(blueToothVal);
    delay(100);
  }
  if (blueToothVal=='n')
  {//if value from bluetooth serial is n
    digitalWrite(13,HIGH);            //switch on LED
    if (lastValue!='n')
      mySerial.println(F("LED is on")); //print LED is on
    lastValue=blueToothVal;
    blueToothVal = 'm';
    delay(100);
  }
  else if (blueToothVal=='f')
  {//if value from bluetooth serial is n
    digitalWrite(13,LOW);             //turn off LED
    if (lastValue!='f')
      mySerial.println(F("LED is off")); //print LED is on
    lastValue=blueToothVal;
    blueToothVal = 'm';
    delay(100);
  }else{
    if(blueToothVal != 'm' && blueToothVal <= '9' && blueToothVal >= '0'){
       motorSpeed += (blueToothVal);
    }
    else{
      if(atoi2(motorSpeed) >= 1500 && atoi2(motorSpeed) <= 2000){
         Serial.println("motorspeed string: ");
         Serial.println(motorSpeed);
         Serial.println("motorspeed as num: ");
         Serial.println(atoi2(motorSpeed));
         myServo.writeMicroseconds(atoi2(motorSpeed));
         delay(200);
       }
       else if(atoi2(motorSpeed) < 1500){
         myServo.writeMicroseconds(1460); 
         delay(200);
       }
       blueToothVal = 'm';
       motorSpeed = "";
    }   
  }
}


