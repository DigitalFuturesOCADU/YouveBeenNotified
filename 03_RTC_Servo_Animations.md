# RTC with Animation System Examples

These examples demonstrate how to combine the Real Time Clock (RTC) with the ServoNotifier animation system to create sophisticated time-based animations. These are the most advanced examples in the library, showing the full power of the combined timing and animation capabilities.

## Table of Contents
- [ServoAnimation_00_1Servo_Sweep](#servoanimation_00_1servo_sweep)
- [RTC_ServoAnimation_01_1ServoSimple](#rtc_servoanimation_01_1servosimple)
- [RTC_ServoAnimation_02_2ServosSimple](#rtc_servoanimation_02_2servossimple)
- [RTC_ServoAnimation_03_1Servo_TimeCues](#rtc_servoanimation_03_1servo_timecues)
- [RTC_ServoAnimation_04_2Servos_1Switch1Constant](#rtc_servoanimation_04_2servos_1switch1constant)
- [RTC_ServoAnimation_05_Simple_TimeCues](#rtc_servoanimation_05_simple_timecues)
- [RTC_ServoAnimation_06_SpeedControl](#rtc_servoanimation_06_speedcontrol)

## ServoAnimation_00_1Servo_Sweep

**Description**: A simple introductory example showing how to create a basic servo sweep animation using the ServoNotifier system without RTC integration.

**Key Features**:
- Single servo moves back and forth across its full range (0-180 degrees)
- Uses only two keyframes to define the motion
- BOOMERANG playback mode for automatic back-and-forth motion
- Demonstrates the most basic animation setup possible

**Key Functions**:
- `setup()`: Initializes the servo and creates a simple sweep animation
- `loop()`: Updates the animation and applies it to the servo

**Key Variables**:
- `myServo`: Servo object to be controlled
- `notifier`: ServoNotifier that handles the animation

## RTC_ServoAnimation_01_1ServoSimple

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

## RTC_ServoAnimation_03_1Servo_TimeCues

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

## RTC_ServoAnimation_02_2ServosSimple

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

## RTC_ServoAnimation_04_2Servos_1Switch1Constant

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

## RTC_ServoAnimation_05_Simple_TimeCues

**Description**: A simplified version of TimeCues example without LED matrix, using direct variable references for easier understanding and modification.

**Key Features**:
- Five different animations with different playback modes (ONCE, LOOP, BOOMERANG)
- Each animation is triggered at a specific "cue" minute (0-4)
- Uses individual variables for cue times and animation names for clarity
- Demonstrates a gradual progression of animation complexity and behaviors

**Key Functions**:
- `setup()`: Initializes the RTC, servo, and creates five animations
- `loop()`: Checks current time and plays animations at specific minutes
- `setupAnimations()`: Creates the 5 different animations for the servo

**Key Variables**:
- `cue0`, `cue1`, etc.: Individual minute values when animations should trigger
- `animName0`, `animName1`, etc.: String variables for animation names
- `myServo`: Servo object to be controlled
- `notifier`: ServoNotifier that handles the animations
- `animationSpeed`: Controls the playback speed of all animations
- `currentAnimation`: Tracks which animation is currently playing

## RTC_ServoAnimation_06_SpeedControl

**Description**: Demonstrates how to modify animation speed in response to time-based triggers, using a single animation with varying playback rates.

**Key Features**:
- A single complex animation plays continuously throughout
- Five different playback speeds change at specific minute intervals
- Shows how to create varied motion without multiple animations
- Uses the global speed control feature of the ServoNotifier

**Key Functions**:
- `setup()`: Initializes the RTC, servo, and creates a single complex animation
- `loop()`: Checks current time and adjusts animation speed at specific minutes
- `setupAnimation()`: Creates a single animation with multiple keyframes

**Key Variables**:
- `cue0`, `cue1`, etc.: Individual minute values when speed changes should occur
- `speedSettings[]`: Array of speed multipliers for each cue point
- `currentSpeed`: Tracks the current playback speed
- `myServo`: Servo object to be controlled
- `notifier`: ServoNotifier that handles the animation
- `animName`: String variable for the animation name