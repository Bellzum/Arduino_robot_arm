/*
step4_full_memory_controller.ino

Purpose
- Full beginner-friendly controller:
  1) Live control of 3 servos using 3 potentiometers
  2) Save up to 5 positions using a SAVE button
  3) Light an LED for each saved slot (1..5)
  4) Replay saved positions using a PLAY button

Hardware
- Arduino Uno
- 3x Servo motors
- 3x Potentiometers
- 2x Push buttons
- 5x LEDs + resistors (220Ω–330Ω)

Wiring
Servos:
- Servo1 signal -> D5
- Servo2 signal -> D6
- Servo3 signal -> D9
- Servo VCC -> 5V
- Servo GND -> GND (common ground)

Potentiometers (for each pot):
- One outer leg -> 5V
- Middle leg (wiper) -> A0 / A1 / A2
- Other outer leg -> GND
Important: If analogRead stays at 0, the pot is not powered or wiper is not on A0/A1/A2.

Buttons (INPUT_PULLUP):
- SAVE button: D12 <-> button <-> GND
- PLAY button: D13 <-> button <-> GND
Not pressed=1, pressed=0

LEDs (each with resistor in series):
- LED1 -> D2
- LED2 -> D3
- LED3 -> D4
- LED4 -> D7
- LED5 -> D8
Each LED: pin -> resistor -> LED long leg, LED short leg -> GND

How to Run
1) Upload this sketch.
2) Turn pots to move servos.
3) Press SAVE up to 5 times to store positions (LEDs show how many saved).
4) Press PLAY to replay saved positions in order.

Expected Behavior
- Turning pots changes servo angles.
- Each SAVE press stores current angles and turns on the next LED.
- PLAY moves servos through saved angles (only the positions you saved).

Notes
- A simple debounce delay is used (300ms).
- This code prevents saving more than 5 positions.
*/

#include <Servo.h>

// -------------------- Servo objects --------------------
Servo servo1;
Servo servo2;
Servo servo3;

// -------------------- Pin mapping --------------------
const int SERVO1_PIN = 5;
const int SERVO2_PIN = 6;
const int SERVO3_PIN = 9;

const int POT1_PIN = A0;
const int POT2_PIN = A1;
const int POT3_PIN = A2;

const int BUTTON_SAVE = 12;
const int BUTTON_PLAY = 13;

// 5 LEDs indicate 5 saved slots
const int LED_PINS[5] = {2, 3, 4, 7, 8};

// -------------------- State variables --------------------
int saveIndex = 0;            // how many positions have been saved (0..5)
bool playRequested = false;   // set true when PLAY is pressed

// Arrays store saved angles for each servo
int servo1Saved[5];
int servo2Saved[5];
int servo3Saved[5];

void setup() {
  // Attach servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // LEDs
  for (int i = 0; i < 5; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW); // start off
  }

  // Buttons (INPUT_PULLUP: pressed=LOW)
  pinMode(BUTTON_SAVE, INPUT_PULLUP);
  pinMode(BUTTON_PLAY, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Step4: Full controller ready.");
}

void loop() {
  // ---------- Live control: read pots and move servos ----------
  int angle1 = map(analogRead(POT1_PIN), 0, 1023, 0, 180);
  int angle2 = map(analogRead(POT2_PIN), 0, 1023, 0, 180);
  int angle3 = map(analogRead(POT3_PIN), 0, 1023, 0, 180);

  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);

  // ---------- SAVE ----------
  // pressed = LOW because of INPUT_PULLUP
  if (digitalRead(BUTTON_SAVE) == LOW) {
    if (saveIndex < 5) {
      servo1Saved[saveIndex] = angle1;
      servo2Saved[saveIndex] = angle2;
      servo3Saved[saveIndex] = angle3;

      digitalWrite(LED_PINS[saveIndex], HIGH);

      Serial.print("Saved position ");
      Serial.println(saveIndex + 1);

      saveIndex++;
    } else {
      Serial.println("Already saved 5 positions.");
    }

    delay(300); // debounce
  }

  // ---------- PLAY ----------
  if (digitalRead(BUTTON_PLAY) == LOW) {
    playRequested = true;
    delay(300); // debounce
  }

  // ---------- Replay saved positions ----------
  if (playRequested) {
    Serial.println("Playing saved positions...");

    for (int i = 0; i < saveIndex; i++) {
      servo1.write(servo1Saved[i]);
      servo2.write(servo2Saved[i]);
      servo3.write(servo3Saved[i]);

      Serial.print("Position ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(servo1Saved[i]);
      Serial.print(", ");
      Serial.print(servo2Saved[i]);
      Serial.print(", ");
      Serial.println(servo3Saved[i]);

      delay(1000);
    }

    Serial.println("Done.");
    playRequested = false;
  }
}
