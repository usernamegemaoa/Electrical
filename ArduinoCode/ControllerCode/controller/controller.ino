// Include servo library
#include <Servo.h>

// Initialize variables
Servo myservo;
int analogPotentiometer = 0; // A0
int servoMotor = 2; // D2
int bluetoothButton = 3; // D3
int motorButton = 4; // D4
int diodeTest = 5; // D5
int flashLightButton = 6; // D6
int flashLightLED = 7; //D7
int val1;//value from potentiometer
//int val2;

int flashLightState = 0;//value to determine state of flash light when button is pressed
int servoMotorState = 0;//value to determine state of motor when button is pressed
int bluetoothState = 0;//value to determine state of bluetooth when button is pressed
unsigned int startTime = 0;
unsigned int currTime = 0;
bool motorOn = false;//boolean value to determine state of motor
bool bluetoothOn = false;//boolean value to determine state of bluetooth
bool lightOn = false;

// Setup code
void setup()
{ 
    // Initialize the pushbutton pin as an input
    pinMode(motorButton, INPUT); 
    pinMode(bluetoothButton, INPUT);
    pinMode(diodeTest, OUTPUT);
    pinMode(flashLightButton,INPUT);
    pinMode(flashLightLED,OUTPUT);    
    
    
    // Initialize bit rate for serial transmission
    Serial.begin(9600);
    //Serial1.begin(115200);
    
    
    // Attach the myservo motor to servoMotor
    // Initialize servo object and assign to pin 4 
    myservo.attach(servoMotor);
    // myservo.write(90);
}



//bluetooth test function
void bluetoothTest(){
    
    //check the state of bluetooth if high
    if(bluetoothState == HIGH){
      
        //set bluetooth to opposite value
        bluetoothOn = !bluetoothOn;
        
        //check if true
        if(bluetoothOn == true)
           //write to diode true, turns on diode 
           digitalWrite(diodeTest, HIGH);
        else
           //write to diode false, turns off diode
          digitalWrite(diodeTest, LOW);
       
       //delay for 5miliseconds to prevent fast turn on/off
       delay(500);
    }
    else{
       delay(50); 
    }
 bluetoothState = digitalRead(bluetoothButton);
}

// Method will handle the motor
void handleMotor(){
  // If the bluetooth switch is on, then we can start the motor
  while(servoMotorState == HIGH && bluetoothOn == true)
  {
    //Serial.println(bluetooth);
    // Read the output voltage value from potentiometer
    val1 = analogRead(analogPotentiometer);
    
    // Potentiiometer range from 0 to 179 degrees from the range 0 to 1023
    val1 = map(val1,0,1023,0,179);
    
    // Write the value into the motor
    myservo.write(val1);
   
    // Delay a little bit
    delay(200);
    
    
    
     // Update the states
     flashLightState = digitalRead(flashLightButton);
     bluetoothState = digitalRead(bluetoothButton);
     servoMotorState = digitalRead(motorButton);
     // Quit if bluetooth is turned off
    /*if(bluetoothState == HIGH){
      bluetoothOn = false;
      bluetoothState = LOW;
    }*/
      
      //call functions to check state
      bluetoothTest();  
      turnOnLight();
  }
  while(servoMotorState == LOW && bluetoothOn == true){
    
    //  Serial.println(servoMotorState);
    myservo.write(89);
    
    // Update the states
    bluetoothState = digitalRead(bluetoothButton);
    servoMotorState = digitalRead(motorButton);
    flashLightState = digitalRead(flashLightButton);

    
    // Quit if bluetooth is turned off
    /*if(bluetoothState == HIGH){
      bluetoothOn = false;
      bluetoothState = LOW;
    }*/

    //call function to check state
    bluetoothTest();
    turnOnLight();    
  }
}


void turnOnLight(){
 
 //check the state of the LED 
 if(flashLightState == HIGH){
  lightOn = !lightOn;
  
  if(lightOn == true)
    //digital write to digitalport turn on
    digitalWrite(flashLightLED,HIGH);  
  
  else
    //digital write to digitalport turn off
    digitalWrite(flashLightLED,LOW);
  
  //delay 
  delay(500);   
 }
 else{
  delay(50);
 }
 flashLightState = digitalRead(flashLightButton);
} 

void loop()
{
   // Check if the the bluetooth button is pressed for 
   // three seconds
   // bluetoothState = digitalRead(bluetoothButton);
   bluetoothState = digitalRead(bluetoothButton);
   servoMotorState = digitalRead(motorButton);
   flashLightState = digitalRead(flashLightButton);
   //call bluetooth test funciton
   bluetoothTest();
   
   turnOnLight();
  
   Serial.println("Outside Bluetooth is on");
   //if(bluetoothState == HIGH)
   //  bluetoothOn = true;

  // Call the method that will handle the motor
  //Serial1.print("Bluetoooth value is: ");
  //Serial1.println(bluetoothState);
  //Serial1.println(servoMotorState);
  
  // Only change the motor speed if the bluetooth is ON
  if(bluetoothOn == true){
    
   
    Serial.println("I am in bluetooth is on");
    //digitalWrite(diodeTest, HIGH);
    handleMotor();
  }
  
  // Make sure the motor is completely still when bluetooth is OFF
  else{
  //  digitalWrite(diodeTest, LOW);
    myservo.write(89); 
  }
}
