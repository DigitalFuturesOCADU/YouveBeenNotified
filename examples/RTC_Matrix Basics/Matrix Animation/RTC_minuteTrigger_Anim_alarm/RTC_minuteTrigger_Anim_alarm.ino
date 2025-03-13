/*
 * RTC Minute Trigger using Alarm Method with Sine Wave Animation
 * 
 * This sketch demonstrates how to use the RTC alarm functionality to trigger
 * changes to a sine wave animation speed precisely at the beginning of each minute.
 * It also displays a second counter in the serial monitor.
 * 
 * Key Functions:
 * - minuteUpdate(): Changes the animation speed and prints status messages
 * - alarmCallback(): Called by the RTC when seconds are zero
 * - updateAnimation(): Updates and displays the sine wave animation
 * 
 * Key Variables:
 * - phase: Current phase of the sine wave
 * - phaseIncrement: How fast the wave moves (changed every minute)
 * - minuteFlag: Safe interrupt flag to trigger updates in the main loop
 */

// Include the required libraries
#include "RTC.h"
// Include ArduinoGraphics BEFORE Arduino_LED_Matrix
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

// Create the matrix instance
ArduinoLEDMatrix matrix;

// Variables for Serial Debug
boolean showDebug = true; // toggles whether or not to show the debug printout
int lastSecond = -1; // holds the previous time value 

// Variables for animation state and timing
volatile bool minuteFlag = false; // Flag for minute update
float phase = 0.0; // Current phase of the sine wave
float phaseIncrement = 0.1; // How fast the wave moves (will change every minute)
unsigned long lastAnimUpdate = 0; // Time of last animation update
const unsigned long animUpdateInterval = 100; // Animation update interval in ms

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

  // Set initial time (needed for alarm to work)
  RTCTime initialTime(14, Month::MARCH, 2025, 0, 0, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(initialTime);

  Serial.println("RTC Sine Wave Animation Speed Example");
  Serial.println("Method: Alarm Interrupt");

  // Trigger the alarm every time the seconds are zero
  RTCTime alarmTime;
  alarmTime.setSecond(0);

  // Make sure to only match on the seconds
  AlarmMatch matchTime;
  matchTime.addMatchSecond();

  // Set the alarm callback
  RTC.setAlarmCallback(alarmCallback, alarmTime, matchTime);
  
  // Random seed from an unconnected analog pin
  randomSeed(analogRead(A0));
}

void loop() 
{
  // Check if the minute flag has been set
  if (minuteFlag) 
  {
    minuteFlag = false;  // Reset the flag
    minuteUpdate();      // Call the minute update function
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

// Prints out current minute/second values
void showTimeDebug()
{
  // Get current time for second display
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
    Serial.print(" | Speed: ");
    Serial.println(phaseIncrement, 3);

    lastSecond = currentSecond;
  }
}

// This function is triggered by the RTC alarm
void alarmCallback() 
{
  // Just set a flag - don't do any processing here
  minuteFlag = true;
}

// Common function for minute updates - called from the main loop
void minuteUpdate() 
{
  // Change to a new random speed between 0.02 and 0.6
  phaseIncrement = 0.02 + (random(1, 59) / 100.0);
  
  // Get current time for display
  RTCTime currentTime;
  RTC.getTime(currentTime);
  
  // Print update message
  Serial.println("---------------------");
  Serial.println("MINUTE UPDATE");
  Serial.print("Current Minute: ");
  Serial.println(currentTime.getMinutes());
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