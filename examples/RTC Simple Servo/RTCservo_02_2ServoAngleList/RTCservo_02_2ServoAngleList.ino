/*
 * RTC Two Servo Controller
 * 
 * This sketch demonstrates how to use the RTC to control two servo motors,
 * moving them to predefined positions every minute. The display cycles through
 * showing the current minute, servo1 position, and servo2 position.
 * 
 * Key Functions:
 * - minuteUpdate(): Updates both servo positions based on the current minute
 * - showTimeDebug(): Displays current second count and servo values
 * - displayValues(): Cycles through a 5-frame display sequence (minute, "s1:", servo1 value, "s2:", servo2 value)
 * 
 * Key Variables:
 * - servoPositions1[]/servoPositions2[]: Two arrays of 10 predefined servo positions
 * - currentServoPosition1/2: Stores the current servo position values
 * - displayState: Controls which value is shown on the LED matrix (0=minute, 1=servo1, 2=servo2)
 * - lastMinute: Stores the last minute value to detect changes
 * 
 * Drawing to the Matrix
 * See this guide for drawing things to the matrix : https://github.com/DigitalFuturesOCADU/YouveBeenNotified/blob/main/ArduinoGraphics_R4.md
 */

// Include the required libraries
#include "RTC.h"
// Include ArduinoGraphics BEFORE Arduino_LED_Matrix
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include <Servo.h>

// Create the matrix instance
ArduinoLEDMatrix matrix;

// Create servo objects and define servo pins
Servo myServo1;
Servo myServo2;
const int servoPin1 = 9;  // Connect servo1 to pin 9
const int servoPin2 = 10; // Connect servo2 to pin 10

// Two arrays of 10 predefined servo positions (0-180 degrees)
// Note that positions 3 and 4 are the same (60 degrees) in the first array
int servoPositions1[] = {0, 20, 40, 60, 60, 90, 120, 140, 160, 180};
int servoPositions2[] = {180, 160, 140, 120, 100, 80, 60, 40, 20, 0}; // Second array with different values
int currentServoPosition1 = 0;  // Stores the current servo1 position value
int currentServoPosition2 = 0;  // Stores the current servo2 position value

// Variables for Serial Debug and Display
boolean showDebug = true;  // toggles whether or not to show the debug printout
int lastSecond = -1;       // holds the previous time value
int displayState = 0;      // 0 = minute, 1 = "s1:", 2 = servo1 value, 3 = "s2:", 4 = servo2 value
int displayToggleInterval = 1; // seconds between toggling the display

// Variables for counter state and timing
int currentMinute;         // stores the current minute from the RTC
int currentSecond;         // stores the current second from the RTC
int lastMinute = -1;       // Used to detect minute changes
int updateInterval = 1;    // Minutes between servo updates (can be changed)

void setup() 
{
  // Initialize Serial Communication
  Serial.begin(9600);

  // Initialize the LED matrix
  if (!matrix.begin()) {
    Serial.println("Failed to initialize LED matrix!");
    while (1);
  }

  // Initialize the RTC
  RTC.begin();

  // Set initial time
  RTCTime initialTime(04, Month::APRIL, 2025, 10, 0, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(initialTime);

  // Attach servos to their pins
  myServo1.attach(servoPin1);
  myServo2.attach(servoPin2);
  
  // Move servos to initial positions
  currentServoPosition1 = servoPositions1[0];
  currentServoPosition2 = servoPositions2[0];
  myServo1.write(currentServoPosition1);
  myServo2.write(currentServoPosition2);

  Serial.println("RTC Two Servo Controller");
  Serial.println("Method: Time Polling with Dual Servo Control");
  Serial.println("Displays: Cycling through Minute, Servo1, Servo2");
  
  // Display initial values
  displayValues();
}

void loop() 
{
  // Get current time
  RTCTime currentTime;
  RTC.getTime(currentTime);
  
  // Current values
  currentMinute = currentTime.getMinutes();
  currentSecond = currentTime.getSeconds();
  
  // Check if minute has changed
  if (currentMinute != lastMinute) 
  {
    // Update the stored minute
    lastMinute = currentMinute;
    
    // Trigger the minute update only if we've reached a multiple of updateInterval
    // The default is every minute, but it can be changed with the variable
    if (currentMinute % updateInterval == 0) {
      minuteUpdate();
    }
  }
  
  if(showDebug)
  {
    showTimeDebug();
  }
}

// Prints out current minute/second and servo values
// Also handles cycling the display through minute, servo1, and servo2
void showTimeDebug()
{
  // Check if second has changed
  if (currentSecond != lastSecond)
  {
    Serial.print("DEBUG Time: ");
    Serial.print(currentMinute);
    Serial.print(":");
    Serial.print(currentSecond);
    Serial.print(" | Servo1: ");
    Serial.print(currentServoPosition1);
    Serial.print(" | Servo2: ");
    Serial.print(currentServoPosition2);
    Serial.print(" | Display: ");
    
    switch(displayState) {
      case 0:
        Serial.println("Minute");
        break;
      case 1:
        Serial.println("s1: label");
        break;
      case 2:
        Serial.println("Servo1 Value");
        break;
      case 3:
        Serial.println("s2: label");
        break;
      case 4:
        Serial.println("Servo2 Value");
        break;
    }

    // Update the last second
    lastSecond = currentSecond;
    
    // Toggle the display every displayToggleInterval seconds
    if (currentSecond % displayToggleInterval == 0) {
      // Cycle through display states (0=minute, 1=s1:, 2=servo1 value, 3=s2:, 4=servo2 value)
      displayState = (displayState + 1) % 5;
      displayValues();   // Update the display
    }
  }
}

// Updates both servo positions based on the current minute
void minuteUpdate() 
{
  // Calculate the position index (0-9) based on minute
  // Using modulo 10 (%) because we have 10 positions in our arrays (indexes 0-9)
  // This creates a repeating pattern: minutes 0,10,20,30,40,50 use position[0],
  // minutes 1,11,21,31,41,51 use position[1], and so on
  int positionIndex = (currentMinute % 10);
  
  // Update the servo positions
  currentServoPosition1 = servoPositions1[positionIndex];
  currentServoPosition2 = servoPositions2[positionIndex];
  
  // Move the servos
  myServo1.write(currentServoPosition1);
  myServo2.write(currentServoPosition2);
  
  // Display the updated values
  displayValues();
  
  // Print update message
  Serial.println("---------------------");
  Serial.println("MINUTE UPDATE");
  Serial.print("Current Minute: ");
  Serial.println(currentMinute);
  Serial.print("Position Index: ");
  Serial.println(positionIndex);
  Serial.print("Servo1 Position (degrees): ");
  Serial.println(currentServoPosition1);
  Serial.print("Servo2 Position (degrees): ");
  Serial.println(currentServoPosition2);
  Serial.println("---------------------");
}

// Cycles through a 5-frame display sequence: 
// minute, "s1:", servo1 value, "s2:", servo2 value
void displayValues() 
{
  matrix.beginDraw();
  matrix.clear(); // Clear the previous display
  
  // Set text properties
  matrix.stroke(0xFFFFFFFF); // Set text color to white (on)
  matrix.textFont(Font_4x6);  // Use the smaller font as requested
  
  // Determine which value to display based on the display state
  switch(displayState) {
    case 0:
      // Display minute with 'm' suffix
      matrix.text(String(currentMinute) + "m", 0, 0);
      break;
    case 1:
      // Display "s1:" label
      matrix.text("s1:", 0, 0);
      break;
    case 2:
      // Display servo1 value only
      matrix.text(String(currentServoPosition1), 0, 0);
      break;
    case 3:
      // Display "s2:" label
      matrix.text("s2:", 0, 0);
      break;
    case 4:
      // Display servo2 value only
      matrix.text(String(currentServoPosition2), 0, 0);
      break;
  }
  
  matrix.endDraw();
}