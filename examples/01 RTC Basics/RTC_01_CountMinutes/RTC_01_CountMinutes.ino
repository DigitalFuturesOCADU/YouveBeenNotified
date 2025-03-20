/*
 * RTC Minute Counter with Bidirectional Counting
 * 
 * This sketch demonstrates how to use the RTC polling functionality to count
 * and display minutes on the LED matrix. It can count either up to 60 or down from 60.
 * 
 * Key Functions:
 * - minuteUpdate(): Updates the counter based on count direction
 * - showTimeDebug(): Displays current second count and counter value
 * - displayMinute(): Draws the current minute value on the LED matrix
 * 
 * Key Variables:
 * - countDirection: Determines counting direction (0 = up to 60, 1 = down from 60)
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

// Create the matrix instance
ArduinoLEDMatrix matrix;

// Variables for Serial Debug
boolean showDebug = true;  // toggles whether or not to show the debug printout
int lastSecond = -1;       // holds the previous time value

// Variables for counter state and timing
int countDirection = 1;    // 0 = count up to 60, 1 = count down from 60
int currentMinute;         // stores the current minute from the RTC
int currentSecond;         // stores the current second from the RTC
int lastMinute = -1;       // Used to detect minute changes
int updateInterval = 1;    // Minutes between counter updates (can be changed)

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

  Serial.println("RTC Minute Counter with Bidirectional Counting");
  Serial.println("Method: Time Polling");
  
  if (countDirection == 0) {
    Serial.println("Count Direction: UP (0 to 60)");
  } else {
    Serial.println("Count Direction: DOWN (60 to 0)");
  }
  
  // Display initial counter value
  displayMinute();
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

// Prints out current minute/second values
void showTimeDebug()
{
  // Check if second has changed
  if (currentSecond != lastSecond)
  {
    Serial.print("DEBUG Time: ");
    Serial.print(currentMinute);
    Serial.print(":");
    Serial.println(currentSecond);

    lastSecond = currentSecond;
  }
}

// Common function for minute updates - called when minute changes
void minuteUpdate() 
{
  // Display the counter based on direction
  displayMinute();
  
  // Print update message
  Serial.println("---------------------");
  Serial.println("MINUTE UPDATE");
  Serial.print("Current Minute: ");
  Serial.println(currentMinute);
  
  // Show display value based on count direction
  if (countDirection == 0) {
    Serial.print("Counting UP: ");
    Serial.println(currentMinute);
  } else {
    Serial.print("Counting DOWN: ");
    Serial.println(60 - currentMinute);
  }
  
  Serial.println("---------------------");
}

// Displays the current minute value on the LED matrix
void displayMinute() 
{
  matrix.beginDraw();
  matrix.clear(); // Clear the previous display
  
  // Set text properties
  matrix.stroke(0xFFFFFFFF); // Set text color to white (on)
  matrix.textFont(Font_5x7);  // Use the larger font for better visibility
  
  // Get the value to display based on count direction
  String displayValue;
  
  if (countDirection == 0) {
    // Count up - display current minute
    displayValue = String(currentMinute);
  } else {
    // Count down - display 60 minus current minute
    displayValue = String(60 - currentMinute);
  }
  
  // Calculate position to center the digits
  // For two digits, we need to adjust the positioning
  int xPos;
  if (displayValue.length() > 1) {
    xPos = (matrix.width() - (matrix.textFontWidth() * 2)) / 2;
  } else {
    xPos = (matrix.width() - matrix.textFontWidth()) / 2;
  }
  
  int yPos = (matrix.height() - matrix.textFontHeight()) / 2;
  
  // Display the counter value
  matrix.text(displayValue, xPos, yPos);
  
  matrix.endDraw();
}