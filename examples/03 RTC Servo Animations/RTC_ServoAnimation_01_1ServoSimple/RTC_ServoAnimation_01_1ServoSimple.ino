/*
 * RTCservo Simplest Alternating Animations
 * 
 * This sketch demonstrates how to use the RTC to control a servo motor,
 * alternating between two different animations each minute. It provides
 * the simplest example of combining time-based triggers with ServoNotifier
 * animations. Animation 1 plays on even minutes, and Animation 2 plays on
 * odd minutes.
 * 
 * Key Functions:
 * - setup(): Initializes the servo and creates two animations (Wave and Sweep)
 * - loop(): Checks the current minute and switches animations when the minute changes
 * 
 * Key Variables:
 * - myServo: Servo object to be controlled
 * - notifier: ServoNotifier that handles the animations
 * - anim1Name/anim2Name: String variables storing the names of the animations
 * - lastMinute: Stores the last minute value to detect changes
 * 
 * Implementation Notes:
 * - Uses the modulo operator (%) to determine even/odd minutes
 * - Wave animation: A gentle motion between 45 and 135 degrees
 * - Sweep animation: Full range motion from 0 to 180 degrees
 * - Both animations use LOOP mode to repeat continuously
 * - No delay() calls are used; the ServoNotifier handles all timing
 * 
 * Extension Ideas:
 * - Add more animations by creating additional KeyframeAnimation objects
 * - Experiment with different playback modes (ONCE, LOOP, BOOMERANG)
 * - Adjust animation speed using notifier.setGlobalSpeed()
 * - Create more complex animation patterns by adding more keyframes
 * - Add a display to show the current animation status on the LED matrix
 */

// Include the required libraries
#include "RTC.h"
#include <Servo.h>
#include "YouveBeenNotified.h"

// Create servo and ServoNotifier
int servoPin = 9;
Servo myServo;
ServoNotifier notifier(myServo);

// Time offset for testing
int minuteOffset = 1; //use this to start further along in the time sequence

// Speed Adjustment
float playSpeed = 2.0; //use this to scale the speed of the current animation

// Animation names
String anim1Name = "Wave";
String anim2Name = "Sweep";

// Track the last minute value
int lastMinute = -1;  

void setup() 
{
  // Initialize serial, RTC, and servo
  Serial.begin(9600);
  RTC.begin();
  
  // Set initial time
  RTCTime initialTime(04, Month::APRIL, 2025, 10, 0, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(initialTime);

  // Connect servo to pin 9
  myServo.attach(servoPin);
  
  // Setup animation 1 - simple wave pattern
  KeyframeAnimation anim1(anim1Name);
  anim1.addKeyFrame(45, 0);       // Start at 45 degrees
  anim1.addKeyFrame(135, 1000);   // Move to 135 degrees over 1 second
  anim1.addKeyFrame(45, 2000);    // Back to 45 degrees over 1 second
  
  // Setup animation 2 - full range sweep
  KeyframeAnimation anim2(anim2Name);
  anim2.addKeyFrame(0, 0);        // Start at 0 degrees
  anim2.addKeyFrame(180, 1500);   // Move to 180 degrees over 1.5 seconds
  anim2.addKeyFrame(0, 3000);     // Back to 0 degrees over 1.5 seconds
  
  // Add animations to notifier
  notifier.addAnimation(anim1);
  notifier.addAnimation(anim2);

  // set the playSpeed
  notifier.setGlobalSpeed(playSpeed);
  
  Serial.println("RTCservo Simplest Alternating Animations");
  Serial.println("Animation 1 plays on even minutes");
  Serial.println("Animation 2 plays on odd minutes");
}

void loop() 
{
  // Update animation
  notifier.update();
  
  // Get current time
  RTCTime currentTime;
  RTC.getTime(currentTime);
  int currentMinute = currentTime.getMinutes() + minuteOffset; //add the clock minute to the offset to allow for easier testing
  
  // Check if minute has changed
  if (currentMinute != lastMinute) 
  {
    lastMinute = currentMinute;
    
    // Determine which animation to play based on even/odd minute
    if (currentMinute % 2 == 0) {
      // Even minute - play animation 1
      notifier.playAnimation(anim1Name, LOOP);
      Serial.print("Minute ");
      Serial.print(currentMinute);
      Serial.println(": Playing Wave animation");
    } else {
      // Odd minute - play animation 2
      notifier.playAnimation(anim2Name, LOOP);
      Serial.print("Minute ");
      Serial.print(currentMinute);
      Serial.println(": Playing Sweep animation");
    }
  }
  
  // If servo position has changed, update the servo
  if (notifier.hasChanged()) 
  {
    myServo.write(notifier.getValue());
  }
}