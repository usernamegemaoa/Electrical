
#include <Servo.h>

Servo myservo;
int digitalPin = 10;
int analogPotentiometer = 0;
int servoMotor = 9;
int val1;
int val2;
bool isOn = false;
void setup()
{
  
  pinMode(digitalPin,OUTPUT);
  myservo.attach(servoMotor);
  Serial.begin(9600);
  
}


void loop(){
 
 
 val2 = digitalRead(digitalPin);
 Serial.println(val2);
 delay(3000);
if(val2 == HIGH)
 isOn = true;
 while(isOn){
 val1 = analogRead(analogPotentiometer);
 val1 = map(val1,0,1023,0,179);
 myservo.write(val1);
 delay(15);
 Serial.println(val1);
 //delay(1);
 //analogWrite(digitalPin, val/4);
}
 
}
