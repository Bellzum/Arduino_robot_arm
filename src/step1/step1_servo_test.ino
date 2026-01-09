/*
step1_servo_test.ino

Purpose
- Verify that 3 servos are wired correctly and can move using fixed angles.
- This is the minimum working checkpoint before adding LEDs, buttons, or potentiometers.

Hardware
- Arduino Uno (Tinkercad compatible)
- 3x Servo motors

Wiring
- Servo signal pins:
  - Servo1 signal -> D5
  - Servo2 signal -> D6
  - Servo3 signal -> D9
- Servo power:
  - Servo VCC (red)  -> 5V
  - Servo GND (black/brown) -> GND
- Important:
  - All servos must share GND with Arduino.
  - In real hardware, 3 servos often require an external 5V supply.

How to Run
- Upload this sketch.
- Watch the servos move between two sets of angles.

Expected Behavior
- Every ~1.5 seconds, servos move between:
  - Position A: (0, 90, 180)
  - Position B: (180, 0, 90)

Notes
- If nothing moves:
  - Check servo power wiring (5V/GND)
  - Check signal pins (D5/D6/D9)
  - In real hardware, USB power may be insufficient for 3 servos.
*/

#include <Servo.h>

// Servo objects
Servo servo1;
Servo servo2;
Servo servo3;

void setup() {
  // Attach servo signal wires to Arduino pins
  servo1.attach(5);
  servo2.attach(6);
  servo3.attach(9);
}

void loop() {
  // Position A
  servo1.write(0);
  servo2.write(90);
  servo3.write(180);
  delay(1500);

  // Position B
  servo1.write(180);
  servo2.write(0);
  servo3.write(90);
  delay(1500);
}
