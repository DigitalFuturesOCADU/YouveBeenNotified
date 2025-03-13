/*
 * RTC Minute Trigger using Polling Method with LED Matrix Shapes
 * 
 * This sketch demonstrates how to use the RTC polling functionality to trigger
 * shape changes on the LED matrix at the beginning of each minute.
 * It cycles through different shapes and displays a second counter in the serial monitor.
 * 
 * Key Functions:
 * - minuteUpdate(): Changes the displayed shape and prints status messages
 * - showTimeDebug(): Displays current second count and shape index
 * - displayShape(): Draws the current shape on the LED matrix
 * 
 * Key Variables:
 * - shapeIndex: Tracks which shape is currently displayed
 * - lastMinute: Stores the last minute value to detect changes
 * - lastSecond: Stores the last second value to detect changes
 * 
 * Comparison with other methods:
 * - Alarm Method: Most precise timing for specific clock values. Triggers exactly 
 *   at the beginning of each minute with minimal code. Good for exact timing requirements.
 * - Time Polling Method (this): Continuously checks time, which uses more CPU but is flexible 
 *   for detecting various time patterns. Works even if RTC doesn't support alarms.
 * - Periodic Callback Method: Uses a counter approach with regular interrupts. Good for 
 *   custom intervals that don't align with clock values, but may drift over time.
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

// Variables for shape state and timing
int shapeIndex = 0;        // Track which shape to display
int lastMinute = -1;       // Used to detect minute changes

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
  RTCTime initialTime(14, Month::MARCH, 2025, 0, 0, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(initialTime);

  Serial.println("RTC Minute Trigger with LED Matrix Shapes Example");
  Serial.println("Method: Time Polling");
  
  // Display initial shape
  displayShape();
}

void loop() 
{
  // Get current time
  RTCTime currentTime;
  RTC.getTime(currentTime);
  
  // Current values
  int currentMinute = currentTime.getMinutes();
  
  // Check if minute has changed
  if (currentMinute != lastMinute) 
  {
    // Update the stored minute
    lastMinute = currentMinute;
    
    // Trigger the minute update
    minuteUpdate();
  }
  
  if(showDebug)
  {
    showTimeDebug();
  }
}

// Prints out current minute/second values
void showTimeDebug()
{
  // Get current time
  RTCTime currentTime;
  RTC.getTime(currentTime);
  
  // Current values
  int currentMinute = currentTime.getMinutes();
  int currentSecond = currentTime.getSeconds();

  // Check if second has changed
  if (currentSecond != lastSecond)
  {
    Serial.print("DEBUG Time: ");
    Serial.print(currentMinute);
    Serial.print(":");
    Serial.print(currentSecond);
    Serial.print(" | Shape Index: ");
    Serial.println(shapeIndex);

    lastSecond = currentSecond;
  }
}

// Common function for minute updates - called when minute changes
void minuteUpdate() 
{
  // Change to the next shape
  shapeIndex = (shapeIndex + 1) % 5; // Cycle through 5 shapes (0-4)
  
  // Display the new shape
  displayShape();
  
  // Get current time for display
  RTCTime currentTime;
  RTC.getTime(currentTime);
  
  // Print update message
  Serial.println("---------------------");
  Serial.println("MINUTE UPDATE");
  Serial.print("Current Minute: ");
  Serial.println(currentTime.getMinutes());
  Serial.print("Current Shape: ");
  Serial.println(shapeIndex);
  Serial.println("---------------------");
}

// Displays the current shape based on shapeIndex
void displayShape() 
{
  matrix.beginDraw();
  matrix.clear(); // Clear the previous display
  
  // Set stroke to "on"
  matrix.stroke(0xFFFFFFFF);
  
  switch (shapeIndex) {
    case 0:
      // Rectangle outline
      matrix.noFill();
      matrix.rect(1, 1, 10, 6);
      break;
      
    case 1:
      // Filled rectangle
      matrix.fill(0xFFFFFFFF);
      matrix.rect(2, 2, 8, 4);
      break;
      
    case 2:
      // Circle outline
      matrix.noFill();
      matrix.circle(5, 3, 5);
      break;
      
    case 3:
      // Filled circle
      matrix.fill(0xFFFFFFFF);
      matrix.circle(5, 3, 4);
      break;
      
    case 4:
      // Crossed lines
      matrix.line(0, 0, 11, 7); // Diagonal line
      matrix.line(0, 7, 11, 0); // Crossed diagonal
      break;
  }
  
  // Add the shape number as text
  matrix.textFont(Font_4x6); // Use the smaller font
  matrix.stroke(0xFFFFFFFF); // Ensure text is visible
  
  // Display the shape number in the corner
  matrix.text(String(shapeIndex), 0, 0);
  
  matrix.endDraw();
}