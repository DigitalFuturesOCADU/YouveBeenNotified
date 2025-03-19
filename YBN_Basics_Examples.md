# Animation System Basics Examples

These examples focus on demonstrating the core animation capabilities of the ServoNotifier system without RTC integration. They provide a foundation for understanding how to create, customize, and control keyframe animations for servo motors.

## Table of Contents
- [YBN_01_1Servo_1Animation](#ybn_01_1servo_1animation)
- [YBN_02_1Servo_AnimSequence](#ybn_02_1servo_animsequence)

## YBN_01_1Servo_1Animation

**Description**: Demonstrates how to use the ServoNotifier class to animate a servo motor through a series of keyframes, with display on the LED matrix.

**Key Features**:
- Creates a 4-keyframe animation for smooth servo movement
- Displays the current servo angle on the LED matrix
- Prints detailed animation information to Serial Monitor
- Uses LOOP mode for continuous animation
- Provides extensive comments and extension ideas for customizing animations

**Key Functions**:
- `setup()`: Creates the servo animation with 4 keyframes
- `loop()`: Updates the animation and prints status when value changes
- `printAnimationInfo()`: Displays current animation information
- `displayServoAngle()`: Displays the current servo angle on the LED matrix

**Key Variables**:
- `myServo`: Servo object to be controlled
- `notifier`: ServoNotifier that handles the animation
- `servoAnimation`: Defines the keyframes and timing for the animation
- `animationName`: String variable storing the name of the animation
- `animationSpeed`: Controls animation playback rate (1.0 = normal)

## YBN_02_1Servo_AnimSequence

**Description**: Shows how to play multiple animations in sequence, with each animation playing once and then automatically starting the next animation.

**Key Features**:
- Creates three distinct animations with different motion patterns
- Each animation plays in ONCE mode and triggers the next when completed
- Displays the current animation name on the LED matrix
- Shows "-fin-" on the matrix when all animations are complete
- Demonstrates how to detect animation completion and chain animations

**Key Functions**:
- `setup()`: Creates three different servo animations and prepares for sequential playback
- `loop()`: Updates the animation and checks for completion to start the next animation
- `printAnimationInfo()`: Displays current animation information
- `displayText()`: Displays text on the LED matrix (animation name or "-fin-")

**Key Variables**:
- `myServo`: Servo object to be controlled
- `notifier`: ServoNotifier that handles the animations
- `anim1`, `anim2`, `anim3`: Three different KeyframeAnimations
- `currentAnimIndex`: Tracks which animation is currently playing
- `animationsComplete`: Flag to indicate when all animations are finished