/* This program
 will test the bluetooth functionality
 of the sender which is HC05
 For assistance:
 MAC ADDR:
 HC-05 - mac: 30-14-11-18-21-81
 HC-06 - mac: 30-14-12-17-14-80
 Sender = HC05 = 1411
 Receiver = HC06 = 1451
*/

// Libraries/Files to include
#include <SoftwareSerial.h>

// Variables here for buttons/components on the board
SoftwareSerial BTSerial(10, 11); // rx |tx
int potentiometer = 0; // A0
int ledTestButton = 3; // D3
int motorButton = 4; // D4
int led1 = 5; // D5
int led2 = 6; // D6
bool ledTestState = false;
bool ledOn = false;
bool motorState = false;
int motorSpeed = 0; // data read from potentiometer
String motorSpdStr = "";
byte buf[] ={};

// Initialize pins and serial
void setup() {
  Serial.begin(9600); // MUST BE 9600
  BTSerial.begin(38400); // MUST BE 38400
  pinMode(ledTestButton, INPUT); // button as input
  pinMode(motorButton, INPUT); // button as input
  pinMode(led1, OUTPUT); // pin to diode
  pinMode(led2, OUTPUT); // pin to diode
  pinMode(potentiometer, INPUT);
}


// Method that handles the LED on the board (receiver BT module)
// and the controller (sender BT module)
void turnOnLight() {
  delay(150);
  // Read button state
  if (digitalRead(ledTestButton) == HIGH) {
    // Turn the led off/on
    if (ledOn == false) {
      BTSerial.write('n');
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      ledOn = true;
    }
    else {
      BTSerial.write('f');
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      ledOn = false;
    }
  }
  delay(300);
}

// Method that handles the potentiometer input and the motor rotation
void handleMotor() {
  delay(150);
  if (digitalRead(motorButton) == HIGH) {
    // get value of potentiometer here
    // then send
    motorSpeed = 0;
    motorSpeed = analogRead(potentiometer);
    motorSpeed = map(motorSpeed, 0, 1023, 1000, 2000);
    Serial.println(motorSpeed);
    motorSpdStr = "";
    motorSpdStr += motorSpeed;
    motorSpdStr += 'm';
   // motorSpdStr.getBytes(buf, motorSpdStr.length());
    Serial.println(motorSpdStr.length());
    for(int i = 0; i < motorSpdStr.length(); i++)  
       BTSerial.print(motorSpdStr[i]);
    delay(100);
  }
  else {
    motorSpdStr = "m";
    delay(100);
  }
}

void loop () {
  // Determine if the lights need to be turned off or on
  turnOnLight();

  // Control the motor speed / direction
  //  motorState = digitalRead(motorButton);
  handleMotor();
}
