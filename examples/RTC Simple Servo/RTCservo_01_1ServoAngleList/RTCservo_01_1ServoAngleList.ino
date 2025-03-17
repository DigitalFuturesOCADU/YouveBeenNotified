/*
 * RTC Servo Controller
 * 
 * This sketch demonstrates how to use the RTC to control a servo motor,
 * moving it to predefined positions every minute. The current minute and 
 * servo position are displayed on the LED matrix.
 * 
 * Key Functions:
 * - minuteUpdate(): Updates the servo position based on the current minute
 * - showTimeDebug(): Displays current second count and servo value
 * - displayMinuteAndServo(): Toggles between showing minute (with 'm' suffix) or servo angle (with 'd' suffix)
 * 
 * Key Variables:
 * - servoPositions[]: Array of 10 predefined servo positions (0-180 degrees)
 * - currentServoPosition: Stores the current servo position value
 * - lastMinute: Stores the last minute value to detect changes
 * - lastSecond: Stores the last second value to detect changes
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

// Create servo object and define servo pin
Servo myServo;
int servoPin = 9;  // Connect servo to pin 9

// Array of 10 predefined servo positions (0-180 degrees)
// Note that positions 3 and 4 are the same (60 degrees)
int servoPositions[] = {0, 20, 40, 60, 60, 90, 120, 140, 160, 180};
int currentServoPosition = 0;  // Stores the current servo position value

// Variables for Serial Debug and Display
boolean showDebug = true;  // toggles whether or not to show the debug printout
int lastSecond = -1;       // holds the previous time value
boolean showMinute = true; // when true, shows minute; when false, shows servo angle
int displayToggleInterval = 2; // seconds between toggling the display

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

  // Attach servo to the servo pin
  myServo.attach(servoPin);
  
  // Move servo to initial position
  currentServoPosition = servoPositions[0];
  myServo.write(currentServoPosition);

  Serial.println("RTC Servo Controller");
  Serial.println("Method: Time Polling with Servo Control");
  Serial.println("Displays: Minute,Servo Position");
  
  // Display initial values
  displayMinuteAndServo();
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
// Also handles toggling the display between minute and servo angle
void showTimeDebug()
{
  // Check if second has changed
  if (currentSecond != lastSecond)
  {
    Serial.print("DEBUG Time: ");
    Serial.print(currentMinute);
    Serial.print(":");
    Serial.print(currentSecond);
    Serial.print(" | Servo Position: ");
    Serial.print(currentServoPosition);
    Serial.print(" | Display Mode: ");
    Serial.println(showMinute ? "Minute" : "Servo Angle");

    // Update the last second
    lastSecond = currentSecond;
    
    // Toggle the display every displayToggleInterval seconds
    if (currentSecond % displayToggleInterval == 0) {
      showMinute = !showMinute;  // Toggle between showing minute and servo angle
      displayMinuteAndServo();   // Update the display
    }
  }
}

// Updates servo position based on the current minute
void minuteUpdate() 
{
  // Calculate the servo position index (0-9) based on minute
  // Using modulo 10 (%) because we have 10 positions in our array (indexes 0-9)
  // This creates a repeating pattern: minutes 0,10,20,30,40,50 use position[0],
  // minutes 1,11,21,31,41,51 use position[1], and so on
  int positionIndex = (currentMinute % 10);
  
  // Update the servo position
  currentServoPosition = servoPositions[positionIndex];
  myServo.write(currentServoPosition);
  
  // Display the current minute and servo position
  displayMinuteAndServo();
  
  // Print update message
  Serial.println("---------------------");
  Serial.println("MINUTE UPDATE");
  Serial.print("Current Minute: ");
  Serial.println(currentMinute);
  Serial.print("Servo Position Index: ");
  Serial.println(positionIndex);
  Serial.print("Servo Position (degrees): ");
  Serial.println(currentServoPosition);
  Serial.println("---------------------");
}

// Displays either the minute (with 'm') or servo position (with 'd') on the LED matrix
void displayMinuteAndServo() 
{
  matrix.beginDraw();
  matrix.clear(); // Clear the previous display
  
  // Set text properties
  matrix.stroke(0xFFFFFFFF); // Set text color to white (on)
  matrix.textFont(Font_4x6);  // Use the smaller font as requested
  
  // Determine which value to display based on the toggle
  if (showMinute) {
    // Display minute with 'm' suffix
    matrix.text(String(currentMinute) + "m", 0, 0);
  } else {
    // Display servo position with 'd' suffix
    matrix.text(String(currentServoPosition) + "d", 0, 0);
  }
  
  matrix.endDraw();
}