/*
 * RTC Periodic Trigger using Counter Method with LED Matrix Shapes
 * 
 * This sketch demonstrates how to use the RTC periodic callback functionality to trigger
 * shape changes on the LED matrix precisely at the beginning of each minute using a second counter.
 * It cycles through different shapes and displays a second counter in the serial monitor.
 * 
 * Key Functions:
 * - minuteUpdate(): Changes the displayed shape and prints status messages
 * - periodicCallback(): Called by the RTC every second
 * - showTimeDebug(): Displays current second count and shape index
 * - displayShape(): Draws the current shape on the LED matrix
 * 
 * Key Variables:
 * - shapeIndex: Tracks which shape is currently displayed
 * - secondFlag: Safe interrupt flag to trigger updates in the main loop
 * - secondCount: Counts seconds and triggers minuteUpdate when it reaches 60
 * 
 * Comparison with other methods:
 * - Alarm Method: Most precise timing for specific clock values but requires RTC time setup.
 * - Time Polling Method: Requires continuous time checking.
 * - Periodic Callback Method (this): Simple counter approach with regular interrupts.
 *   Works without needing to configure or read the actual RTC time.
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
volatile bool secondFlag = false;  // Flag for second update
int secondCount = 0;  // Counter for seconds

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

  Serial.println("RTC Periodic Trigger with LED Matrix Shapes Example");
  Serial.println("Method: Counter with Callback");

  // Set up the periodic callback to trigger every 1 second
  RTC.setPeriodicCallback(periodicCallback, Period::ONCE_EVERY_1_SEC);
  
  // Display initial shape
  displayShape();
}

void loop() 
{
  // Check if the second flag has been set
  if (secondFlag) 
  {
    secondFlag = false;  // Reset the flag
    secondCount++;       // Increment the counter
    
    // Check if a minute has passed
    if (secondCount >= 60) 
    {
      secondCount = 0;    // Reset the counter
      minuteUpdate();     // Call the minute update function
    }
  }
  
  if(showDebug)
  {
    showTimeDebug();
  }
}

// Prints out current second count and shape index
void showTimeDebug()
{
  // Check if second has changed
  if (secondCount != lastSecond)
  {
    Serial.print("DEBUG Time: 0:");
    Serial.print(secondCount);
    Serial.print(" | Shape Index: ");
    Serial.println(shapeIndex);

    lastSecond = secondCount;
  }
}

// This function is triggered by the RTC every second
void periodicCallback() 
{
  // Just set a flag - don't do any processing here
  secondFlag = true;
}

// Common function for minute updates - called when counter reaches 60
void minuteUpdate() 
{
  // Change to the next shape
  shapeIndex = (shapeIndex + 1) % 5; // Cycle through 5 shapes (0-4)
  
  // Display the new shape
  displayShape();
  
  // Print update message
  Serial.println("---------------------");
  Serial.println("MINUTE UPDATE");
  Serial.println("60 seconds have elapsed");
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