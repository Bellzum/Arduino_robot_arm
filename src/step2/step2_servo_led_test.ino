/*
step2_servo_led_test.ino

Purpose
- Verify servos still move correctly while LEDs blink.
- Confirms digital outputs work in your circuit without buttons/pots.

Hardware
- Arduino Uno
- 3x Servos
- 3x LEDs (or more), each with a resistor (220Ω–330Ω)

Wiring
Servos:
- Servo1 signal -> D5
- Servo2 signal -> D6
- Servo3 signal -> D9
- Servo VCC -> 5V, Servo GND -> GND (common ground)

LEDs (each LED must be in series with a resistor):
- D2 -> resistor -> LED (long leg) ; LED (short leg) -> GND
- D7 -> resistor -> LED (long leg) ; LED (short leg) -> GND
- D8 -> resistor -> LED (long leg) ; LED (short leg) -> GND

How to Run
- Upload this sketch.
- Watch servos move and LEDs switch patterns every ~1.5s.

Expected Behavior
- Position A:
  - Servo angles: (0, 90, 180)
  - LED1 (D2) ON
  - LED4 (D7) OFF
  - LED5 (D8) OFF
- Position B:
  - Servo angles: (180, 0, 90)
  - LED1 (D2) OFF
  - LED4 (D7) ON
  - LED5 (D8) ON

Notes
- If LED1 works but LED4/LED5 do not:
  - check that those LEDs are truly wired to D7 and D8
  - check LED polarity and resistor placement
*/

#include <Servo.h>

// Servo objects
Servo servo1;
Servo servo2;
Servo servo3;

// LED pins (chosen to avoid conflicts with buttons used later)
const int LED1 = 2;
const int LED4 = 7;
const int LED5 = 8;

void setup() {
  // Attach servos
  servo1.attach(5);
  servo2.attach(6);
  servo3.attach(9);

  // Configure LEDs as outputs
  pinMode(LED1, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
}

void loop() {
  // Position A: move servos + LED pattern A
  servo1.write(0);
  servo2.write(90);
  servo3.write(180);

  digitalWrite(LED1, HIGH);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);

  delay(1500);

  // Position B: move servos + LED pattern B
  servo1.write(180);
  servo2.write(0);
  servo3.write(90);

  digitalWrite(LED1, LOW);
  digitalWrite(LED4, HIGH);
  digitalWrite(LED5, HIGH);

  delay(1500);
}
