/*
 * RTC Servo Notifier Time Cues with LED Matrix
 * 
 * This sketch demonstrates how to combine the RTC time-based triggers with 
 * the ServoNotifier animation system and LED Matrix display. Four different 
 * animations are played at specific minute cues, each with a different 
 * playback mode. The current animation name is displayed on the LED matrix.
 * 
 * Key Functions:
 * - checkTimeCues(): Checks if the current minute matches a cue time
 * - setupAnimations(): Creates the 4 different animations for the servo
 * - showTimeDebug(): Displays current time and animation status
 * - playAnimationForCue(): Plays the appropriate animation for the current cue
 * - updateMatrixDisplay(): Updates the LED matrix with current animation info
 * 
 * Key Variables:
 * - animationSpeed: Controls the playback speed of all animations
 * - minuteOffset: Value added to the minute to make testing different cues easier
 * - cueMinutes[]: Array of specific minutes when animations should trigger
 * - myServo: Servo object to be controlled
 * - notifier: ServoNotifier that handles the animations
 * - matrix: ArduinoLEDMatrix object for the display
 * - animPlaying: Tracks which animation is currently playing
 * 
 * Drawing to the Matrix
 * See this guide for drawing things to the matrix : https://github.com/DigitalFuturesOCADU/YouveBeenNotified/blob/main/ArduinoGraphics_R4.md
 */

// Include the required libraries
#include "RTC.h"
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

// Animation speed control - 1.0 is normal speed
float animationSpeed = 1.0;

// Time offset for testing - added to current minute to allow testing different
// cues without waiting. Set to 0 for normal operation.
int minuteOffset = 0;

// Variables for Serial Debug
boolean showDebug = true;  // toggles whether or not to show the debug printout
int lastSecond = -1;       // holds the previous time value

// Variables for counter state and timing
int currentMinute;         // stores the current minute from the RTC
int currentSecond;         // stores the current second from the RTC
int lastMinute = -1;       // Used to detect minute changes


// Create animation name variables
String animNames[] = {"Wave", "Sweep", "Pulse", "Random"};

// Animation display abbreviations (short enough to fit on matrix)
String animShortNames[] = {"WAV", "SWP", "PLS", "RND"};

// Playback mode abbreviations for display
String modeTexts[] = {"1X", "LP", "BM"};  // ONCE, LOOP, BOOMERANG

// Time cue system - each of these minutes will trigger a specific animation
// For example, when the (currentMinute + startOffset) equals cueMinutes[0],
// the first animation "Wave" will play.
int cueMinutes[] = {1, 2, 3, 4};  // Minutes when animations should trigger (adjust as needed)
int animPlaying = -1;  // Tracks which animation is currently playing (-1 = none)
int currentMode = -1;  // Tracks current playback mode

// Display variables
boolean displayNeedsUpdate = true;  // Flag to track when matrix needs updating
int displayToggleInterval = 3;      // Seconds between toggling display
boolean showAnimName = true;        // When true, shows animation name; when false, shows playback mode
int displayCounter = 0;             // Counter for toggling display

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
  if (!matrix.begin()) {
    Serial.println("Failed to initialize LED matrix!");
    while (1);
  }

  // Initialize the RTC
  RTC.begin();

  // Set initial time
  RTCTime initialTime(04, Month::APRIL, 2025, 10, 0, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(initialTime);

  // Attach servo to the pin
  myServo.attach(servoPin);
  
  // Print header
  Serial.println("RTC Servo Notifier with LED Matrix Display");
  Serial.println("------------------------------------------");
  Serial.print("Animation Speed: ");
  Serial.println(animationSpeed);
  Serial.print("Minute Offset for Testing: ");
  Serial.println(minuteOffset);
  Serial.println("Cue Minutes:");
  
  for(int i = 0; i < 4; i++)
  {
    Serial.print("  Cue ");
    Serial.print(i);
    Serial.print(": Minute ");
    Serial.print(cueMinutes[i]);
    Serial.print(" - Animation: ");
    Serial.println(animNames[i]);
  }
  
  // Setup animations
  setupAnimations();
  
  // Set animation speed
  notifier.setGlobalSpeed(animationSpeed);

  // Display startup message on matrix
  matrix.beginDraw();
  matrix.clear();
  matrix.stroke(0xFFFFFFFF);
  matrix.textFont(Font_4x6);
  matrix.text("RDY", 0, 0);
  matrix.endDraw();
  delay(1000);
}

void loop() 
{
  // Update the animation
  notifier.update();
  
  // Get current time
  RTCTime currentTime;
  RTC.getTime(currentTime);
  
  // Get current minute and second values
  currentMinute = (currentTime.getMinutes() + minuteOffset);  // Add offset and wrap around 60
  currentSecond = currentTime.getSeconds();
  
  // Check if minute has changed
  if (currentMinute != lastMinute) 
  {
    // Update the stored minute
    lastMinute = currentMinute;
    
    // Check for cue triggers
    checkTimeCues();
    
    // Matrix display needs update when minute changes
    displayNeedsUpdate = true;
  }
  
  // Show debug info if enabled
  if(showDebug)
  {
    showTimeDebug();
  }
  
  // Check if the animation value has changed
  if (notifier.hasChanged()) 
  {
    // Get current servo angle
    int currentAngle = notifier.getValue();
    
    // Move the servo to the current position
    myServo.write(currentAngle);
  }
  
  // Update matrix display when needed
  if(displayNeedsUpdate) 
  {
    updateMatrixDisplay();
    displayNeedsUpdate = false;
  }
  
  // Toggle between displaying animation name and mode every few seconds
  displayCounter++;
  if(displayCounter >= 100) {  // Approximately every 2 seconds (not exact)
    displayCounter = 0;
    
    // Only toggle if animation is playing
    if(animPlaying >= 0) {
      showAnimName = !showAnimName;
      displayNeedsUpdate = true;
    }
  }
  
  // Small delay to prevent overwhelming the processor
  delay(20);
}

// Creates and sets up the four animations
void setupAnimations() 
{
  // Animation 1 - "Wave" (slow back and forth) - LOOP mode
  KeyframeAnimation anim1(animNames[0]);
  anim1.addKeyFrame(0, 0);      // Start at 0 degrees
  anim1.addKeyFrame(90, 1000);  // Move to 90 degrees over 1 second
  anim1.addKeyFrame(30, 2000);  // Move to 30 degrees over 1 second
  anim1.addKeyFrame(120, 3000); // End at 120 degrees after 3 seconds total
  
  // Animation 2 - "Sweep" (full range sweep) - BOOMERANG mode
  KeyframeAnimation anim2(animNames[1]);
  anim2.addKeyFrame(0, 0);       // Start at 0 degrees
  anim2.addKeyFrame(180, 1500);  // Sweep all the way to 180 over 1.5 seconds
  
  // Animation 3 - "Pulse" (quick pulses) - ONCE mode
  KeyframeAnimation anim3(animNames[2]);
  anim3.addKeyFrame(90, 0);      // Start at center (90 degrees)
  anim3.addKeyFrame(45, 300);    // Quick pulse to 45
  anim3.addKeyFrame(90, 600);    // Back to center
  anim3.addKeyFrame(135, 900);   // Quick pulse to 135
  anim3.addKeyFrame(90, 1200);   // Back to center
  anim3.addKeyFrame(45, 1500);   // Another pulse to 45
  anim3.addKeyFrame(90, 1800);   // End at center
  
  // Animation 4 - "Random" (random positions) - LOOP mode
  KeyframeAnimation anim4(animNames[3]);
  anim4.addKeyFrame(90, 0);       // Start at center
  anim4.addKeyFrame(30, 800);     // Move to 30
  anim4.addKeyFrame(150, 1600);   // Move to 150
  anim4.addKeyFrame(60, 2400);    // Move to 60
  anim4.addKeyFrame(120, 3200);   // Move to 120
  anim4.addKeyFrame(20, 4000);    // Move to 20
  anim4.addKeyFrame(90, 4800);    // End at center
  
  // Add all animations to the notifier
  notifier.addAnimation(anim1);
  notifier.addAnimation(anim2);
  notifier.addAnimation(anim3);
  notifier.addAnimation(anim4);
}

// Checks if current minute matches a cue minute and triggers the appropriate animation
// This function runs whenever the minute changes, checking if the new minute
// matches one of our predefined cue times.
void checkTimeCues() 
{
  // Check each cue minute
  for(int i = 0; i < 4; i++)
  {
    if(currentMinute == cueMinutes[i])
    {
      // Cue matched, play corresponding animation
      playAnimationForCue(i);
      break;
    }
  }
}

// Plays the appropriate animation for the given cue index
void playAnimationForCue(int cueIndex) 
{
  // Only start a new animation if not already playing this one
  if(animPlaying != cueIndex)
  {
    animPlaying = cueIndex;
    
    Serial.println("---------------------");
    Serial.print("ANIMATION CUE TRIGGERED: ");
    Serial.println(animNames[cueIndex]);
    Serial.print("Current Minute: ");
    Serial.println(currentMinute);
    
    // Different playback mode for each animation
    switch(cueIndex)
    {
      case 0:
        // Play "Wave" animation in LOOP mode
        notifier.playAnimation(animNames[cueIndex], LOOP);
        currentMode = 1;  // LOOP
        Serial.println("Mode: LOOP");
        break;
        
      case 1:
        // Play "Sweep" animation in BOOMERANG mode
        notifier.playAnimation(animNames[cueIndex], BOOMERANG);
        currentMode = 2;  // BOOMERANG
        Serial.println("Mode: BOOMERANG");
        break;
        
      case 2:
        // Play "Pulse" animation in ONCE mode
        notifier.playAnimation(animNames[cueIndex], ONCE);
        currentMode = 0;  // ONCE
        Serial.println("Mode: ONCE");
        break;
        
      case 3:
        // Play "Random" animation in LOOP mode
        notifier.playAnimation(animNames[cueIndex], LOOP);
        currentMode = 1;  // LOOP
        Serial.println("Mode: LOOP");
        break;
    }
    
    Serial.println("---------------------");
    
    // Update display when animation changes
    displayNeedsUpdate = true;
    showAnimName = true;  // Start by showing animation name
  }
}

// Prints out current time and animation status
void showTimeDebug()
{
  // Check if second has changed
  if (currentSecond != lastSecond)
  {
    Serial.print("DEBUG Time: ");
    Serial.print(currentMinute);
    Serial.print(":");
    Serial.print(currentSecond);
    
    if(animPlaying >= 0)
    {
      Serial.print(" | Animation: ");
      Serial.print(notifier.getCurrentAnimationName());
      Serial.print(" | Servo Position: ");
      Serial.print(notifier.getValue());
      
      if(notifier.isCompleted())
      {
        Serial.print(" | COMPLETED");
        // Reset animPlaying when a ONCE animation completes
        if(animPlaying == 2) // Pulse animation
        {
          animPlaying = -1;
          // Update display when animation completes
          displayNeedsUpdate = true;
        }
      }
    }
    else
    {
      Serial.print(" | No animation playing");
    }
    
    Serial.println();
    lastSecond = currentSecond;
  }
}

// Updates the LED matrix with current animation information
void updateMatrixDisplay()
{
  matrix.beginDraw();
  matrix.clear();
  matrix.stroke(0xFFFFFFFF);  // White color
  matrix.textFont(Font_4x6);  // Smaller font to fit more text
  
  // Determine what to display
  if(animPlaying >= 0) 
  {
    // If animation is playing, show either name or mode
    if(showAnimName) 
    {
      // Show animation short name (abbreviated to fit)
      matrix.text(animShortNames[animPlaying], 0, 0);
    } 
    else 
    {
      // Show playback mode
      matrix.text(modeTexts[currentMode], 0, 0);
    }
  } 
  else 
  {
    // No animation playing, show IDLE
    matrix.text("IDLE", 0, 0);
  }
  
  matrix.endDraw();
}