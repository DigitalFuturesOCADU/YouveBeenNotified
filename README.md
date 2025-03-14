# You've Been Notified

## Table of Contents

- [Overview](#overview)
- [Installing the Library](#installing-the-library)
- [Using the LED Matrix for Debugging](#using-the-led-matrix-for-debugging)
- [Real Time Clock (RTC) on Arduino R4 WiFi](#real-time-clock-rtc-on-arduino-r4-wifi)
  - [Guide to Arduino R4 Wifi RTC](#guide-to-arduino-r4-wifi-rtc)
  - [Key RTC Data Types](#key-rtc-data-types)
  - [Essential RTC Methods](#essential-rtc-methods)
- [Using the LED Matrix Display](#using-the-led-matrix-display)
  - [Basic Setup](#basic-setup)
  - [Display Patterns](#display-patterns)
  - [Best Practices](#best-practices)
  - [Matrix Properties](#matrix-properties)
- [Combining RTC with Animation Timing](#combining-rtc-with-animation-timing)
- [RTC Example Projects Overview](#rtc-example-projects-overview)
- [Notifier Objects for Animating Servos / LEDs Featur](#notifier-objects-for-animating-servos--leds-featur)
  - [Core Components](#core-components)
  - [Basic Usage](#basic-usage)
  - [Advanced Features](#advanced-features)

## Overview

This library introduces methods and examples for Project 3 that will assist in : 
- Reading / Using information from the Real Time Clock
- Controlling Servos and other actuators as outputs
- Drawing the the LED Matrix for Debugging

## Installing the Library

- Open the Arduino Library Manager
- Search YouveBeenNotified or DIGF
- Press the **Install** Button *Note* If you have previously installed the library, press **Update**

## Using the LED Matrix for Debugging

For this project, we will be using the LED Matrix for Debugging and back-end information

- This will use the ArduinoAnimation library, which should already be installed
- A specific guide for using this library with the R4 Wifi is available here [Arduino Graphics Guide](ArduinoGraphics_R4.md)

## Real Time Clock (RTC) on Arduino R4 WiFi

The Arduino R4 WiFi features a built-in Real Time Clock (RTC) that allows your projects to track time even when disconnected from the internet. When working with time-based projects that need to run for extended periods (like our 1-hour requirement), the RTC provides reliable timing for triggering animations.

### Guide to Arduino R4 Wifi RTC
You can find a full overview of the RTC on the Arduino [here](https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc/).

### Key RTC Data Types

#### RTCTime

This class handles time storage and representation:

```cpp
// Create a time object (day, month, year, hours, minutes, seconds, day of week, DST)
RTCTime myTime(14, Month::MARCH, 2025, 12, 30, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_ACTIVE);

// Get values from a time object
int currentHour = myTime.getHour();
int currentMinute = myTime.getMinutes();
int currentSecond = myTime.getSeconds();
```


### Essential RTC Methods

#### Basic Setup

```cpp
void setup() {
  // Initialize the RTC
  RTC.begin();
  
  // Set the initial time (important for alarm functionality)
  RTCTime initialTime(14, Month::MARCH, 2025, 12, 0, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(initialTime);
}
```

#### Getting Current Time

```cpp
void loop() {
  // Create a variable to store the current time
  RTCTime currentTime;
  
  // Get the current time from the RTC
  RTC.getTime(currentTime);
  
  // Use time components
  int hour = currentTime.getHour();
  int minute = currentTime.getMinutes();
  int second = currentTime.getSeconds();
  
  // Display or use the time...
}
```

## Using the LED Matrix Display

The Arduino R4 WiFi includes a built-in 12x8 LED matrix that's perfect for displaying information and debug data. Here are the key methods and patterns for using the display effectively.

### Basic Setup

```cpp
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

void setup() {
  if (!matrix.begin()) {
    Serial.println("Matrix initialization failed!");
    while (1);
  }
}
```

### Display Patterns

#### Showing Numbers

```cpp
void displayNumber(int value) {
  matrix.beginDraw();
  matrix.clear();
  
  // Configure text properties
  matrix.stroke(0xFFFFFFFF);
  matrix.textFont(Font_5x7);
  
  // Center the number
  String numStr = String(value);
  int xPos = (matrix.width() - (numStr.length() * 5)) / 2;
  matrix.text(numStr, xPos, 1);
  
  matrix.endDraw();
}
```

#### Progress Indicator

```cpp
void showProgress(int percentage) {
  matrix.beginDraw();
  matrix.clear();
  
  // Map percentage to matrix width
  int fillWidth = map(percentage, 0, 100, 0, matrix.width());
  
  // Draw progress bar
  for(int x = 0; x < fillWidth; x++) {
    matrix.point(x, 3);
    matrix.point(x, 4);
  }
  
  matrix.endDraw();
}
```

#### Status Indicators

```cpp
void showState(int state) {
  matrix.beginDraw();
  matrix.clear();
  
  switch(state) {
    case 0:  // Idle - circle
      matrix.circle(6, 4, 3);
      break;
    case 1:  // Active - filled circle
      matrix.fill(0xFFFFFFFF);
      matrix.circle(6, 4, 3);
      break;
    case 2:  // Error - X
      matrix.line(3, 2, 8, 6);
      matrix.line(3, 6, 8, 2);
      break;
  }
  
  matrix.endDraw();
}
```

#### Scrolling Messages

```cpp
void showScrollingMessage(const char* message) {
  matrix.beginDraw();
  matrix.clear();
  
  matrix.stroke(0xFFFFFFFF);
  matrix.textFont(Font_4x6);
  matrix.textScrollSpeed(75);  // Milliseconds between scroll steps
  
  matrix.beginText(0, 1);
  matrix.print(message);
  matrix.endText(SCROLL_LEFT);
  
  matrix.endDraw();
}
```

### Best Practices

1. **Drawing Operations**
   - Always wrap drawing code between `beginDraw()` and `endDraw()`
   - Clear the display with `clear()` before drawing new content
   - Use `stroke()` to set the LED state (0xFFFFFFFF for on)

2. **Text Display**
   - Use `Font_5x7` for numbers and large text
   - Use `Font_4x6` for scrolling messages
   - Center text by calculating position based on string length

3. **Animation**
   - Use `millis()` for timing instead of `delay()`
   - Keep scrolling messages brief
   - Update display only when content changes

4. **Debug Information**
   - Show critical states with distinct patterns
   - Use progress bars for percentage-based data
   - Implement simple animations for active/idle states

### Matrix Properties

- Width: 12 pixels (`matrix.width()`)
- Height: 8 pixels (`matrix.height()`)
- Available Fonts: 
  - `Font_4x6` (compact)
  - `Font_5x7` (standard)
- Scroll Directions:
  - `SCROLL_LEFT`
  - `SCROLL_RIGHT`

For more detailed information about matrix drawing methods, see the [Arduino Graphics Guide](ArduinoGraphics_R4.md).

## Combining RTC with Animation Timing

Here's how to set up a project that triggers different animations at specific minutes during an hour:

```cpp
// Global flags and variables
volatile bool minuteChangeFlag = false;
int lastMinute = -1;
bool animationTriggers[4] = {false, false, false, false}; // For 4 specific times

// Alarm callback for start of each minute
void minuteAlarmCallback() {
  minuteChangeFlag = true;
}

void setup() {
  // Initialize RTC and set time
  RTC.begin();
  RTCTime initialTime(14, Month::MARCH, 2025, 0, 0, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(initialTime);
  
  // Set up alarm for minute changes
  RTCTime alarmTime;
  alarmTime.setSecond(0);
  AlarmMatch matchTime;
  matchTime.addMatchSecond();
  RTC.setAlarmCallback(minuteAlarmCallback, alarmTime, matchTime);
  
  // Initialize animations, LED matrix, etc.
  // ...
}

void loop() {
  // Handle minute change flag from interrupt
  if (minuteChangeFlag) {
    minuteChangeFlag = false;
    checkTimeEvents();
  }
  
  // Update animations
  updateAnimations();
}

void checkTimeEvents() {
  // Get current time
  RTCTime currentTime;
  RTC.getTime(currentTime);
  int currentMinute = currentTime.getMinutes();
  
  // Check for specific minute triggers
  switch (currentMinute) {
    case 10: // At 10 minutes past the hour
      animationTriggers[0] = true;
      break;
    case 25: // At 25 minutes past the hour
      animationTriggers[1] = true;
      break;
    case 40: // At 40 minutes past the hour
      animationTriggers[2] = true;
      break;
    case 55: // At 55 minutes past the hour
      animationTriggers[3] = true;
      break;
  }
  
  // Check if minute has changed to update the regular animation
  if (currentMinute != lastMinute) {
    lastMinute = currentMinute;
    changeAnimation(currentMinute % 5); // Change animation every 5 minutes
  }
}

void updateAnimations() {
  // Check and reset special animation triggers
  for (int i = 0; i < 4; i++) {
    if (animationTriggers[i]) {
      animationTriggers[i] = false;
      playSpecialAnimation(i);
    }
  }
  
  // Update current animation
  // ...your animation update code...
}

void changeAnimation(int index) {
  // Code to switch to a different animation based on index
}

void playSpecialAnimation(int index) {
  // Code to play a special animation based on index
}
```

## RTC Example Projects Overview

The examples folder contains several demonstrations of RTC functionality combined with matrix animations:

### RTC_01_CountMinutes
Demonstrates basic RTC polling to track and display minutes on the LED matrix.
- Counts up to 60 or down from 60 (configurable direction)
- Displays the current minute count on the LED matrix
- Uses polling technique to check time at regular intervals
- Includes debug output through Serial Monitor

### RTC_02_CycleShapes
Shows how to change displayed patterns on the LED matrix based on minute changes.
- Cycles through 5 different shapes: rectangle outline, filled rectangle, circle outline, filled circle, crossed lines
- Changes to next shape at the start of each minute
- Includes debug output showing current time and shape index
- Demonstrates simple shape drawing on the matrix display

### RTC_03_IncrementLEDs
Visualizes time passage by sequentially illuminating LEDs across the matrix.
- Each minute lights up one additional LED in sequence
- Fills matrix from left-to-right, top-to-bottom
- Resets when matrix is completely filled
- Shows how to map linear time to 2D display coordinates

### RTC_04_TimeCue
Demonstrates triggering special events at specific times.
- Displays minute counting similar to RTC_01_CountMinutes
- Adds a special "cue" function that activates at a configured minute
- When cue triggers, the entire matrix illuminates as a visual alert
- Shows how to implement time-based event triggers

Each example provides a foundation for your own time-based projects, from simple counting to timed event triggers that can be used with servo animations and LED notifications.





## Notifier Objects for Animating Servos / LEDs Featur

- Keyframe-based animation system for precise motion and light control
- Compatible with servos, LEDs, and NeoPixel RGB strips
- Multiple playback modes (once, loop, boomerang)
- Smooth transitions between animations
- Dynamic speed control
- Value scaling and range limiting
- Multiple named animations per controller
- Library of RGB color animations
- Simple to use for beginners, powerful for advanced users



### Core Components

#### BaseNotifier Class
Base class that handles animation timing, interpolation, and playback control.

```cpp
// Create animations
KeyframeAnimation waveMotion("wave");
waveMotion.addKeyFrame(0, 0);      // Start at 0 degrees
waveMotion.addKeyFrame(90, 1000);  // Move to 90 degrees over 1 second

// Get animation values and control playback
// (Typically you'll use a specialized notifier like ServoNotifier)
BaseNotifier animator;
animator.addAnimation(waveMotion);
animator.playAnimation("wave", LOOP);
animator.update();
float value = animator.getValue();
```

#### ServoNotifier Class
Specialized animation controller for servo motors.

```cpp
// Create a servo animator
ServoNotifier servoAnimator(myServo);  // Reference to Servo object

// Add and play animation
servoAnimator.addAnimation(waveMotion);
servoAnimator.playAnimation("wave", LOOP);

// In loop, update animation and apply to servo
servoAnimator.update();
myServo.write(servoAnimator.getValue());  // Apply value to servo
```

#### LEDNotifier Class
Handles LED animation control with both analog (PWM) and digital modes.

```cpp
// Create an LED controller (pin, mode)
LEDNotifier ledAnimator(10, ANALOG);  // PWM on pin 10
// Or for digital LED:
// LEDNotifier statusLED(13, DIGITAL);

// Initialize
ledAnimator.begin();

// Create and add animation
KeyframeAnimation breathing("breathing");
breathing.addKeyFrame(0, 0);      // Off
breathing.addKeyFrame(255, 1000); // Fade in over 1 second
breathing.addKeyFrame(0, 2000);   // Fade out over 1 second

ledAnimator.addAnimation(breathing);

// Play animation
ledAnimator.playAnimation("breathing", LOOP);

// In loop, just update (it controls the LED directly)
ledAnimator.update();
```

#### RGBKeyframeAnimation Class
Stores a sequence of RGB color keyframes for NeoPixel animations.

```cpp
// Create a color animation
RGBKeyframeAnimation rainbow("rainbow");

// Add color keyframes (r, g, b, time in ms)
rainbow.addKeyFrame(255, 0, 0, 0);      // Red
rainbow.addKeyFrame(0, 255, 0, 1000);   // Fade to Green over 1s
rainbow.addKeyFrame(0, 0, 255, 2000);   // Fade to Blue over 1s
rainbow.addKeyFrame(255, 0, 0, 3000);   // Fade back to Red over 1s
```

#### RGBLEDNotifier Class
Manages RGB LED strip animation for NeoPixels.

```cpp
// Create RGB animator (pin, number of pixels)
RGBLEDNotifier pixelAnimator(6, 8);  // 8 pixels on pin 6

// Initialize
pixelAnimator.begin();

// Add animation
pixelAnimator.addAnimation(rainbow);

// Play animation
pixelAnimator.playAnimation("rainbow", LOOP);

// Update animation (handles NeoPixel control)
pixelAnimator.update();
```

### PlayMode Types
Animation playback modes (matching TinyFilmFestival):

- `ONCE`: Play animation once and stop
- `LOOP`: Play animation in a continuous loop
- `BOOMERANG`: Play animation forward then backward repeatedly

### AnimationState Types
Animation state tracking (matching TinyFilmFestival):

- `IDLE`: No animation loaded
- `PLAYING`: Animation playing
- `PAUSED`: Animation paused
- `COMPLETED`: Animation finished (PLAY_ONCE mode)

## Basic Usage

### Servo Animation

```cpp
#include <YouveBeenNotified.h>
#include <Servo.h>

// 1. Create hardware and notifier objects
Servo myServo;
ServoNotifier servoAnimator(myServo);

// 2. Create animation variable
KeyframeAnimation waveMotion("wave");

void setup() {
  // Setup hardware
  myServo.attach(9);
  
  // 3. Add animation to notifier
  servoAnimator.addAnimation(waveMotion);
  
  // 4. Define keyframes
  waveMotion.addKeyFrame(0, 0);      // Start at 0 degrees
  waveMotion.addKeyFrame(90, 1000);  // Move to 90 degrees over 1 second
  waveMotion.addKeyFrame(180, 2000); // Move to 180 degrees over 1 second
  
  // 5. Start animation
  servoAnimator.playAnimation("wave", LOOP);
  
  // Optional: limit range for safety
  servoAnimator.setValueRange(10, 170);
}

void loop() {
  // 6. Update animation
  servoAnimator.update();
  
  // Apply to servo
  myServo.write(servoAnimator.getValue());
}
```

### LED Animation

```cpp
#include <YouveBeenNotified.h>

// 1. Create notifier object
LEDNotifier ledAnimator(10, ANALOG);  // PWM on pin 10

// 2. Create animation variable
KeyframeAnimation breathing("breathing");

void setup() {
  // Initialize hardware
  ledAnimator.begin();
  
  // 3. Add animation to notifier
  ledAnimator.addAnimation(breathing);
  
  // 4. Define keyframes
  breathing.addKeyFrame(0, 0);      // Off
  breathing.addKeyFrame(255, 1000); // Fade in over 1 second
  breathing.addKeyFrame(0, 2000);   // Fade out over 1 second
  
  // 5. Start animation
  ledAnimator.playAnimation("breathing", LOOP);
}

void loop() {
  // 6. Update animation - LED is controlled internally
  ledAnimator.update();
}
```

### RGB LED Animation

```cpp
#include <YouveBeenNotified.h>
#include <Adafruit_NeoPixel.h>

// 1. Create hardware objects and notifier
#define PIXEL_PIN 6
#define PIXEL_COUNT 8
Adafruit_NeoPixel pixels(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
RGBLEDNotifier pixelAnimator;

// 2. Create animation variable
RGBKeyframeAnimation rainbow("rainbow");

void setup() {
  // Initialize NeoPixel hardware
  pixels.begin();
  pixels.show();
  
  // 3. Add animation to notifier
  pixelAnimator.addAnimation(rainbow);
  
  // 4. Define keyframes
  rainbow.addKeyFrame(255, 0, 0, 0);      // Red
  rainbow.addKeyFrame(0, 255, 0, 1000);   // Green after 1s
  rainbow.addKeyFrame(0, 0, 255, 2000);   // Blue after 2s
  rainbow.addKeyFrame(255, 0, 0, 3000);   // Red after 3s
  
  // 5. Start animation
  pixelAnimator.playAnimation("rainbow", LOOP);
}

void loop() {
  // 6. Update animation calculations
  pixelAnimator.update();
  
  // Apply to NeoPixels directly with color value
  for(int i=0; i<PIXEL_COUNT; i++) {
    pixels.setPixelColor(i, pixelAnimator.getColor());
  }
  pixels.show();
}
```

## Advanced Features

### Animation Blending

Smoothly transition between animations:

```cpp
// Switch to "alert" animation with a 500ms crossfade
servoAnimator.crossfadeTo("alert", 500, LOOP);
```

### Animation Timing

Track animation progress:

```cpp
// Get time remaining to next keyframe (in milliseconds)
unsigned long timeToNext = servoAnimator.getTimeToNextKeyframe();

// Get time remaining until animation completes (in milliseconds)
unsigned long timeToEnd = servoAnimator.getTimeToCompletion();

// Check if animation has completed
if (servoAnimator.isCompleted()) {
  // Animation has finished (for PLAY_ONCE mode)
  // Start next animation
  servoAnimator.playAnimation("nextAnimation", LOOP);
}
```

### Value Adjustment

Adjust output values for fine control:

```cpp
// Scale output values
servoAnimator.setValueScale(0.5);  // Half the movement range

// Offset values
servoAnimator.setValueOffset(10);  // Add 10 to all values

// Limit range
servoAnimator.setValueRange(10, 170);  // Clamp to 10-170 degrees
```

### Dynamic Keyframe Modification

Change animations on-the-fly:

```cpp
// Change a keyframe value 
waveMotion.setKeyFrameValue(2, 120);  // Limit max angle

// Change a keyframe timing
breathing.setKeyFrameTime(1, 2000);  // Slow down fade-in

// Change RGB keyframe color
rainbow.setKeyFrameColor(1, 255, 255, 0);  // Change to yellow
```

