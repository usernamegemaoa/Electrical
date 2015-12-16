// Basic bluetooth test sketch 3a. AT mode using button switch
// HC-05 with EN pin and button switch
//
// Uses normal serial with the host computer plus software serial for communication with the bluetooth module
// The software serial library has to be placed inside the Arduino folder.
//
//  Pins
//  BT VCC to Arduino 5V out. Disconnect before running the sketch
//  BT GND to GND
//  BT RX (through a voltage divider) to Arduino pin 11
//  BT TX  to Arduino pin 10 (no need voltage divider)
//  BT EN to 3.3V (5V through a voltage divider)
//
// When a command is entered in to the serial monitor on the computer 
// the Arduino will relay it to the bluetooth module and display the result.
//
 
 
char serialByte = '0';
const byte  LEDPIN = 13; 
 
#include <SoftwareSerial.h>
SoftwareSerial BTserial(10,11); // RX | TX
 
void setup() 
{
    pinMode(LEDPIN, OUTPUT);
 
    // communication with the host computer
    Serial.begin(9600);     
 
    // Start the software serial - baud rate for AT mode is 38400 
    Serial1.begin(38400); 
 
    // LED to show we have started the serial channels
    digitalWrite(LEDPIN, HIGH);  
 
    // Give the user time to enter AT mode
    // wait for the user to enter a '1' to start
    Serial.println("After entering AT mode, type 1");
    while (serialByte !='1')
    {
       serialByte = Serial.read();
    }   
 
    Serial.println(" ");
    Serial.println("AT mode.");
    Serial.println("Remember to to set Both NL & CR in the serial monitor.");
    Serial.println(" ");  
    Serial.print("BT STATE = ");
    BTserial.println("AT+STATE" );
    delay(10000);
 
}
 
 
void loop() 
{
    // listen for communication from the BT module and then write it to the serial monitor
    if ( BTserial.available() )   {  Serial.write( BTserial.read() );  }
 
    // listen for user input and send it to the HC-05
   if ( Serial.available() )   {  BTserial.write( Serial.read() );  }
}
