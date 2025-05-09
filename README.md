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
- [Servo Motors](#servo-motors)
    - [Servo Basics](#servo-basics)
        - [Types of Servos](#types-of-servos)
        - [Specifications for Project Servo](#specifications-for-project-servo-sg90)
        - [Movement Range](#movement-range)
        - [Size and Power](#size-and-power)
        - [Wiring](#wiring)
        - [Arduino Servo Library](#arduino-servo-library)
    - [Mechanisms and Attachments](#mechanisms-and-attachments)
    - [Wiring Diagrams](#wiring-diagrams)
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
    - [Animation Playback Controls](#animation-playback-controls)
- [Example Projects](#example-projects)
- [Using the LED Matrix Display](#using-the-led-matrix-display)
    - [Basic Setup](#basic-setup-1)
    - [Display Patterns](#display-patterns)
        - [Showing Numbers](#showing-numbers)
        - [Progress Indicator](#progress-indicator)
    - [Best Practices](#best-practices)

## Introduction

This library introduces methods for Project 3 to assist with:
- Reading and using information from the Real Time Clock
- Controlling servos and other actuators as outputs
- Creating keyframe-based animations for smooth motion control
- Using the LED Matrix for debugging and visualization

The library combines time-based triggers with keyframe animation capabilities, allowing you to create complex kinetic projects.

## Installing the Library

**Current version: 1.1.6**

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

For complete wiring diagrams for both single and dual servo configurations, see the [Wiring Diagrams](Wiring_Diagrams.md) guide.

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

**Mechanical**
- **Linear Actuator**: Converts rotational motion to linear (straight line) movement. See [Linear Actuator Guide](linear-actuator.md) for build instructions.
- **[Kinetic Wearables Toolkit](https://github.com/socialbodylab/KineticWearablesToolkit)**: Collection of ready-made wearable servo mechanisms and attachments optimized for attaching to the body.
- **[Thingiverse Micro Servo Mechanisms](https://www.thingiverse.com/search?q=micro+servo&page=1)**: Collection of 3D printable mechanisms and attachments designed for micro servos

**Other Materials**

- **Paper and Card Stock**: Create fluttering wings, fans, or flexible sculptures
- **Fabric and Textiles**: Rippling banners, stretchy materials, or suspended cloth that drapes
- **Natural Materials**: Feathers, leaves, or lightweight branches for organic movement
- **Wire and Mesh**: Bendable sculptures that maintain shape while moving
- **Lightweight Plastics**: Mylar sheets, cellophane, or thin flexible acrylic for fluid motion
- **String/Thread**: Suspended elements that swing, bounce, or create pendulum effects


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

##### Before setup()
1. Create the servo and notifier objects
```cpp
Servo myServo;
ServoNotifier notifier(myServo);
```

2. Create and name an animation
```cpp
KeyframeAnimation wave("wave");
```

##### In setup()
3. Attach servo to pin
```cpp
myServo.attach(9);  // Attach to pin 9 (or your chosen pin)
```

4. Add keyframes (position, time in ms)
```cpp
wave.addKeyFrame(0, 0);       // Start position
wave.addKeyFrame(90, 1000);   // Move to 90° over 1 second
wave.addKeyFrame(45, 2000);   // Move to 45° over next second
wave.addKeyFrame(0, 3000);    // Return to start over final second
```

5. Add animation to notifier
```cpp
notifier.addAnimation(wave);
```

6. Start the animation
```cpp
notifier.playAnimation("wave", LOOP);
```

##### In loop()
7. Update the notifier and servo
```cpp
notifier.update();  // Calculate new positions
if (notifier.hasChanged()) {
  myServo.write(notifier.getValue());
}
```

#### Multiple Animations Example

##### Before setup()
1. Create servo and notifier objects
```cpp
Servo myServo;
ServoNotifier notifier(myServo);

KeyframeAnimation wave("wave");
KeyframeAnimation pulse("pulse");
```

##### In setup()
2. Attach servo
```cpp
myServo.attach(9);  // Attach to pin 9 (or your chosen pin)
```

3. Add keyframes to both animations
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

5. Start with one animation
```cpp
notifier.playAnimation("wave", LOOP);
```

##### In loop()
6. Update animations
```cpp
notifier.update();  // Calculate new positions
if (notifier.hasChanged()) {
  myServo.write(notifier.getValue());
}
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

```cpp
// Example: ServoAnimation_01_Sweep
// Shows how to create a simple servo sweep using
// a single keyframe and boomerang playback mode

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
```

### Advanced Features

#### Multiple Animations

Create and switch between different animation patterns:

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

Adjust playback speed dynamically without modifying keyframes:

```cpp
// Play at double speed
notifier.setGlobalSpeed(2.0);

// Half speed
notifier.setGlobalSpeed(0.5);

// Return to normal speed
notifier.setGlobalSpeed(1.0);
```

#### Crossfading

Smoothly transition between animations for continuous motion:

```cpp
// Blend from current animation to "pulse" over 500ms
notifier.crossfadeTo("pulse", 500, LOOP);

// You can also specify the playback mode for the target animation
notifier.crossfadeTo("wave", 1000, BOOMERANG);
```

### Animation Playback Controls

The library provides several methods to control animation playback:

#### Play Animation
Starts an animation with the specified playback mode:

```cpp
// Play by animation name
notifier.playAnimation("wave", LOOP); // Name and playback mode

// Play by animation reference
notifier.playAnimation(waveMotion, ONCE);
```

#### Pause and Resume
Temporarily pause an animation while preserving its current position:

```cpp
// Pause the current animation
notifier.pause();

// Resume a paused animation
notifier.resume();
```

When paused, the animation will:
- Maintain its current position
- Keep track of how long it's been paused
- Continue from exact same position when resumed
- Adjust timing to account for the pause duration

#### Stop Animation
Completely stops the current animation:

```cpp
// Stop any currently playing animation
notifier.stop();
```

When stopped, the animation will:
- Reset to the IDLE state
- Clear all animation references
- Need to be started again with playAnimation()
- Not maintain its position or timing information

#### Simple Implementation Example

Here's a complete example showing how to implement and use play, pause, and stop controls with button inputs:

```cpp
#include <Servo.h>
#include "YouveBeenNotified.h"

// Create servo and notifier
Servo myServo;
ServoNotifier notifier(myServo);

// Pin assignments
const int servoPin = 9;
const int playButtonPin = 2;
const int pauseButtonPin = 3;
const int stopButtonPin = 4;

// Button state tracking
bool playButtonState = false;
bool pauseButtonState = false;
bool stopButtonState = false;
bool lastPlayButtonState = false;
bool lastPauseButtonState = false;
bool lastStopButtonState = false;

void setup() {
  // Set up Serial for debugging
  Serial.begin(9600);
  
  // Initialize button pins
  pinMode(playButtonPin, INPUT_PULLUP);
  pinMode(pauseButtonPin, INPUT_PULLUP);
  pinMode(stopButtonPin, INPUT_PULLUP);
  
  // Attach servo to pin
  myServo.attach(servoPin);
  
  // Set up a wave animation
  KeyframeAnimation wave("wave");
  wave.addKeyFrame(0, 0);       // Start at 0 degrees
  wave.addKeyFrame(90, 1000);   // Move to 90 degrees over 1 second
  wave.addKeyFrame(45, 2000);   // Move to 45 degrees over 1 second
  wave.addKeyFrame(120, 3000);  // Move to 120 degrees over 1 second
  wave.addKeyFrame(0, 4000);    // Return to 0 degrees over 1 second
  
  // Add animation to notifier
  notifier.addAnimation(wave);
  
  Serial.println("Animation setup complete");
  Serial.println("Press buttons to control: Play, Pause, or Stop");
}

void loop() {
  // Update animation calculations
  notifier.update();
  
  // If the servo value has changed, update the servo position
  if (notifier.hasChanged()) {
    myServo.write(notifier.getValue());
  }
  
  // Read button states (inverted because of INPUT_PULLUP)
  playButtonState = !digitalRead(playButtonPin);
  pauseButtonState = !digitalRead(pauseButtonPin);
  stopButtonState = !digitalRead(stopButtonPin);
  
  // Check for Play button press
  if (playButtonState && !lastPlayButtonState) {
    // Start animation in LOOP mode
    notifier.playAnimation("wave", LOOP);
    Serial.println("Animation started");
  }
  
  // Check for Pause/Resume button press
  if (pauseButtonState && !lastPauseButtonState) {
    if (notifier.isPaused()) {
      // Resume if currently paused
      notifier.resume();
      Serial.println("Animation resumed");
    } else if (notifier.isPlaying()) {
      // Pause if currently playing
      notifier.pause();
      Serial.println("Animation paused");
    }
  }
  
  // Check for Stop button press
  if (stopButtonState && !lastStopButtonState) {
    // Stop the animation
    notifier.stop();
    Serial.println("Animation stopped");
  }
  
  // Update button state history
  lastPlayButtonState = playButtonState;
  lastPauseButtonState = pauseButtonState;
  lastStopButtonState = stopButtonState;
  
  // Print animation status when it changes
  static AnimationState lastState = IDLE;
  if (notifier.getState() != lastState) {
    lastState = notifier.getState();
    
    Serial.print("Animation state: ");
    switch (lastState) {
      case IDLE:
        Serial.println("IDLE");
        break;
      case PLAYING:
        Serial.println("PLAYING");
        break;
      case PAUSED:
        Serial.println("PAUSED");
        break;
      case COMPLETED:
        Serial.println("COMPLETED");
        break;
    }
  }
  
  
}
```

### Animation State Transitions

The animation system uses the following state machine for managing animations:

- **IDLE** → No animation playing (initial state)
- **PLAYING** → Animation is actively running
- **PAUSED** → Animation is temporarily frozen
- **COMPLETED** → Animation (ONCE mode only) has finished

Possible state transitions:
- IDLE → PLAYING: Call `playAnimation()`
- PLAYING → PAUSED: Call `pause()`
- PAUSED → PLAYING: Call `resume()`
- PLAYING → COMPLETED: Automatic when a ONCE animation finishes
- PLAYING → IDLE: Call `stop()`
- PAUSED → IDLE: Call `stop()`
- COMPLETED → IDLE: Call `stop()` or `playAnimation()`

You can check the current state with methods like `isPlaying()`, `isPaused()`, `isCompleted()`, or the more general `getState()` method.

## Example Projects

The library includes several example sketches to demonstrate key concepts:

| Category | Example | Description |
|----------|---------|-------------|
| **RTC Basics** | RTC_01_CountMinutes | Basic minute counter displayed on LED matrix |
| | RTC_02_CycleShapes | Cycle through different shapes on the minute |
| | RTC_03_IncrementLEDs | Sequentially fill LEDs as time passes |
| | RTC_04_TimeCue | Trigger special events at specific times |
| **Servo Control** | RTCservo_01_1Servo_AngleList | Introduction to servo control methods |
| | RTCservo_02_2Servos_AngleList | Move servo based on current time |
| | RTCservo_03_1Servo_RandomAngles | Control servo with random positions |
| | RTCservo_04_2Servos_RandomAngles | Control two servos with random positions |
| **RTC Servo Animations** | RTC_ServoAnimation_01_1ServoSimple | Simple example combining RTC with servo animations |
| | RTC_ServoAnimation_02_2ServosSimple | Control two servo animations with RTC |
| | RTC_ServoAnimation_03_1Servo_TimeCues | Trigger animations at specific minute cues |
| | RTC_ServoAnimation_04_2Servos_1Switch1Constant | Advanced dual servo animation control |

For detailed instructions and code walkthroughs:
- [01 RTC Basics](./01_RTC_Basics.md)
- [02 RTC Simple Servo](./02_RTC_Simple_Servo.md)
- [03 RTC Servo Animations](./03_RTC_Servo_Animations.md)
- [04 YBN Animation Basics](./04_YBN_Animation_Basics.md)

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