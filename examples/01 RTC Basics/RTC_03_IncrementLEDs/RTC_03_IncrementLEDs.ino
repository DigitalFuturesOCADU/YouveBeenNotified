/*
 * RTC Minute Tracker using LED Matrix
 * 
 * This sketch demonstrates how to use the RTC polling functionality to track
 * minutes by lighting up LEDs on the matrix one by one.
 * Each minute, a new LED lights up until the matrix is filled, then it resets.
 * 
 * Key Functions:
 * - minuteUpdate(): Lights up the next LED and prints status messages
 * - showTimeDebug(): Displays current second and LED position
 * - updateLED(): Lights up the current LED position on the matrix
 * 
 * Key Variables:
 * - ledX, ledY: Track the current LED position on the matrix
 * - lastMinute: Stores the last minute value to detect changes
 * - lastSecond: Stores the last second value to detect changes
 * 
 */

// Include the required libraries
#include "RTC.h"
// Include ArduinoGraphics BEFORE Arduino_LED_Matrix
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

// Create the matrix instance
ArduinoLEDMatrix matrix;

// Variables for Serial Debug
boolean showDebug = true;  // toggles whether or not to show the debug printout
int lastSecond = -1;       // holds the previous time value

// Variables for LED position tracking
int ledX = -1;              // X position of current LED (0-11)
int ledY = 0;              // Y position of current LED (0-7)
int currentMinute;         // stores the current minute from the RTC
int currentSecond;         // stores the current second from the RTC
int lastMinute = -1;       // Used to detect minute changes
int updateInterval = 1;    // Minutes between LED updates (can be changed)

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

  Serial.println("RTC Minute Tracker using LED Matrix Example");
  Serial.println("Method: Time Polling");
  
  // Clear the matrix and light the first LED
  matrix.beginDraw();
  matrix.clear();
  matrix.endDraw();
  
  // Update the initial LED
  updateLED();
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

// Prints out current minute/second values and LED position
void showTimeDebug()
{
  // Check if second has changed
  if (currentSecond != lastSecond)
  {
    Serial.print("DEBUG Time: ");
    Serial.print(currentMinute);
    Serial.print(":");
    Serial.print(currentSecond);
    Serial.print(" | LED Position: (");
    Serial.print(ledX);
    Serial.print(",");
    Serial.print(ledY);
    Serial.println(")");

    lastSecond = currentSecond;
  }
}

// Function for minute updates - called when minute changes
void minuteUpdate() 
{
  // Move to the next LED position
  ledX++;
  
  // If we reach the end of a row, move to the next row
  if (ledX >= matrix.width()) {
    ledX = 0;
    ledY++;
    
    // If we reach the end of all rows, reset to the beginning
    if (ledY >= matrix.height()) {
      // Reset to top-left when we've filled the matrix
      ledX = 0;
      ledY = 0;
      
      // Clear the matrix for a fresh start
      matrix.beginDraw();
      matrix.clear();
      matrix.endDraw();
    }
  }
  
  // Update the LED display
  updateLED();
  
  // Print update message
  Serial.println("---------------------");
  Serial.println("MINUTE UPDATE");
  Serial.print("Current Minute: ");
  Serial.println(currentMinute);
  Serial.print("LED Position: (");
  Serial.print(ledX);
  Serial.print(",");
  Serial.print(ledY);
  Serial.println(")");
  Serial.println("---------------------");
}

// Updates the LED matrix to show the current LED position
void updateLED() 
{
  matrix.beginDraw();
  
  // Turn on the current LED position
  matrix.stroke(0xFFFFFFFF); // Set color to "on"
  matrix.point(ledX, ledY);
  
  matrix.endDraw();
}