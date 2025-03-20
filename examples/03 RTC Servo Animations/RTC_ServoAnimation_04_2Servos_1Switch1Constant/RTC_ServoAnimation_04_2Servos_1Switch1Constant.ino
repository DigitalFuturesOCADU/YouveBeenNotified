/*
 * RTCservo Two Servos Example
 * 
 * This sketch controls two servos with different animation patterns.
 * Servo 1 alternates between two animations each minute, while Servo 2
 * continuously oscillates between 0-180 degrees without stopping.
 * 
 * Key Functions:
 * - setupServo1Animations(): Creates two animations for the first servo
 * - setupServo2Animation(): Creates the continuous oscillation for the second servo
 * - loop(): Updates both animations and handles minute-based switching for Servo 1
 * 
 * Key Variables:
 * - servo1/servo2: Servo objects to be controlled
 * - notifier1/notifier2: ServoNotifier objects that handle the animations
 * - servoPin1/servoPin2: Pins connected to the servos
 * - minuteOffset: Value added to the current minute for easier testing
 * - playSpeed: Controls animation speed for both servos
 * - lastMinute: Stores the last minute value to detect changes
 * 
 * Implementation Notes:
 * - Servo 1 alternates between Wave and Sweep animations based on even/odd minutes
 * - Servo 2 runs a continuous slow oscillation independent of the minute changes
 * - Both servos have independent ServoNotifier objects but share the same playSpeed
 * - No delay() calls are used; the ServoNotifiers handle all timing
 * 
 * Troubleshooting:
 * - Added more debug output to monitor animation state and servo values
 * - Make sure servo power is connected (5V and GND)
 * - Check that signal wires are connected to the correct pins
 * - Try different animation speeds if movements are too fast
 */

// Include the required libraries
#include "RTC.h"
#include <Servo.h>
#include "YouveBeenNotified.h"

// Create first servo and notifier (for alternating animations)
int servoPin1 = 9;
Servo servo1;
ServoNotifier notifier1(servo1);

// Create second servo and notifier (for continuous oscillation)
int servoPin2 = 10;
Servo servo2;
ServoNotifier notifier2(servo2);

// Time offset for testing
int minuteOffset = 0; //use this to start further along in the time sequence

// Speed Adjustment
float playSpeed = 1.0; //use this to scale the speed of the current animation (changed from 2.0 to 1.0)

// Animation names
String anim1Name = "Wave";
String anim2Name = "Sweep";
String oscName = "Oscillate";


// Track the last minute value
int lastMinute = -1;


void setup() 
{
  // Initialize serial and RTC
  Serial.begin(9600);
  
  // Wait for serial to connect
  Serial.println("Starting RTCservo Two Servos Example...");
  
  RTC.begin();
  
  // Set initial time
  RTCTime initialTime(04, Month::APRIL, 2025, 10, 0, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(initialTime);

  // Connect servos to pins
  servo1.attach(servoPin1);  // Servo 1 on pin 9
  
  servo2.attach(servoPin2);  // Servo 2 on pin 10
  
  // Setup animations for servo 1
  setupServo1Animations();
  
  // Setup animation for servo 2
  setupServo2Animation();
  
  // Set the playSpeed for both notifiers
  notifier1.setGlobalSpeed(playSpeed);
  notifier2.setGlobalSpeed(playSpeed);
  
  // Start the animation on Servo 2
  notifier2.playAnimation(oscName, LOOP);

}

// Setup animations for servo 1 (alternating each minute)
void setupServo1Animations() 
{
  Serial.println("Setting up animations for Servo 1...");
  
  // Setup animation 1 - simple wave pattern
  KeyframeAnimation anim1(anim1Name);
  anim1.addKeyFrame(45, 0);       // Start at 45 degrees
  anim1.addKeyFrame(135, 1000);   // Move to 135 degrees over 1 second
  anim1.addKeyFrame(135, 2000);   // hold 135 degrees over 1 second
  anim1.addKeyFrame(45, 3000);    // Back to 45 degrees over 1 second
  
  // Setup animation 2 - full range sweep
  KeyframeAnimation anim2(anim2Name);
  anim2.addKeyFrame(0, 0);        // Start at 0 degrees
  anim2.addKeyFrame(180, 1500);   // Move to 180 degrees over 1.5 seconds
  anim2.addKeyFrame(0, 3000);     // Back to 0 degrees over 1.5 seconds
  
  // Add animations to notifier
  notifier1.addAnimation(anim1);
  notifier1.addAnimation(anim2);
}

// Setup animation for servo 2 (continuous oscillation)
void setupServo2Animation() 
{
  Serial.println("Setting up animation for Servo 2...");
  
  // Create a slow oscillation animation (takes 8 seconds for a complete cycle)
  KeyframeAnimation oscillate(oscName);
  oscillate.addKeyFrame(0, 0);        // Start at 0 degrees
  oscillate.addKeyFrame(180, 4000);   // Slowly move to 180 degrees over 4 seconds
  oscillate.addKeyFrame(0, 8000);     // Slowly return to 0 degrees over 4 seconds
  
  // Add animation to notifier
  notifier2.addAnimation(oscillate);
}

void loop() 
{
  // Update both animations
  notifier1.update();
  notifier2.update();
  
  // Get current time
  RTCTime currentTime;
  RTC.getTime(currentTime);
  int currentMinute = currentTime.getMinutes() + minuteOffset; //add the clock minute to the offset to allow for easier testing
  
  Serial.print("Current Minute : ");
  Serial.println(currentMinute);

  // Check if minute has changed (for servo 1)
  if (currentMinute != lastMinute) 
  {
    lastMinute = currentMinute;
    
    // Determine which animation to play for servo 1
    if (currentMinute % 2 == 0) {
      // Even minute - play animation 1

      notifier1.playAnimation(anim1Name, LOOP);
      Serial.print("Minute ");
      Serial.print(currentMinute);
      Serial.println(": Playing Wave animation on Servo 1");
    } else 
    {
      // Odd minute - play animation 2
 
      notifier1.playAnimation(anim2Name, LOOP);
      Serial.print("Minute ");
      Serial.print(currentMinute);
      Serial.println(": Playing Sweep animation on Servo 1");
    }
  }
  
  // Update servo 1 if position has changed
  if (notifier1.hasChanged()) 
  {
    int value1 = notifier1.getValue();
    servo1.write(value1);
    Serial.print("Servo 1 position: ");
    Serial.print(value1);
    Serial.print(" Servo 1 animation: ");
    Serial.println(notifier1.getCurrentAnimationName());
  }
  
  // Update servo 2 if position has changed
  if (notifier2.hasChanged()) 
  {
    int value2 = notifier2.getValue();
    servo2.write(value2);
    Serial.print("Servo 2 position: ");
    Serial.print(value2);
    Serial.print(" Servo 2 animation: ");
    Serial.println(notifier2.getCurrentAnimationName());
  }
  

}