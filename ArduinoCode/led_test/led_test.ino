
#include <Servo.h>

// Initialize variables
Servo myservo;
int analogPotentiometer = 0;
int servoMotor = 9;
int val1;
int val2;
bool isOn = false;

// Setup code
void setup()
{
  // Attach the myservo motor to servoMotor
  // Initialize servo object and assign to pin 9 
  myservo.attach(servoMotor);
  // Initialize bit rate for serial transmission
  Serial.begin(9600);
}


void loop()
{
    // Read the output voltage value from potentiometer
    val1 = analogRead(analogPotentiometer);
    // Potentiiometer range from 179 to 1023
    val1 = map(val1,0,1023,0,179);
    // Write the value into the motor
    myservo.write(val1);
    // Delay a little bit
    delay(15);
    // Print the value of the output potentiometer
    Serial.println(val1);
}
