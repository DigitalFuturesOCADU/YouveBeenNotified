/*
 * RTC Minute Trigger using Polling Method
 * 
 * This sketch demonstrates how to use the RTC polling functionality to trigger
 * actions at the beginning of each minute by regularly checking the current time.
 * It also displays a second counter in the serial monitor using RTC second values.
 * 
 * Key Functions:
 * - minuteUpdate(): Toggles the LED and prints status messages
 * - checkTimeAndUpdate(): Checks if minute or second has changed and triggers updates
 * 
 * Key Variables:
 * - ledState: Tracks whether the LED is on or off
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
 * 
 * To extend this code:
 * - Modify the polling conditions to trigger on different time changes
 * - Add additional actions to the minuteUpdate function
 */

// Include the RTC library
#include "RTC.h"

// Variables for timing and LED state
bool ledState = false;
int lastMinute = -1;
int lastSecond = -1;

void setup() 
{
  // Initialize Serial Communication
  Serial.begin(9600);

  // Define LED as output
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize the RTC
  RTC.begin();

  // Set initial time
  RTCTime initialTime(14, Month::MARCH, 2025, 9, 0, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(initialTime);

  Serial.println("RTC Minute Trigger - Polling Method");
}

void loop() 
{
  // Check time and update if the minute or second has changed
  checkTimeAndUpdate();
  
  // Small delay to prevent hammering the RTC with constant polling
  delay(50);
}

// Function to check time and trigger minute or second updates if needed
void checkTimeAndUpdate() 
{
  // Get current time
  RTCTime currentTime;
  RTC.getTime(currentTime);
  
  // Current values
  int currentMinute = currentTime.getMinutes();
  int currentSecond = currentTime.getSeconds();
  
  // Check if minute has changed
  if (currentMinute != lastMinute) 
  {
    // Update the stored minute
    lastMinute = currentMinute;
    
    // Trigger the minute update
    minuteUpdate();
  }
  
  // Check if second has changed
  if (currentSecond != lastSecond)
  {
    // Update the stored second
    lastSecond = currentSecond;
    
    // Display seconds counting
    Serial.print("Time: ");
    Serial.print(currentTime.getHour());
    Serial.print(":");
    Serial.print(currentMinute);
    Serial.print(":");
    Serial.println(currentSecond);
    Serial.print("LED state: ");
    Serial.println(ledState ? "ON" : "OFF");
  }
}

// Common function for minute updates - called when minute changes
void minuteUpdate() 
{
  // Toggle LED state
  ledState = !ledState;
  digitalWrite(LED_BUILTIN, ledState);
  
  // Get current time for display
  RTCTime currentTime;
  RTC.getTime(currentTime);
  
  // Print update message
  Serial.println("---------------------");
  Serial.println("MINUTE UPDATE");
  Serial.print("Current time: ");
  Serial.print(currentTime.getHour());
  Serial.print(":");
  Serial.print(currentTime.getMinutes());
  Serial.print(":");
  Serial.println(currentTime.getSeconds());
  Serial.print("LED switched to: ");
  Serial.println(ledState ? "ON" : "OFF");
  Serial.println("---------------------");
}