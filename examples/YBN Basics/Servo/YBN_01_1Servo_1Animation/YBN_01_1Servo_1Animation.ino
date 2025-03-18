/*
 * ServoNotifier Animation Example
 * 
 * This sketch demonstrates how to use the ServoNotifier class to animate
 * a servo motor through a series of 4 keyframes. The animation updates are
 * monitored and information is printed to the Serial Monitor and displayed
 * on the LED matrix.
 * 
 * Key Functions:
 * - setup(): Creates the servo animation with 4 keyframes
 * - loop(): Updates the animation and prints status when value changes
 * - printAnimationInfo(): Displays current animation information
 * - displayServoAngle(): Displays the current servo angle on the LED matrix
 * 
 * Key Variables:
 * - myServo: Servo object to be controlled
 * - notifier: ServoNotifier that handles the animation
 * - servoAnimation: Defines the keyframes and timing for the animation
 * - animationName: String variable storing the name of the animation
 * - animationSpeed: Controls animation playback rate (1.0 = normal, 2.0 = double, 0.5 = half)
 * 
 * Drawing to the Matrix
 * See this guide for drawing things to the matrix : https://github.com/DigitalFuturesOCADU/YouveBeenNotified/blob/main/ArduinoGraphics_R4.md
 * 
 * Extension Ideas:
 * - Changing/Adding Keyframes:
 *   • Add more keyframes to create more complex movements:
 *     servoAnimation.addKeyFrame(angle, timeInMilliseconds);
 *   • Change existing keyframes in setup() to modify the animation pattern
 *   • Create multiple animations with different sets of keyframes
 *   • Use different timing between keyframes for varied speeds
 * 
 * - Changing Playback Modes:
 *   • ONCE: Play the animation one time then stop
 *     notifier.playAnimation(animationName, ONCE);
 *   • LOOP: Play the animation repeatedly (current setting)
 *     notifier.playAnimation(animationName, LOOP);
 *   • BOOMERANG: Play forward then backward repeatedly
 *     notifier.playAnimation(animationName, BOOMERANG);
 *   • You can change the mode during runtime using:
 *     notifier.setPlaybackMode(newMode);
 * 
 * - Changing Animation Speed:
 *   • Change the playback rate of the entire animation:
 *     notifier.setGlobalSpeed(animationSpeed);
 *   • 1.0 = normal speed (100%)
 *   • 2.0 = double speed (200%)
 *   • 0.5 = half speed (50%) 
 */

// Include the required libraries
#include <Servo.h>
#include "YouveBeenNotified.h"
// Include ArduinoGraphics BEFORE Arduino_LED_Matrix
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

// Create the matrix instance
ArduinoLEDMatrix matrix;

// Create servo object and specify the pin
Servo myServo;
int servoPin = 9;  // Connect servo to pin 9

// Create ServoNotifier object
ServoNotifier notifier(myServo);

// Create a string variable for the animation name
String animationName = "Wave";

// Create a KeyframeAnimation
KeyframeAnimation servoAnimation(animationName);

// Animation speed control - 1.0 is normal speed
float animationSpeed = 1.0;

void setup() 
{
  // Initialize Serial Communication
  Serial.begin(9600);
  
  // Wait for serial port to connect
  while (!Serial) 
  {
    ; // Wait for serial port to connect
  }

  // Initialize the LED matrix
  if (!matrix.begin()) 
  {
    Serial.println("Failed to initialize LED matrix!");
    while (1);
  }

  // Attach servo to the pin
  myServo.attach(servoPin);
  
  // Print header
  Serial.println("ServoNotifier Animation Example");
  Serial.println("-------------------------------");
  
  // Set up the animation with 4 keyframes
  // Parameters: value (angle in degrees), time (milliseconds)
  servoAnimation.addKeyFrame(0, 0);      // Start at 0 degrees
  servoAnimation.addKeyFrame(90, 1000);  // Move to 90 degrees over 1 second
  servoAnimation.addKeyFrame(30, 2000);  // Move to 30 degrees over 1 second
  servoAnimation.addKeyFrame(120, 3000); // End at 120 degrees after 3 seconds total
  
  // Add the animation to the notifier
  notifier.addAnimation(servoAnimation);
  
  // Set animation speed
  notifier.setGlobalSpeed(animationSpeed);
  
  // Start playing the animation in LOOP mode
  notifier.playAnimation(animationName, LOOP);
  
  Serial.println("Animation started!");
  Serial.print("Animation speed: ");
  Serial.println(animationSpeed);
  
  // Display initial angle on matrix
  displayServoAngle(0);
}

void loop() 
{
  // Update the animation
  notifier.update();
  
  // Check if the animation value has changed
  if (notifier.hasChanged()) 
  {
    // Get current servo angle
    int currentAngle = notifier.getValue();
    
    // Move the servo to the current position
    myServo.write(currentAngle);
    
    // Print animation information when the value changes
    printAnimationInfo();
    
    // Update the LED matrix with the current angle
    displayServoAngle(currentAngle);
  }
}

// Prints current animation status information to Serial Monitor
void printAnimationInfo() 
{
  Serial.println("---------------------");
  Serial.print("Animation Name: ");
  Serial.println(notifier.getCurrentAnimationName());
  
  Serial.print("Current Value: ");
  Serial.println(notifier.getValue());
  
  Serial.print("Animation Speed: ");
  Serial.println(animationSpeed);
  
  Serial.print("Time to Next Keyframe: ");
  Serial.print(notifier.timeToNextKey());
  Serial.println(" ms");
  
  Serial.print("Remaining Animation Time: ");
  Serial.print(notifier.timeRemaining());
  Serial.println(" ms");
  Serial.println("---------------------");
}

// Displays the current servo angle on the LED matrix
void displayServoAngle(int angle) 
{
  // Begin drawing to the matrix
  matrix.beginDraw();
  matrix.clear(); // Clear the previous display
  
  // Set text properties
  matrix.stroke(0xFFFFFFFF); // Set text color to white (on)
  matrix.textFont(Font_4x6);  // Use the smaller font
  
  // Display servo angle 
  matrix.text(String(angle), 0, 0);
  
  // End drawing and update the matrix
  matrix.endDraw();
}