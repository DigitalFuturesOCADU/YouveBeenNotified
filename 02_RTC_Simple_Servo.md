# RTC Simple Servo Examples

These examples demonstrate how to combine the Real Time Clock (RTC) with basic servo motor control. They use standard Arduino Servo library functions (rather than the animation system) to create time-based servo movements.

## Table of Contents
- [RTCservo_01_1Servo_AngleList](#rtcservo_01_1servo_anglelist)
- [RTCservo_02_2Servos_AngleList](#rtcservo_02_2servos_anglelist)
- [RTCservo_03_1Servo_RandomAngles](#rtcservo_03_1servo_randomangles)
- [RTCservo_04_2Servos_RandomAngles](#rtcservo_04_2servos_randomangles)

## RTCservo_01_1Servo_AngleList

**Description**: Controls a single servo motor using predetermined positions from an array, changing position each minute based on the minute value.

**Key Features**:
- Uses an array of 10 predefined servo positions (0-180 degrees)
- Position changes every minute based on current minute % 10
- Display toggles between showing minute (with 'm' suffix) and servo position (with 'd' suffix)
- Simple minute-based timing system

// ...existing code...

## RTCservo_02_2Servos_AngleList

**Description**: Controls two servo motors using predetermined position arrays, with a cycling display that shows the minute and both servo positions.

// ...existing code...

## RTCservo_03_1Servo_RandomAngles

**Description**: Controls a single servo motor using random positions that change each minute.

// ...existing code...

## RTCservo_04_2Servos_RandomAngles

**Description**: Controls two servo motors with random positions that change each minute, with a cycling display.

// ...existing code...
