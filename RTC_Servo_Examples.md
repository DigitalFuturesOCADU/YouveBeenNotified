# RTC Simple Servo Examples

These examples demonstrate how to combine the Real Time Clock (RTC) with basic servo motor control. They use standard Arduino Servo library functions (rather than the animation system) to create time-based servo movements.

## Table of Contents
- [RTCservo_01_1ServoAngleList](#rtcservo_01_1servoanglelist)
- [RTCservo_02_2ServoAngleList](#rtcservo_02_2servoanglelist)
- [RTCservo_03_1ServoRandom](#rtcservo_03_1servorandom)
- [RTCservo_04_2ServoRandom](#rtcservo_04_2servorandom)

## RTCservo_01_1ServoAngleList

**Description**: Controls a single servo motor using predetermined positions from an array, changing position each minute based on the minute value.

**Key Features**:
- Uses an array of 10 predefined servo positions (0-180 degrees)
- Position changes every minute based on current minute % 10
- Display toggles between showing minute (with 'm' suffix) and servo position (with 'd' suffix)
- Simple minute-based timing system

**Key Functions**:
- `minuteUpdate()`: Updates the servo position based on the current minute
- `showTimeDebug()`: Displays current second count and servo value
- `displayMinuteAndServo()`: Toggles between showing minute or servo angle on the LED matrix

**Key Variables**:
- `servoPositions[]`: Array of 10 predefined servo positions (0-180 degrees)
- `currentServoPosition`: Stores the current servo position value
- `showMinute`: Boolean to toggle between showing minute vs servo angle
- `lastMinute`: Stores the last minute value to detect changes

## RTCservo_02_2ServoAngleList

**Description**: Controls two servo motors using predetermined position arrays, with a cycling display that shows the minute and both servo positions.

**Key Features**:
- Controls two servos simultaneously with different position arrays
- 5-state display cycle shows: minute, "s1:", servo1 value, "s2:", servo2 value
- Position changes every minute based on current minute % 10
- Demonstrates multi-servo control with time-based triggers

**Key Functions**:
- `minuteUpdate()`: Updates both servo positions based on the current minute
- `showTimeDebug()`: Displays current second count and servo values
- `displayValues()`: Cycles through a 5-frame display sequence on the LED matrix

**Key Variables**:
- `servoPositions1[]`/`servoPositions2[]`: Two arrays of 10 predefined servo positions
- `currentServoPosition1`/`currentServoPosition2`: Store the current servo position values
- `displayState`: Controls which value is shown on the LED matrix (0=minute, 1="s1:", 2=servo1, 3="s2:", 4=servo2)
- `lastMinute`: Stores the last minute value to detect changes

## RTCservo_03_1ServoRandom

**Description**: Controls a single servo motor using random positions that change each minute.

**Key Features**:
- Generates random servo positions within a specified range (0-180 degrees)
- New random position generated every minute
- Display toggles between showing minute (with 'm' suffix) and servo position (with 'd' suffix)
- Demonstrates randomized time-based movement

**Key Functions**:
- `minuteUpdate()`: Updates the servo position to a random value
- `showTimeDebug()`: Displays current second count and servo value
- `displayMinuteAndServo()`: Toggles between showing minute or servo angle on the LED matrix

**Key Variables**:
- `minServoAngle`/`maxServoAngle`: Range limits for the random servo position
- `currentServoPosition`: Stores the current servo position value
- `showMinute`: Boolean to toggle between showing minute vs servo angle
- `lastMinute`: Stores the last minute value to detect changes

## RTCservo_04_2ServoRandom

**Description**: Controls two servo motors with random positions that change each minute, with a cycling display.

**Key Features**:
- Controls two servos with independent random positions
- 5-state display cycle shows: minute, "s1:", servo1 value, "s2:", servo2 value
- New random positions generated every minute
- Combines multi-servo control with randomized time-based triggers

**Key Functions**:
- `minuteUpdate()`: Updates both servo positions to random values
- `showTimeDebug()`: Displays current second count and servo values
- `displayValues()`: Cycles through a 5-frame display sequence on the LED matrix

**Key Variables**:
- `minServoAngle`/`maxServoAngle`: Range limits for random servo positions
- `currentServoPosition1`/`currentServoPosition2`: Store the current servo position values
- `displayState`: Controls which value is shown on the LED matrix
- `lastMinute`: Stores the last minute value to detect changes