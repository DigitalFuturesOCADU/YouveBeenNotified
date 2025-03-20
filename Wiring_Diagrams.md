# Wiring Diagrams for Servo Projects

This guide provides wiring instructions for both single servo and dual servo configurations used with the Arduino R4 WiFi and the You've Been Notified library.

## Single Servo Configuration

![Single Servo Wiring Diagram](/images/1Servo.png)

*Single servo wiring diagram showing direct connection to Arduino R4 WiFi*

### Connection Details

| Servo Wire | Arduino Pin |
|------------|-------------|
| Yellow/Orange (Signal) | Pin 9 |
| Red (Power) | 5V |
| Brown/Black (Ground) | GND |

This configuration is used for all single-servo examples in the library. The servo connects directly to the Arduino without requiring additional components.

### Key Points

- The servo signal wire connects to PWM-capable pin 9
- Power is supplied directly from the Arduino's 5V pin
- This configuration can power a single micro servo (SG90) safely

## Dual Servo Configuration

![Dual Servo Wiring Diagram with Breadboard](/images/2Servo_breadboard.png)

*Dual servo wiring diagram using a breadboard for power distribution*

### Connection Details

| Component | Connection |
|-----------|------------|
| Servo 1 Signal | Pin 9 |
| Servo 2 Signal | Pin 10 |
| Both Servo Power Lines | 5V via breadboard |
| Both Servo Ground Lines | GND via breadboard |

### Why Use a Breadboard?

A breadboard is required for dual-servo setups because:

1. The Arduino R4 WiFi has a limited number of 5V pins (only one)
2. The breadboard allows both servos to connect to the same power source
3. It provides a clean power distribution system without soldering



