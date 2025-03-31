/*
 * RTC Servo Animation - Speed Control
 * 
 * This sketch demonstrates how to use the RTC to control a servo motor's
 * animation speed. A single animation plays continuously, but its speed 
 * changes at specific minute "cues". This shows how to create dynamic 
 * movement without needing multiple animations.
 * 
 * Key Functions:
 * - setup(): Initializes the RTC, servo, and creates a single animation
 * - loop(): Checks current time and adjusts animation speed at specific minutes
 * - setupAnimation(): Creates a single complex animation for the servo
 * 
 * Key Variables:
 * - cue1, cue2, cue3, cue4: Specific minutes when speed changes occur
 * - myServo: Servo object to be controlled
 * - notifier: ServoNotifier that handles the animation
 * - speedSettings: Values for different playback speeds at each cue
 * - minuteOffset: Value added to the minute for easier testing
 * 
 * Implementation Notes:
 * - Uses simple if statements to trigger speed changes at specific times
 * - Keeps the same animation playing throughout, only changes its speed
 * - Serial monitor displays current time and animation speed
 * - No delay() calls are used; the ServoNotifier handles all timing
 */

// Include the required libraries
#include "RTC.h"
#include <Servo.h>
#include "YouveBeenNotified.h"

// Create servo object and specify the pin
Servo myServo;
int servoPin = 9;  // Connect servo to pin 9

// Create ServoNotifier object
ServoNotifier notifier(myServo);

// Time offset for testing - added to current minute
// Set to 0 for normal operation
int minuteOffset = 0;

// Individual minute cues for triggering speed changes
int cue0 = 0;  // Normal speed (1.0) at minute 0
int cue1 = 1;  // Slow speed (0.5) at minute 1
int cue2 = 2;  // Fast speed (2.0) at minute 2
int cue3 = 3;  // Very slow speed (0.25) at minute 3
int cue4 = 4;  // Very fast speed (3.0) at minute 4

// Speed settings for each cue (multipliers)
float speedSettings[] = {1.0, 0.5, 2.0, 0.25, 3.0};

// Variables for time tracking
int currentMinute;
int lastMinute = -1;
float currentSpeed = 1.0;

// Animation name
String animName = "Wave";

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);
  
  // Wait for serial port to connect
  while (!Serial) {
    ; // Wait for serial port to connect
  }

  // Initialize the RTC
  RTC.begin();

  // Set initial time
  RTCTime initialTime(04, Month::APRIL, 2025, 10, 0, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(initialTime);

  // Attach servo to the pin
  myServo.attach(servoPin);
  
  // Print header
  Serial.println("RTC Servo Animation - Speed Control");
  Serial.println("-----------------------------------------");
  Serial.print("Minute Offset for Testing: ");
  Serial.println(minuteOffset);
  Serial.println("Cue Minutes and Speeds:");
  Serial.print("  Cue 0: Minute ");
  Serial.print(cue0);
  Serial.print(" - Normal speed (");
  Serial.print(speedSettings[0]);
  Serial.println("x)");
  Serial.print("  Cue 1: Minute ");
  Serial.print(cue1);
  Serial.print(" - Slow speed (");
  Serial.print(speedSettings[1]);
  Serial.println("x)");
  Serial.print("  Cue 2: Minute ");
  Serial.print(cue2);
  Serial.print(" - Fast speed (");
  Serial.print(speedSettings[2]);
  Serial.println("x)");
  Serial.print("  Cue 3: Minute ");
  Serial.print(cue3);
  Serial.print(" - Very slow speed (");
  Serial.print(speedSettings[3]);
  Serial.println("x)");
  Serial.print("  Cue 4: Minute ");
  Serial.print(cue4);
  Serial.print(" - Very fast speed (");
  Serial.print(speedSettings[4]);
  Serial.println("x)");
  
  // Setup animation
  setupAnimation();
  
  // Set initial animation speed
  notifier.setGlobalSpeed(currentSpeed);
  
  // Start the animation in LOOP mode
  notifier.playAnimation(animName, LOOP);
  
  Serial.print("Starting animation: ");
  Serial.print(animName);
  Serial.println(" in LOOP mode");
}

void loop() {
  // Update the animation
  notifier.update();
  
  // Get current time
  RTCTime currentTime;
  RTC.getTime(currentTime);
  
  // Get current minute value (add offset for testing)
  currentMinute = (currentTime.getMinutes() + minuteOffset);
  
  // Check if minute has changed
  if (currentMinute != lastMinute) {
    lastMinute = currentMinute;
    
    // Print minute change debug info
    Serial.print("Minute changed to: ");
    Serial.println(currentMinute);
    
    // Check for each cue time and adjust animation speed
    if (currentMinute == cue0) {
      // Normal speed
      currentSpeed = speedSettings[0];
      notifier.setGlobalSpeed(currentSpeed);
      Serial.print("Setting speed to NORMAL (");
      Serial.print(currentSpeed);
      Serial.println("x)");
    }
    else if (currentMinute == cue1) {
      // Slow speed
      currentSpeed = speedSettings[1];
      notifier.setGlobalSpeed(currentSpeed);
      Serial.print("Setting speed to SLOW (");
      Serial.print(currentSpeed);
      Serial.println("x)");
    }
    else if (currentMinute == cue2) {
      // Fast speed
      currentSpeed = speedSettings[2];
      notifier.setGlobalSpeed(currentSpeed);
      Serial.print("Setting speed to FAST (");
      Serial.print(currentSpeed);
      Serial.println("x)");
    }
    else if (currentMinute == cue3) {
      // Very slow speed
      currentSpeed = speedSettings[3];
      notifier.setGlobalSpeed(currentSpeed);
      Serial.print("Setting speed to VERY SLOW (");
      Serial.print(currentSpeed);
      Serial.println("x)");
    }
    else if (currentMinute == cue4) {
      // Very fast speed
      currentSpeed = speedSettings[4];
      notifier.setGlobalSpeed(currentSpeed);
      Serial.print("Setting speed to VERY FAST (");
      Serial.print(currentSpeed);
      Serial.println("x)");
    }
  }
  
  // Check if the animation value has changed
  if (notifier.hasChanged()) {
    // Move the servo to the current position
    myServo.write(notifier.getValue());
  }
  
  // Small delay to prevent overwhelming the processor
  delay(20);
}

// Creates and sets up the animation
void setupAnimation() {
  // Create a wave animation with multiple keyframes
  KeyframeAnimation animation(animName);
  
  // Add keyframes to create a complex, undulating motion
  animation.addKeyFrame(90, 0);       // Start at center position
  animation.addKeyFrame(30, 1000);    // Move to 30 degrees over 1 second
  animation.addKeyFrame(150, 2000);   // Move to 150 degrees over 1 second
  animation.addKeyFrame(90, 3000);    // Return to center over 1 second
  animation.addKeyFrame(120, 4000);   // Move to 120 degrees
  animation.addKeyFrame(60, 5000);    // Move to 60 degrees
  animation.addKeyFrame(90, 6000);    // End at center position
  
  // Add animation to notifier
  notifier.addAnimation(animation);
}
