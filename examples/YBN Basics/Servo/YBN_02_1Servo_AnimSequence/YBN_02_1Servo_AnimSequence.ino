/*
 * ServoNotifier Sequential Animations Example
 * 
 * This sketch demonstrates how to use the ServoNotifier class to play
 * three different animations in sequence. Each animation plays ONCE, and
 * when it completes, the next animation automatically starts. When all
 * animations are complete, "-fin-" is displayed on the LED matrix.
 * 
 * Key Functions:
 * - setup(): Creates three different servo animations and prepares for sequential playback
 * - loop(): Updates the animation and checks for completion to start the next animation
 * - printAnimationInfo(): Displays current animation information
 * - displayText(): Displays text on the LED matrix (animation name or "-fin-")
 * 
 * Key Variables:
 * - myServo: Servo object to be controlled
 * - notifier: ServoNotifier that handles the animations
 * - anim1, anim2, anim3: Three different KeyframeAnimations
 * - currentAnimIndex: Tracks which animation is currently playing
 * - animationsComplete: Flag to indicate when all animations are finished
 * 
 * Drawing to the Matrix
 * See this guide for drawing things to the matrix : https://github.com/DigitalFuturesOCADU/YouveBeenNotified/blob/main/ArduinoGraphics_R4.md
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



// Track which animation is currently playing
int currentAnimIndex = 0;

// Animation names array for easy reference
String animNames[] = {"Wave", "Sweep", "Pulse"};

// Create three different animations
KeyframeAnimation anim1(animNames[0]);
KeyframeAnimation anim2(animNames[1]);
KeyframeAnimation anim3(animNames[2]);

// Flag for when all animations are complete
bool animationsComplete = false;

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
  Serial.println("ServoNotifier Sequential Animations Example");
  Serial.println("------------------------------------------");
  
  // Set up animation 1 - "Wave" (slow back and forth)
  anim1.addKeyFrame(0, 0);      // Start at 0 degrees
  anim1.addKeyFrame(90, 1000);  // Move to 90 degrees over 1 second
  anim1.addKeyFrame(30, 2000);  // Move to 30 degrees over 1 second
  anim1.addKeyFrame(120, 3000); // End at 120 degrees after 3 seconds total
  
  // Set up animation 2 - "Sweep" (full range sweep)
  anim2.addKeyFrame(0, 0);       // Start at 0 degrees
  anim2.addKeyFrame(180, 1500);  // Sweep all the way to 180 over 1.5 seconds
  anim2.addKeyFrame(0, 3000);    // Return to 0 over 1.5 seconds
  
  // Set up animation 3 - "Pulse" (quick pulses)
  anim3.addKeyFrame(90, 0);      // Start at center (90 degrees)
  anim3.addKeyFrame(45, 300);    // Quick pulse to 45
  anim3.addKeyFrame(90, 600);    // Back to center
  anim3.addKeyFrame(135, 900);   // Quick pulse to 135
  anim3.addKeyFrame(90, 1200);   // Back to center
  anim3.addKeyFrame(45, 1500);   // Another pulse to 45
  anim3.addKeyFrame(90, 1800);   // End at center
  
  // Add all animations to the notifier
  notifier.addAnimation(anim1);
  notifier.addAnimation(anim2);
  notifier.addAnimation(anim3);
  
  // Set animation speed
  notifier.setGlobalSpeed(animationSpeed);
  
  // Start playing the first animation in ONCE mode
  notifier.playAnimation(animNames[currentAnimIndex], ONCE);
  
  Serial.println("Starting animation sequence!");
  Serial.print("First animation: ");
  Serial.println(animNames[currentAnimIndex]);
  
  // Display animation name on matrix
  displayText(animNames[currentAnimIndex]);
}

void loop() 
{
  // Update the animation
  notifier.update();
  
  // Check if all animations are complete
  if (animationsComplete) {
    // Nothing more to do, we're finished
    return;
  }
  
  // Check if the current animation is completed
  if (notifier.isCompleted()) {
    // Move to the next animation
    currentAnimIndex++;
    
    if (currentAnimIndex < 3) {
      // Start the next animation
      notifier.playAnimation(animNames[currentAnimIndex], ONCE);
      
      Serial.println("---------------------");
      Serial.print("Starting next animation: ");
      Serial.println(animNames[currentAnimIndex]);
      Serial.println("---------------------");
      
      // Display the new animation name
      displayText(animNames[currentAnimIndex]);
    } else {
      // All animations are complete
      animationsComplete = true;
      
      Serial.println("---------------------");
      Serial.println("All animations complete!");
      Serial.println("---------------------");
      
      // Display "-fin-" on the matrix
      displayText("fin");
    }
  }
  
  // Check if the animation value has changed
  if (notifier.hasChanged()) 
  {
    // Get current servo angle
    int currentAngle = notifier.getValue();
    
    // Move the servo to the current position
    myServo.write(currentAngle);
    
    // Print animation information when the value changes
    printAnimationInfo();
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

// Displays text on the LED matrix
void displayText(String text) 
{
  // Begin drawing to the matrix
  matrix.beginDraw();
  matrix.clear(); // Clear the previous display
  
  // Set text properties
  matrix.stroke(0xFFFFFFFF); // Set text color to white (on)
  matrix.textFont(Font_4x6);  // Use the smaller font
  
  // Display text
  matrix.text(text, 0, 0);
  
  // End drawing and update the matrix
  matrix.endDraw();
}