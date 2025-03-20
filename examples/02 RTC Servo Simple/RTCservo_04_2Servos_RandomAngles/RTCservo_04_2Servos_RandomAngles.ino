/*
 * RTC Two Random Servo Controller
 * 
 * This sketch demonstrates how to use the RTC to control two servo motors,
 * moving them to random positions every minute. The display cycles through
 * showing the current minute, servo1 position, and servo2 position.
 * 
 * Key Functions:
 * - minuteUpdate(): Updates both servo positions to random values
 * - showTimeDebug(): Displays current second count and servo values
 * - displayValues(): Cycles through a 5-frame display sequence
 * 
 * Key Variables:
 * - minServoAngle/maxServoAngle: Range limits for random servo positions
 * - currentServoPosition1/2: Stores the current servo position values
 * - displayState: Controls which value is shown on the LED matrix
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

// Servo position range limits
int minServoAngle = 0;   // Minimum servo angle
int maxServoAngle = 180; // Maximum servo angle
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

  // Initialize random seed using an analog read
  randomSeed(analogRead(0));
  
  // Attach servos to their pins
  myServo1.attach(servoPin1);
  myServo2.attach(servoPin2);
  
  // Move servos to initial random positions
  currentServoPosition1 = random(minServoAngle, maxServoAngle + 1);
  currentServoPosition2 = random(minServoAngle, maxServoAngle + 1);
  myServo1.write(currentServoPosition1);
  myServo2.write(currentServoPosition2);

  Serial.println("RTC Two Random Servo Controller");
  Serial.println("Method: Time Polling with Dual Random Servo Control");
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
// Also handles cycling the display through the 5-frame sequence
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

// Updates both servo positions to random values - called when minute changes
void minuteUpdate() 
{
  // Generate random positions for both servos
  currentServoPosition1 = random(minServoAngle, maxServoAngle + 1);
  currentServoPosition2 = random(minServoAngle, maxServoAngle + 1);
  
  // Move the servos to new positions
  myServo1.write(currentServoPosition1);
  myServo2.write(currentServoPosition2);
  
  // Display the updated values
  displayValues();
  
  // Print update message
  Serial.println("---------------------");
  Serial.println("MINUTE UPDATE");
  Serial.print("Current Minute: ");
  Serial.println(currentMinute);
  Serial.print("New Random Servo1 Position: ");
  Serial.println(currentServoPosition1);
  Serial.print("New Random Servo2 Position: ");
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