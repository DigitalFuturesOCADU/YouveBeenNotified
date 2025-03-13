/*
 * RTC Minute Trigger using Alarm Method
 * 
 * This sketch demonstrates how to use the RTC alarm functionality to trigger
 * actions precisely at the beginning of each minute (when seconds = 0).
 * It also displays a second counter in the serial monitor.
 * 
 * Key Functions:
 * - minuteUpdate(): Toggles the LED and prints status messages
 * - alarmCallback(): Called by the RTC when seconds are zero
 * 
 * Key Variables:
 * - ledState: Tracks whether the LED is on or off
 * - previousMillis: Used for timing the serial updates
 * - minuteFlag: Safe interrupt flag to trigger updates in the main loop
 * 
 * Comparison with other methods:
 * - Alarm Method (this): Most precise timing for specific clock values. Triggers exactly 
 *   at the beginning of each minute with minimal code. Good for exact timing requirements.
 * - Time Polling Method: Continuously checks time, which uses more CPU but is flexible 
 *   for detecting various time patterns. Works even if RTC doesn't support alarms.
 * - Periodic Callback Method: Uses a counter approach with regular interrupts. Good for 
 *   custom intervals that don't align with clock values, but may drift over time.
 * 
 * To extend this code:
 * - Change the interval variable to update the serial display more/less frequently
 * - Modify the alarm to trigger at different times (e.g., every 30 seconds)
 * - Add additional actions to the minuteUpdate function
 */


// Include the RTC library
#include "RTC.h"

// Variables for Serial Debug
boolean showDebug = true;//toggles whether or not to show the debug printout
int lastSecond = -1; //holds the previoius time value 



bool ledState = false;
volatile bool minuteFlag = false;  // Flag for minute update

void setup() 
{
  // Initialize Serial Communication
  Serial.begin(9600);

  // Define LED as output
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize the RTC
  RTC.begin();

  // Set initial time (needed for alarm to work)
  RTCTime initialTime(14, Month::MARCH, 2025, 0, 0, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(initialTime);

  Serial.println("RTC Minute Trigger Example");
  Serial.println("Method: Alarm Interrupt");

  // Trigger the alarm every time the seconds are zero
  RTCTime alarmTime;
  alarmTime.setSecond(0);

  // Make sure to only match on the seconds
  AlarmMatch matchTime;
  matchTime.addMatchSecond();

  // Set the alarm callback
  RTC.setAlarmCallback(alarmCallback, alarmTime, matchTime);
}

void loop() 
{
  // Check if the minute flag has been set
  if (minuteFlag) 
  {
    minuteFlag = false;  // Reset the flag
    minuteUpdate();     // Call the minute update function
  }

 
  
  if(showDebug)
  {
    showTimeDebug();
  }

}

// Prints out current minute/second valuse
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
    Serial.print(" | LED state: ");
    Serial.println(ledState ? "ON" : "OFF");

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
  // Toggle LED state
  ledState = !ledState;
  digitalWrite(LED_BUILTIN, ledState);
  
  // Get current time for display
  RTCTime currentTime;
  RTC.getTime(currentTime);
  
  // Print update message
  Serial.println("---------------------");
  Serial.println("MINUTE UPDATE");
  Serial.print("Current Minute: ");
  Serial.println(currentTime.getMinutes());
  Serial.print("LED switched to: ");
  Serial.println(ledState ? "ON" : "OFF");  //looks at the current state of the LED to determine what to print
  Serial.println("---------------------");
}