/*
 * RTC Periodic Trigger using Counter Method with Sine Wave Animation
 * 
 * This sketch demonstrates how to use the RTC periodic callback functionality to trigger
 * changes to a sine wave animation speed precisely at the beginning of each minute.
 * It also displays a second counter in the serial monitor.
 * 
 * Key Functions:
 * - minuteUpdate(): Changes the animation speed and prints status messages
 * - periodicCallback(): Called by the RTC every second
 * - showTimeDebug(): Displays current second count and animation speed
 * - updateAnimation(): Updates and displays the sine wave animation
 * 
 * Key Variables:
 * - phase: Current phase of the sine wave
 * - phaseIncrement: How fast the wave moves (changed every minute)
 * - secondFlag: Safe interrupt flag to trigger updates in the main loop
 * - secondCount: Counts seconds and triggers minuteUpdate when it reaches 60
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

// Variables for animation state and timing
float phase = 0.0; // Current phase of the sine wave
float phaseIncrement = 0.1; // How fast the wave moves (will change every minute)
unsigned long lastAnimUpdate = 0; // Time of last animation update
const unsigned long animUpdateInterval = 100; // Animation update interval in ms
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

  Serial.println("RTC Sine Wave Animation Speed Example");
  Serial.println("Method: Counter with Callback");

  // Set up the periodic callback to trigger every 1 second
  RTC.setPeriodicCallback(periodicCallback, Period::ONCE_EVERY_1_SEC);
  
  // Random seed from an unconnected analog pin
  randomSeed(analogRead(A0));
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
  
  // Update animation at regular intervals without using delay()
  unsigned long currentMillis = millis();
  if (currentMillis - lastAnimUpdate >= animUpdateInterval) {
    lastAnimUpdate = currentMillis;
    updateAnimation();
  }
  
  if(showDebug)
  {
    showTimeDebug();
  }
}

// Prints out current second count and animation speed
void showTimeDebug()
{
  // Check if second has changed
  if (secondCount != lastSecond)
  {
    Serial.print("DEBUG Time: 0:");
    Serial.print(secondCount);
    Serial.print(" | Speed: ");
    Serial.println(phaseIncrement, 3);

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
  // Change to a new random speed between 0.02 and 0.6
  phaseIncrement = 0.02 + (random(1, 59) / 100.0);
  
  // Print update message
  Serial.println("---------------------");
  Serial.println("MINUTE UPDATE");
  Serial.println("60 seconds have elapsed");
  Serial.print("New Animation Speed: ");
  Serial.println(phaseIncrement, 3);
  Serial.println("---------------------");
}

// Updates and displays the sine wave animation
void updateAnimation() 
{
  // Clear the display
  matrix.beginDraw();
  matrix.clear();
  
  // Set the drawing color
  matrix.stroke(0xFFFFFFFF);
  
  // Draw the sine wave
  for (int x = 0; x < 12; x++) {
    // Calculate y position using sine function
    // Map sine output (-1 to 1) to LED matrix coordinates
    float sinValue = sin(x * 0.5 + phase);
    int y = (int)(3.5 + 3.0 * sinValue); // Center at y=3.5, amplitude = 3
    
    // Make sure y stays within bounds of display (0-7)
    y = constrain(y, 0, 7);
    
    // Draw the point
    matrix.point(x, y);
  }
  
  // No text display on the matrix for cleaner visualization
  
  // Update the phase for animation
  phase += phaseIncrement;
  if (phase > 2 * PI) {
    phase -= 2 * PI; // Keep phase within reasonable range
  }
  
  // Show the frame
  matrix.endDraw();
}