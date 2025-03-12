/*
 * RTC Periodic Trigger using Counter Method
 * 
 * This sketch demonstrates how to use the RTC periodic callback functionality to trigger
 * actions precisely at the beginning of each minute using a second counter.
 * It focuses exclusively on the callback mechanism without using time get/set methods.
 * 
 * Key Functions:
 * - minuteUpdate(): Toggles the LED and prints status messages
 * - periodicCallback(): Called by the RTC every second
 * - updateSeconds(): Updates the second counter and its display
 * 
 * Key Variables:
 * - ledState: Tracks whether the LED is on or off
 * - secondFlag: Safe interrupt flag to trigger updates in the main loop
 * - secondCount: Counts seconds and triggers minuteUpdate when it reaches 60
 * 
 * Comparison with other methods:
 * - Alarm Method: Most precise timing for specific clock values but requires RTC time setup.
 * - Time Polling Method: Requires continuous time checking.
 * - Periodic Callback Method (this): Simple counter approach with regular interrupts.
 *   Works without needing to configure or read the actual RTC time.
 * 
 * To extend this code:
 * - Change the Period parameter to update at different frequencies
 * - Modify the secondCount threshold for different intervals
 * - Add additional actions to the minuteUpdate function
 */

// Include the RTC library
#include "RTC.h"

// Variables for LED state and timing
bool ledState = false;
volatile bool secondFlag = false;  // Flag for second update
int secondCount = 0;  // Counter for seconds

void setup() 
{
  // Initialize Serial Communication
  Serial.begin(9600);

  // Define LED as output
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize the RTC
  RTC.begin();

  Serial.println("RTC Periodic Trigger - Counter Method");
  Serial.println("Using callback only - no time get/set");

  // Set up the periodic callback to trigger every 1 second
  RTC.setPeriodicCallback(periodicCallback, Period::ONCE_EVERY_1_SEC);
}

void loop() 
{
  // Check if the second flag has been set
  if (secondFlag) 
  {
    secondFlag = false;  // Reset the flag
    updateSeconds();     // Call the second update function
  }
}

// This function is triggered by the RTC every second
void periodicCallback() 
{
  // Just set a flag - don't do any processing here
  secondFlag = true;
}

// Function for second updates - called every second
void updateSeconds() 
{
  // Increment the counter
  secondCount++;
  
  // Display counter and LED state
  Serial.print("Second counter: ");
  Serial.print(secondCount);
  Serial.print("/60");
  Serial.print(" | LED state: ");
  Serial.println(ledState ? "ON" : "OFF");
  
  // Check if a minute has passed
  if (secondCount >= 60) 
  {
    secondCount = 0;  // Reset the counter
    minuteUpdate();   // Call the minute update function
  }
}

// Common function for minute updates - called when counter reaches 60
void minuteUpdate() 
{
  // Toggle LED state
  ledState = !ledState;
  digitalWrite(LED_BUILTIN, ledState);
  
  // Print update message
  Serial.println("---------------------");
  Serial.println("MINUTE UPDATE");
  Serial.println("60 seconds have elapsed");
  Serial.print("LED switched to: ");
  Serial.println(ledState ? "ON" : "OFF");
  Serial.println("---------------------");
}