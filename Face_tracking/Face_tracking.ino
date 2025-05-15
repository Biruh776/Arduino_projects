#include <VarSpeedServo.h>

// Create two servo objects to control two servos independently
VarSpeedServo servo1; 
VarSpeedServo servo2;

String inputString = "";         // A string to hold incoming serial data
unsigned int cont = 0;           // A counter to keep track of 'o' commands received

void setup() 
{
  // Attach servo1 to pin 9
  servo1.attach(9);
  
  // Attach servo2 to pin 10
  servo2.attach(10);

  // Initialize serial communication at 250000 baud rate
  Serial.begin(250000);
  
  // Print "Ready" to indicate setup is complete and program is ready
  Serial.println("Ready");
}

void loop() 
{
  signed int vel;   // Variable to hold velocity parsed from input
  unsigned int pos; // Variable to hold the current position of a servo
  
  // Check if there is incoming serial data available
  if (Serial.available()) 
  {
    // Read the incoming data until the delimiter '!' is found
    inputString = Serial.readStringUntil('!');
    
    // Convert the numeric part of the inputString to an integer (velocity)
    vel = inputString.toInt();   
    
    // Check which servo the command is for based on the last character in the input string
    if(inputString.endsWith("x"))
    {
      // Control servo1 based on velocity value
      if (vel > 2)
        servo1.write(180, vel, false);   // Move servo1 to 180 degrees at speed 'vel'
      else if (vel < -2)
        servo1.write(0, -vel, false);    // Move servo1 to 0 degrees at speed 'abs(vel)'
      else
      {
        // If velocity is small, keep servo1 at its current position immediately
        pos = servo1.read();
        servo1.write(pos, 255, false);       
      } 
    }
    else if(inputString.endsWith("y"))
    {
      // Control servo2 based on velocity value
      if (vel > 2)
        servo2.write(180, vel, false);   // Move servo2 to 180 degrees at speed 'vel'
      else if (vel < -2)
        servo2.write(0, -vel, false);    // Move servo2 to 0 degrees at speed 'abs(vel)'
      else
      {
        // If velocity is small, keep servo2 at its current position immediately
        pos = servo2.read();
        servo2.write(pos, 255, false);       
      } 
    }
    else if(inputString.endsWith("o"))
    {
      // Increment the counter each time an 'o' command is received
      cont++;
      
      if (cont >= 100)
      {
        // After 100 'o' commands, reset both servos to preset positions slowly and smoothly
        pos = servo1.read();
        servo1.write(90, 20, true);       // Move servo1 to 90 degrees at speed 20, wait for completion
        
        pos = servo2.read();
        servo2.write(70 , 20, true);      // Move servo2 to 70 degrees at speed 20, wait for completion
        
        // Reset the counter
        cont = 0;
      }
      else
      {
        // If count < 100, keep both servos at their current positions immediately
        pos = servo1.read();
        servo1.write(pos, 255, false);        
        
        pos = servo2.read();
        servo2.write(pos, 255, false);
      }       
    }
    
    // Clear the input string to prepare for next command
    inputString = "";
  }
}
