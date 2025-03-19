# RTC with Animation System Examples

These examples demonstrate how to combine the Real Time Clock (RTC) with the ServoNotifier animation system to create sophisticated time-based animations. These are the most advanced examples in the library, showing the full power of the combined timing and animation capabilities.

## Table of Contents
- [RTC-YBN_01_1Servo_simpleAsPossible](#rtc-ybn_01_1servo_simpleaspossible)
- [RTC-YBN_01_1Servo_timeCues](#rtc-ybn_01_1servo_timecues)
- [RTC-YBN_02_2Servo_simpleAsPossible](#rtc-ybn_02_2servo_simpleaspossible)
- [RTC-YBN_04_2Servo_1Switch1Constant](#rtc-ybn_04_2servo_1switch1constant)

## RTC-YBN_01_1Servo_simpleAsPossible

**Description**: The simplest example of combining RTC with ServoNotifier animations, alternating between two animations every minute.

**Key Features**:
- Animation 1 plays on even minutes, Animation 2 plays on odd minutes
- Uses the modulo operator (%) to determine even/odd minutes
- Wave animation: A gentle motion between 45 and 135 degrees
- Sweep animation: Full range motion from 0 to 180 degrees
- Both animations use LOOP mode to repeat continuously

**Key Functions**:
- `setup()`: Initializes the servo and creates two animations (Wave and Sweep)
- `loop()`: Checks the current minute and switches animations when the minute changes

**Key Variables**:
- `myServo`: Servo object to be controlled
- `notifier`: ServoNotifier that handles the animations
- `anim1Name`/`anim2Name`: String variables storing the names of the animations
- `lastMinute`: Stores the last minute value to detect changes
- `minuteOffset`: Value added to the minute to make testing different times easier
- `playSpeed`: Controls animation playback speed

## RTC-YBN_01_1Servo_timeCues

**Description**: A comprehensive example showing how to trigger different animations at specific minute "cues," with LED matrix display to show current animation status.

**Key Features**:
- Four different animations with different playback modes (ONCE, LOOP, BOOMERANG)
- Each animation is triggered at a specific "cue" minute
- LED matrix displays the current animation name or playback mode
- Demonstrates animation completion handling, especially for ONCE mode

**Key Functions**:
- `checkTimeCues()`: Checks if the current minute matches a cue time
- `setupAnimations()`: Creates the 4 different animations for the servo
- `showTimeDebug()`: Displays current time and animation status
- `playAnimationForCue()`: Plays the appropriate animation for the current cue
- `updateMatrixDisplay()`: Updates the LED matrix with current animation info

**Key Variables**:
- `animationSpeed`: Controls the playback speed of all animations
- `startOffset`: Value added to the minute to make testing different cues easier
- `cueMinutes[]`: Array of specific minutes when animations should trigger
- `myServo`: Servo object to be controlled
- `notifier`: ServoNotifier that handles the animations
- `matrix`: ArduinoLEDMatrix object for the display
- `animPlaying`: Tracks which animation is currently playing

## RTC-YBN_02_2Servo_simpleAsPossible

**Description**: Controls two servos with different animation patterns - one alternates between animations each minute, while the other continuously oscillates.

**Key Features**:
- Servo 1: Alternates between "Wave" and "Sweep" animations based on even/odd minutes
- Servo 2: Continuously runs a slow oscillation animation independent of the minute changes
- Demonstrates independent control of multiple animated servos
- Combines time-based and continuous animations

**Key Functions**:
- `setupServo1Animations()`: Creates two animations for the first servo
- `setupServo2Animation()`: Creates the continuous oscillation for the second servo
- `loop()`: Updates both animations and handles minute-based switching for Servo 1

**Key Variables**:
- `servo1`/`servo2`: Servo objects to be controlled
- `notifier1`/`notifier2`: ServoNotifier objects that handle the animations
- `servoPin1`/`servoPin2`: Pins connected to the servos
- `minuteOffset`: Value added to the current minute for easier testing
- `playSpeed`: Controls animation speed for both servos
- `lastMinute`: Stores the last minute value to detect changes

## RTC-YBN_04_2Servo_1Switch1Constant

**Description**: Similar to the 2Servo_simpleAsPossible example, but with cleaner implementation and more debug output.

**Key Features**:
- Servo 1: Alternates between "Wave" and "Sweep" animations based on even/odd minutes
- Servo 2: Continuously runs a slow oscillation animation
- Improved debug output for monitoring animation states
- Demonstrates how to use two independent ServoNotifier instances

**Key Functions**:
- `setupServo1Animations()`: Creates two animations for the first servo
- `setupServo2Animation()`: Creates the continuous oscillation animation
- `loop()`: Updates both servos and handles minute-based animation switching

**Key Variables**:
- `servo1`/`servo2`: Servo objects to be controlled
- `notifier1`/`notifier2`: ServoNotifier objects for animations
- `minuteOffset`: Value added to the current minute
- `playSpeed`: Controls animation speed
- `lastMinute`: Stores the last minute value