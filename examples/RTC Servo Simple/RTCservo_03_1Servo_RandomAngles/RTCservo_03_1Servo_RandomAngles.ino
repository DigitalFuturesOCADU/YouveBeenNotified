/*
 * RTC Random Servo Controller
 * 
 * This sketch demonstrates how to use the RTC to control a servo motor,
 * moving it to a random position every minute. The display toggles between
 * showing the current minute and servo position.
 * 
 * Key Functions:
 * - minuteUpdate(): Updates the servo position to a random value
 * - showTimeDebug(): Displays current second count and servo value
 * - displayMinuteAndServo(): Toggles between showing minute or servo value on the LED matrix
 * 
 * Key Variables:
 * - minServoAngle/maxServoAngle: Range limits for the random servo position
 * - currentServoPosition: Stores the current servo position value
 * - showMinute: Boolean to toggle between showing minute vs servo angle
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

// Create servo object and define servo pin
Servo myServo;
const int servoPin = 9;  // Connect servo to pin 9

// Servo position range limits
int minServoAngle = 0;   // Minimum servo angle
int maxServoAngle = 180; // Maximum servo angle
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

  // Initialize random seed using an analog read
  randomSeed(analogRead(0));
  
  // Attach servo to the servo pin
  myServo.attach(servoPin);
  
  // Move servo to initial random position
  currentServoPosition = random(minServoAngle, maxServoAngle + 1);
  myServo.write(currentServoPosition);

  Serial.println("RTC Random Servo Controller");
  Serial.println("Method: Time Polling with Random Servo Control");
  Serial.println("Displays: Minute and Servo Position (toggling)");
  
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

// Updates servo position to a random value - called when minute changes
void minuteUpdate() 
{
  // Generate a random servo position within the defined range
  currentServoPosition = random(minServoAngle, maxServoAngle + 1);
  
  // Move the servo to the new position
  myServo.write(currentServoPosition);
  
  // Display the counter based on direction
  displayMinuteAndServo();
  
  // Print update message
  Serial.println("---------------------");
  Serial.println("MINUTE UPDATE");
  Serial.print("Current Minute: ");
  Serial.println(currentMinute);
  Serial.print("New Random Servo Position: ");
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