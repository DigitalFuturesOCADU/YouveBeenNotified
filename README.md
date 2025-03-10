## Getting Started

Creating animations with YouveBeenNotified follows a simple step-by-step process:

1. **Create Notifier Objects**
   ```cpp
   // Create hardware objects
   Servo myServo;
   
   // Create animation controllers
   ServoNotifier servoAnimator(myServo);
   LEDNotifier ledAnimator(10, ANALOG);
   ```

2. **Create Animation Variables**
   ```cpp
   // Create named animations
   KeyframeAnimation waveMotion("wave");
   KeyframeAnimation breathingLight("breathing");
   ```

3. **Add Animations to Notifiers**
   ```cpp
   // Add animations to controllers
   servoAnimator.addAnimation(waveMotion);
   ledAnimator.addAnimation(breathingLight);
   ```

4. **Define Keyframes**
   ```cpp
   // Define servo animation
   waveMotion.addKeyFrame(0, 0);      // Start at 0 degrees
   waveMotion.addKeyFrame(90, 1000);  // Move to 90 degrees over 1 second
   waveMotion.addKeyFrame(0, 2000);   // Return to 0 degrees over 1 second
   
   // Define LED animation
   breathingLight.addKeyFrame(0, 0);      // Start off
   breathingLight.addKeyFrame(255, 1000); // Fade to full brightness over 1 second
   breathingLight.addKeyFrame(0, 2000);   // Fade to off over 1 second
   ```

5. **Start Animations**
   ```cpp
   // Start animations
   servoAnimator.playAnimation("wave", LOOP);
   ledAnimator.playAnimation("breathing", LOOP);
   ```

6. **Update in Loop**
   ```cpp
   void loop() {
     // Update animations
     servoAnimator.update();
     ledAnimator.update();
     
     // Apply servo value to hardware (for servo)
     myServo.write(servoAnimator.getValue());
   }
   ```### KeyframeAnimation Class
Stores a sequence of value/time keyframes for single-value animations.

```cpp
// Create an animation with a name
KeyframeAnimation waveMotion("wave");

// Add keyframes (value, time in ms)
waveMotion.addKeyFrame(0, 0);      // Start at 0 degrees
waveMotion.addKeyFrame(90, 1000);  // Move to 90 degrees over 1 second
waveMotion.addKeyFrame(180, 2000); // Move to 180 degrees over 1 second
```# YouveBeenNotified Library

| ![Servo Animation](docs/servowave.gif) | ![LED Breathing](docs/ledbreathing.gif) |
|------------------------------------|------------------------------------|
| ![RGB Rainbow](docs/rgbrainbow.gif) | ![RGB Fire](docs/rgbfire.gif) |

YouveBeenNotified is a library that enables keyframe-based animation control for servos, LEDs, and RGB LED strips on Arduino. It offers a game-like animation system with smooth transitions, blending, and precise timing control. This library is designed for physical computing courses, making it accessible for beginners while offering advanced features for complex projects.

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Core Components](#core-components)
    - [KeyframeAnimation Class](#keyframeanimation-class)
    - [YouveBeenNotified Class](#youvebeennotified-class)
    - [LEDNotifier Class](#lednotifier-class)
    - [RGBKeyframeAnimation Class](#rgbkeyframeanimation-class)
    - [RGBLEDNotifier Class](#rgblednotifier-class)
    - [PlayMode Types](#playmode-types)
    - [AnimationState Types](#animationstate-types)
- [Basic Usage](#basic-usage)
    - [Servo Animation](#servo-animation)
    - [LED Animation](#led-animation)
    - [RGB LED Animation](#rgb-led-animation)
- [Advanced Features](#advanced-features)
    - [Animation Blending](#animation-blending)
    - [Value Adjustment](#value-adjustment)
    - [Dynamic Keyframe Modification](#dynamic-keyframe-modification)
- [Examples](#examples)

## Features

- Keyframe-based animation system for precise motion and light control
- Compatible with servos, LEDs, and NeoPixel RGB strips
- Multiple playback modes (once, loop, boomerang)
- Smooth transitions between animations
- Dynamic speed control
- Value scaling and range limiting
- Multiple named animations per controller
- Library of RGB color animations
- Simple to use for beginners, powerful for advanced users

## Installation

1. Download the latest release as a ZIP file
2. In the Arduino IDE, go to Sketch > Include Library > Add .ZIP Library
3. Select the downloaded ZIP file
4. The library and examples will be available under "YouveBeenNotified"

## Core Components

### BaseNotifier Class
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

### ServoNotifier Class
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

### LEDNotifier Class
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

### RGBKeyframeAnimation Class
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

### RGBLEDNotifier Class
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

## Examples

The library includes several examples to help you get started:

- **SimpleServoLED**: Basic servo and LED animation control
- **NeoPixelAnimation**: RGB LED strip animation control
- **AnimationBlending**: Smooth transitions between animations
- **KeyframeModification**: Dynamic animation adjustment
- **MultipleAnimations**: Managing multiple animations in one project

Explore these examples through the Arduino IDE under File > Examples > YouveBeenNotified.
