/*
 * RTC Servo Animation - Simple Time Cues
 * 
 * This sketch demonstrates how to trigger different servo animations
 * at specific times using the RTC. Five different animations play
 * at five specific minute "cues", each with its own playback mode.
 * 
 * Key Functions:
 * - setup(): Initializes the RTC, servo, and creates five animations
 * - loop(): Checks current time and plays animations at specific minutes
 * - setupAnimations(): Creates the 5 different animations for the servo
 * 
 * Key Variables:
 * - cue0, cue1, cue2, cue3, cue4: Specific minutes when animations should trigger
 * - myServo: Servo object to be controlled
 * - notifier: ServoNotifier that handles the animations
 * - animationSpeed: Controls the playback speed of all animations
 * - minuteOffset: Value added to the minute for easier testing
 * 
 * Implementation Notes:
 * - Uses simple if statements to trigger animations at specific times
 * - Each animation has a different playback mode (ONCE, LOOP, BOOMERANG)
 * - Serial monitor displays time and current animation status
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

// Animation speed control - 1.0 is normal speed
float animationSpeed = 1.0;

// Time offset for testing - added to current minute
// Set to 0 for normal operation
int minuteOffset = 0;

// Individual minute cues for triggering animations
int cue0 = 0;  // Small oscillation animation at minute 0 (start)
int cue1 = 1;  // Wave animation at minute 1
int cue2 = 2;  // Sweep animation at minute 2
int cue3 = 3;  // Pulse animation at minute 3
int cue4 = 4;  // Random animation at minute 4

// Variables for time tracking
int currentMinute;
int lastMinute = -1;
String currentAnimation = "None";

// Animation names as individual variables
String animName0 = "Oscillate";
String animName1 = "Wave";
String animName2 = "Sweep";
String animName3 = "Pulse";
String animName4 = "Random";

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
  Serial.println("RTC Servo Animation - Simple Time Cues");
  Serial.println("-----------------------------------------");
  Serial.print("Animation Speed: ");
  Serial.println(animationSpeed);
  Serial.print("Minute Offset for Testing: ");
  Serial.println(minuteOffset);
  Serial.println("Cue Minutes:");
  Serial.print("  Cue 0: Minute ");
  Serial.print(cue0);
  Serial.print(" - ");
  Serial.print(animName0);
  Serial.println(" animation (BOOMERANG mode)");
  Serial.print("  Cue 1: Minute ");
  Serial.print(cue1);
  Serial.print(" - ");
  Serial.print(animName1);
  Serial.println(" animation (LOOP mode)");
  Serial.print("  Cue 2: Minute ");
  Serial.print(cue2);
  Serial.print(" - ");
  Serial.print(animName2);
  Serial.println(" animation (BOOMERANG mode)");
  Serial.print("  Cue 3: Minute ");
  Serial.print(cue3);
  Serial.print(" - ");
  Serial.print(animName3);
  Serial.println(" animation (ONCE mode)");
  Serial.print("  Cue 4: Minute ");
  Serial.print(cue4);
  Serial.print(" - ");
  Serial.print(animName4);
  Serial.println(" animation (LOOP mode)");
  
  // Setup animations
  setupAnimations();
  
  // Set animation speed
  notifier.setGlobalSpeed(animationSpeed);
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
    
    // Check for each cue time and play corresponding animation
    if (currentMinute == cue0) {
      // Cue 0: Play Oscillate animation in BOOMERANG mode
      notifier.playAnimation(animName0, BOOMERANG);
      currentAnimation = animName0;
      Serial.print("Playing ");
      Serial.print(animName0);
      Serial.println(" animation in BOOMERANG mode");
    }
    else if (currentMinute == cue1) {
      // Cue 1: Play Wave animation in LOOP mode
      notifier.playAnimation(animName1, LOOP);
      currentAnimation = animName1;
      Serial.print("Playing ");
      Serial.print(animName1);
      Serial.println(" animation in LOOP mode");
    }
    else if (currentMinute == cue2) {
      // Cue 2: Play Sweep animation in BOOMERANG mode
      notifier.playAnimation(animName2, BOOMERANG);
      currentAnimation = animName2;
      Serial.print("Playing ");
      Serial.print(animName2);
      Serial.println(" animation in BOOMERANG mode");
    }
    else if (currentMinute == cue3) {
      // Cue 3: Play Pulse animation in ONCE mode
      notifier.playAnimation(animName3, ONCE);
      currentAnimation = animName3;
      Serial.print("Playing ");
      Serial.print(animName3);
      Serial.println(" animation in ONCE mode");
    }
    else if (currentMinute == cue4) {
      // Cue 4: Play Random animation in LOOP mode
      notifier.playAnimation(animName4, LOOP);
      currentAnimation = animName4;
      Serial.print("Playing ");
      Serial.print(animName4);
      Serial.println(" animation in LOOP mode");
    }
  }
  
  // Check if animation has completed (important for ONCE mode)
  if (notifier.isCompleted() && currentAnimation == animName3) {
    Serial.print(animName3);
    Serial.println(" animation completed");
    currentAnimation = "None";
  }
  
  // Check if the animation value has changed
  if (notifier.hasChanged()) {
    // Move the servo to the current position
    myServo.write(notifier.getValue());
  }
  
  // Small delay to prevent overwhelming the processor
  delay(20);
}

// Creates and sets up the five animations
void setupAnimations() {
  // Animation 0 - "Oscillate" (gentle oscillation) - BOOMERANG mode
  KeyframeAnimation anim0(animName0);
  anim0.addKeyFrame(80, 0);      // Start near center
  anim0.addKeyFrame(100, 500);   // Move slightly right
  anim0.addKeyFrame(80, 1000);   // Back to center-left
  anim0.addKeyFrame(60, 1500);   // Move slightly left
  anim0.addKeyFrame(80, 2000);   // Return to start
  
  // Animation 1 - "Wave" (slow back and forth) - LOOP mode
  KeyframeAnimation anim1(animName1);
  anim1.addKeyFrame(0, 0);      // Start at 0 degrees
  anim1.addKeyFrame(90, 1000);  // Move to 90 degrees over 1 second
  anim1.addKeyFrame(30, 2000);  // Move to 30 degrees over 1 second
  anim1.addKeyFrame(120, 3000); // End at 120 degrees after 3 seconds total
  
  // Animation 2 - "Sweep" (full range sweep) - BOOMERANG mode
  KeyframeAnimation anim2(animName2);
  anim2.addKeyFrame(0, 0);       // Start at 0 degrees
  anim2.addKeyFrame(180, 1500);  // Sweep all the way to 180 over 1.5 seconds
  
  // Animation 3 - "Pulse" (quick pulses) - ONCE mode
  KeyframeAnimation anim3(animName3);
  anim3.addKeyFrame(90, 0);      // Start at center (90 degrees)
  anim3.addKeyFrame(45, 300);    // Quick pulse to 45
  anim3.addKeyFrame(90, 600);    // Back to center
  anim3.addKeyFrame(135, 900);   // Quick pulse to 135
  anim3.addKeyFrame(90, 1200);   // Back to center
  anim3.addKeyFrame(45, 1500);   // Another pulse to 45
  anim3.addKeyFrame(90, 1800);   // End at center
  
  // Animation 4 - "Random" (random positions) - LOOP mode
  KeyframeAnimation anim4(animName4);
  anim4.addKeyFrame(90, 0);       // Start at center
  anim4.addKeyFrame(30, 800);     // Move to 30
  anim4.addKeyFrame(150, 1600);   // Move to 150
  anim4.addKeyFrame(60, 2400);    // Move to 60
  anim4.addKeyFrame(120, 3200);   // Move to 120
  anim4.addKeyFrame(20, 4000);    // Move to 20
  anim4.addKeyFrame(90, 4800);    // End at center
  
  // Add all animations to the notifier
  notifier.addAnimation(anim0);
  notifier.addAnimation(anim1);
  notifier.addAnimation(anim2);
  notifier.addAnimation(anim3);
  notifier.addAnimation(anim4);
}
