/*
 * ServoAnimation Sweep Example
 * 
 * This sketch demonstrates how to create a simple servo sweep animation
 * using the YouveBeenNotified library. It mimics the behavior of the
 * standard Sweep example but uses the animation system for smoother motion.
 * 
 * Key Functions:
 * - setup(): Initializes the servo and creates a simple sweep animation
 * - loop(): Updates the animation and applies it to the servo
 * 
 * Key Variables:
 * - myServo: Servo object to be controlled
 * - notifier: ServoNotifier that handles the animation
 * 
 * Implementation Notes:
 * - Uses a single keyframe animation with only two points (0° and 180°)
 * - BOOMERANG playback mode makes it swing back and forth automatically
 * - No delay() calls are used; the ServoNotifier handles all timing
 * 
 * Extension Ideas:
 * - Adjust the animation speed using notifier.setGlobalSpeed()
 * - Change the range of motion by modifying the keyframe values
 * - Try different playback modes (ONCE, LOOP) to see how they behave
 * - Add more keyframes to create more complex motion patterns
 */

#include <Servo.h>
#include "YouveBeenNotified.h"

// Create servo and notifier objects
Servo myServo;
ServoNotifier notifier(myServo);

void setup() {
  // Attach servo to pin 9
  myServo.attach(9);
  
  // Create a sweep animation
  KeyframeAnimation sweep("sweep");
  
  // Add single keyframe to move from 0 to 180 over 1 second
  sweep.addKeyFrame(0, 0);      // Start position
  sweep.addKeyFrame(180, 1000); // End position
  
  // Add animation to notifier
  notifier.addAnimation(sweep);
  
  // Play animation in boomerang mode (goes forward, then reverse)
  notifier.playAnimation("sweep", BOOMERANG);
}

void loop() {
  // Update animation calculations
  notifier.update();
  
  // If value changed, update servo position
  if (notifier.hasChanged()) {
    myServo.write(notifier.getValue());
  }
}