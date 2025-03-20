# RTC Basics Examples

These examples demonstrate the fundamental use of the Real Time Clock (RTC) on the Arduino R4 WiFi board, combined with the LED matrix display for visualization. They provide a foundation for understanding time-based trigger systems without the complexity of servo control or animations.

## Table of Contents
- [RTC_SimpleClock](#rtc_simpleclock)
- [RTC_PatternCycle](#rtc_patterncycle)
- [RTC_LEDFill](#rtc_ledfill)
- [RTC_EventTrigger](#rtc_eventtrigger)

## RTC_SimpleClock

**Description**: Demonstrates basic RTC polling to track and display minutes on the LED matrix.

**Key Features**:
- Counts up to 60 or down from 60 (configurable direction)
- Displays the current minute count on the LED matrix
- Uses polling technique to check time at regular intervals
- Includes debug output through Serial Monitor

**Key Functions**:
- `checkTime()`: Polls the RTC and updates when the minute changes
- `showTimeDebug()`: Displays current time values on Serial Monitor
- `displayMinute()`: Draws the current minute value on the LED matrix

**Key Variables**:
- `countDirection`: Determines counting direction (0 = up to 60, 1 = down from 60)
- `lastMinute`: Stores the last minute value to detect changes
- `currentMinute`: Stores the current minute from RTC

## RTC_PatternCycle

**Description**: Shows how to change displayed patterns on the LED matrix based on minute changes.

**Key Features**:
- Cycles through 5 different shapes: rectangle outline, filled rectangle, circle outline, filled circle, crossed lines
- Changes to next shape at the start of each minute
- Includes debug output showing current time and shape index
- Demonstrates simple shape drawing on the matrix display

**Key Functions**:
- `checkTime()`: Polls the RTC and updates when the minute changes
- `showTimeDebug()`: Displays current time values on Serial Monitor
- `displayShape()`: Draws the current shape on the LED matrix

**Key Variables**:
- `shapeIndex`: Tracks which shape is currently displayed
- `lastMinute`: Stores the last minute value to detect changes
- `currentMinute`: Stores the current minute from RTC

## RTC_LEDFill

**Description**: Visualizes time passage by sequentially illuminating LEDs across the matrix.

**Key Features**:
- Each minute lights up one additional LED in sequence
- Fills matrix from left-to-right, top-to-bottom
- Resets when matrix is completely filled
- Shows how to map linear time to 2D display coordinates

**Key Functions**:
- `checkTime()`: Polls the RTC and updates when the minute changes
- `showTimeDebug()`: Displays current time values on Serial Monitor
- `updateLED()`: Lights up the current LED position on the matrix

**Key Variables**:
- `ledX`, `ledY`: Track the current LED position on the matrix
- `lastMinute`: Stores the last minute value to detect changes
- `currentMinute`: Stores the current minute from RTC

## RTC_EventTrigger

**Description**: Demonstrates triggering special events at specific times.

**Key Features**:
- Displays minute counting similar to RTC_SimpleClock
- Adds a special "cue" function that activates at a configured minute
- When cue triggers, the entire matrix illuminates as a visual alert
- Shows how to implement time-based event triggers

**Key Functions**:
- `checkTime()`: Polls the RTC and updates when the minute changes
- `showTimeDebug()`: Displays current time values on Serial Monitor
- `displayMinute()`: Draws the current minute value on the LED matrix
- `triggerCue()`: Activates when the target minute is reached, lighting up all LEDs

**Key Variables**:
- `countDirection`: Determines counting direction (0 = up to 60, 1 = down from 60)
- `lastMinute`: Stores the last minute value to detect changes
- `currentMinute`: Stores the current minute from RTC
- `targetMinute`: The specific minute when the event should trigger (default: 30)