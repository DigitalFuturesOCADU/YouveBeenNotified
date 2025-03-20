# You've Been Notified - Arduino Animation Library
A library for creating time-based animations for Arduino R4 WiFi...for one hour.

![Servo Component Example](/images/servoComponents.gif)

*Image by [Rizwan Syed](https://grabcad.com/library/micro-servo-motor-with-22-awg-wires-1)*


## Table of Contents
- [Introduction](#introduction)
- [Installing the Library](#installing-the-library)
- [Real Time Clock (RTC)](#real-time-clock-rtc)
    - [Key RTC Data Types](#key-rtc-data-types)
        - [RTCTime](#rtctime)
    - [Essential RTC Methods](#essential-rtc-methods)
        - [Basic Setup](#basic-setup)
        - [Getting Current Time](#getting-current-time)
- [Using the LED Matrix Display](#using-the-led-matrix-display)
    - [Basic Setup](#basic-setup-1)
    - [Display Patterns](#display-patterns)
        - [Showing Numbers](#showing-numbers)
        - [Progress Indicator](#progress-indicator)
    - [Best Practices](#best-practices)
- [Servo Motors](#servo-motors)
    - [Servo Basics](#servo-basics)
        - [Types of Servos](#types-of-servos)
        - [Specifications for Project Servo](#specifications-for-project-servo-sg90)
        - [Movement Range](#movement-range)
        - [Size and Power](#size-and-power)
        - [Wiring](#wiring)
        - [Arduino Servo Library](#arduino-servo-library)
    - [Mechanisms and Attachments](#mechanisms-and-attachments)
- [Notifier Animation System](#notifier-animation-system)
    - [Core Components](#core-components)
        - [KeyframeAnimation](#keyframeanimation)
        - [ServoNotifier](#servonotifier)
    - [Status and Information Methods](#status-and-information-methods)
    - [Animation Playback Modes](#animation-playback-modes)
    - [Step-by-Step Animation Creation](#step-by-step-animation-creation)
    - [Basic Usage](#basic-usage)
    - [Advanced Features](#advanced-features)
        - [Multiple Animations](#multiple-animations)
        - [Animation Speed Control](#animation-speed-control)
        - [Crossfading](#crossfading)
- [Example Projects](#example-projects)

## Introduction

This library introduces methods for Project 3 to assist with:
- Reading and using information from the Real Time Clock
- Controlling servos and other actuators as outputs
- Creating keyframe-based animations for smooth motion control
- Using the LED Matrix for debugging and visualization

The library combines time-based triggers with keyframe animation capabilities, allowing you to create complex kinetic projects.

## Installing the Library

1. Open the Arduino Library Manager
2. Search for "YouveBeenNotified" or "DIGF"
3. Press the **Install** Button (*Note*: If you have previously installed the library, press **Update**)

## Real Time Clock (RTC)

The Arduino R4 WiFi features a built-in Real Time Clock (RTC) that allows your projects to track time even when disconnected from the internet. When working with time-based projects that need to run for extended periods (like our 1-hour requirement), the RTC provides reliable timing for triggering animations.

For a full overview of the RTC on the Arduino R4 WiFi, see the [official documentation](https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc/).

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

The Arduino R4 WiFi's 12x8 LED matrix is useful for displaying information and debug data. For detailed documentation on the ArduinoGraphics library for the LED matrix, see the [Arduino Graphics Guide](ArduinoGraphics_R4.md).

### Basic Setup

```cpp
#include "ArduinoGraphics.h" //must be defined before Arduino_LED_Matrix.h
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

Here are some common display patterns you can use:

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

## Servo Motors

### Servo Basics

Servo motors provide precise angular control for your projects and are ideal for creating physical notifications and interactions.

#### Types of Servos
- **Positional (Hobby) Servos**: Can rotate to specific angles (typically 0-180°). These are the type we're using in this project.
- **Continuous Rotation Servos**: Modified to rotate continuously like a normal motor, but with speed control rather than position control.

#### Specifications for Project Servo (SG90)

The [SG90 Micro Servo](https://www.dfrobot.com/product-255.html) used in this project has these key specifications:
- Operating voltage: 4.8V to 6V
- Operating speed: 0.1 sec/60° at 4.8V
- Stall torque: 1.6 kg-cm at 4.8V
- Weight: 9g
- Dimensions: 22.2 x 11.8 x 31 mm
- Angular range: 0-180 degrees


#### Movement Range
- Standard servo range is 0-180 degrees
- Servo horns (attachments) can modify the effective range of movement when connected to mechanisms

#### Size and Power
- We're using micro servos in this project, which are small and lightweight
- Each servo has a speed (how fast can it move) and a torque (how much mass can it move) rating. Consult it's documentation

#### Wiring
A standard servo has three wires:
- Brown/Black: Ground (GND)
- Red: Power (+5V)
- Yellow/Orange: Control/Signal (connect to any Arduino pin)

![Servo Pin Diagram](/images/servoPins.jpg)
*Typical servo wiring diagram showing the three essential connections*

#### Arduino Servo Library

The Servo library comes pre-installed with the Arduino IDE, so you don't need to install it through the Library Manager. Just include it in your sketch and you're ready to go.

**Key Servo Commands:**

- `attach(pin)`: Connects the servo object to the specified pin. Also accepts optional min/max pulse width parameters to calibrate the servo range.
- `detach()`: Disconnects the servo from its pin. This disengages the motor, but keeps a connection to arduino. Can be re-connected by calling attach() again
- `write(angle)`: Moves the servo to the specified angle (0-180 degrees).
- `read()`: Returns the last angle written to the servo (0-180).

Here's a simple example that moves a servo between two positions without using delay():

```cpp
#include <Servo.h>

Servo myServo;           // Create a servo object
int angle = 0;           // Current servo position
unsigned long prevTime;  // For tracking elapsed time
int interval = 1000;     // Time between position changes (ms)

void setup() {
  myServo.attach(9);     // Attach servo to pin 9
  prevTime = millis();   // Initialize timing
}

void loop() {
  // Non-blocking timing approach
  unsigned long currentTime = millis();
  
  if (currentTime - prevTime >= interval) {
    prevTime = currentTime;  // Reset the timer
    
    // Toggle between 0 and 90 degrees
    angle = (angle == 0) ? 90 : 0;
    
    // Move the servo
    myServo.write(angle);
  }
  
  // Other Things
}
```

### Mechanisms and Attachments

Servos can be connected to various mechanisms to create different types of movement:
- **Direct attachment**: The servo horn directly moves the notification element
- **Linkages**: Connect the servo to rods or arms that amplify or transform the motion
- **Cams**: Create cyclical or repeating patterns of movement
- **Gear systems**: Modify the speed or torque of the servo's movement

## Notifier Animation System

The You've Been Notified library includes a keyframe animation system that makes it easy to create complex servo movements.

### Core Components

#### KeyframeAnimation
A sequence of positions and timing information that defines a motion pattern.

```cpp
// Create an animation named "wave"
KeyframeAnimation waveMotion("wave");

// Add keyframes: (position, time in milliseconds)
waveMotion.addKeyFrame(0, 0);      // Start at 0 degrees
waveMotion.addKeyFrame(90, 1000);  // Move to 90 degrees over 1 second
waveMotion.addKeyFrame(30, 2000);  // Move to 30 degrees over 1 second
```

#### ServoNotifier
Manages animations for servo motors, handling all the timing and interpolation.

```cpp
// Create a servo notifier
ServoNotifier notifier(myServo);  // Pass in your Servo object

// Add the animation to the notifier
notifier.addAnimation(waveMotion);

// Play the animation
notifier.playAnimation("wave", LOOP);  // Name and playback mode
```
### Status and Information Methods

#### Animation Status
- `isPlaying()`: Returns true if an animation is currently active
- `getCurrentAnimationName()`: Returns the name of the current animation
- `getPlaybackMode()`: Returns the current playback mode (ONCE, LOOP, BOOMERANG)
- `getElapsedTime()`: Returns milliseconds since animation started
- `getTotalDuration()`: Returns total length of current animation in milliseconds

#### Value Information
- `getValue()`: Returns the current calculated position value
- `hasChanged()`: Returns true if the value has updated since last check
- `getStartValue()`: Returns the first keyframe value of current animation
- `getEndValue()`: Returns the last keyframe value of current animation

#### Animation Management
- `hasAnimation(name)`: Checks if an animation with given name exists
- `getAnimationCount()`: Returns total number of stored animations
- `getAnimationNames()`: Returns array of all animation names
- `getCurrentSpeed()`: Returns current global speed multiplier

### Animation Playback Modes

- **ONCE**: Plays the animation once and stops
- **LOOP**: Repeats the animation continuously
- **BOOMERANG**: Plays forward, then backward, then repeats

#### Animation Names

Each animation is identified by a unique name string:
- Names must be unique within a notifier
- Used to play, stop, or manage specific animations
- Case-sensitive (e.g., "wave" and "Wave" are different)

```cpp
// Creating animations with names
KeyframeAnimation upDown("upDown");    // Name: "upDown"
KeyframeAnimation wave("wave");        // Name: "wave"

// Playing by name
notifier.playAnimation("upDown", ONCE);
notifier.stopAnimation("wave");

// Check if exists
if (notifier.hasAnimation("upDown")) {
    // Animation exists
}
```
### Step-by-Step Animation Creation

#### Single Animation Example
1. Create the notifier
```cpp
Servo myServo;
ServoNotifier notifier(myServo);
```

2. Create and name an animation
```cpp
KeyframeAnimation wave("wave");
```

3. Add keyframes (position, time in ms)
```cpp
wave.addKeyFrame(0, 0);       // Start position
wave.addKeyFrame(90, 1000);   // Move to 90° over 1 second
wave.addKeyFrame(45, 2000);   // Move to 45° over next second
wave.addKeyFrame(0, 3000);    // Return to start over final second
```

4. Add animation to notifier
```cpp
notifier.addAnimation(wave);
```

5. Start the animation
```cpp
notifier.playAnimation("wave", LOOP);
```

#### Multiple Animations Example
1. Create notifier
```cpp
Servo myServo;
ServoNotifier notifier(myServo);
```

2. Create and name two animations
```cpp
KeyframeAnimation wave("wave");
KeyframeAnimation pulse("pulse");
```

3. Add keyframes to both
```cpp
// Wave animation
wave.addKeyFrame(0, 0);
wave.addKeyFrame(90, 1000);
wave.addKeyFrame(0, 2000);

// Pulse animation
pulse.addKeyFrame(45, 0);
pulse.addKeyFrame(60, 500);
pulse.addKeyFrame(45, 1000);
```

4. Add both animations to notifier
```cpp
notifier.addAnimation(wave);
notifier.addAnimation(pulse);
```

5. Play either animation
```cpp
notifier.playAnimation("wave", LOOP);
// or
notifier.playAnimation("pulse", LOOP);
```
### Basic Usage

```cpp
#include <Servo.h>
#include "YouveBeenNotified.h"

// Create servo and notifier
Servo myServo;
ServoNotifier notifier(myServo);

void setup() {
  // Setup servo
  myServo.attach(9);
  
  // Create animation
  KeyframeAnimation waveMotion("wave");
  waveMotion.addKeyFrame(0, 0);      // Start at 0 degrees
  waveMotion.addKeyFrame(90, 1000);  // Move to 90 degrees over 1 second
  waveMotion.addKeyFrame(30, 2000);  // Move to 30 degrees over 1 second
  
  // Add animation to notifier
  notifier.addAnimation(waveMotion);
  
  // Start playing in loop mode
  notifier.playAnimation("wave", LOOP);
}

void loop() {
  // Update animation calculations
  notifier.update();
  
  // If the servo value has changed, update the servo
  if (notifier.hasChanged()) {
    myServo.write(notifier.getValue());
  }
}
```

### Advanced Features

#### Multiple Animations
You can create and switch between multiple animations:

```cpp
// Create multiple animations
KeyframeAnimation wave("wave");
KeyframeAnimation pulse("pulse");

// Add both to notifier
notifier.addAnimation(wave);
notifier.addAnimation(pulse);

// Switch between them
notifier.playAnimation("wave", LOOP);
// Later...
notifier.playAnimation("pulse", ONCE);
```

#### Animation Speed Control
Adjust playback speed without changing the keyframes:

```cpp
// Play at double speed
notifier.setGlobalSpeed(2.0);

// Half speed
notifier.setGlobalSpeed(0.5);
```

#### Crossfading
Smoothly transition between animations:

```cpp
// Blend from current animation to "pulse" over 500ms
notifier.crossfadeTo("pulse", 500, LOOP);
```

## Example Projects

The library includes several example projects to help you get started:

### [RTC Basics Examples](RTC_Basics_Examples.md)
Simple examples showing how to use the Real Time Clock for timing events.

### [RTC Simple Servo Examples](RTC_Servo_Examples.md)
Examples combining RTC with basic servo control.

### [RTC with Animation System Examples](RTC_YBN_Examples.md)
Advanced examples using RTC to trigger animations with the ServoNotifier system.

### [Animation System Basics Examples](YBN_Basics_Examples.md)
Focused examples demonstrating the core animation capabilities.