/*
step3_button_test.ino

Purpose
- Verify that your two buttons are wired correctly using INPUT_PULLUP.
- This is the safest button wiring method for beginners (no external resistor needed).

Hardware
- Arduino Uno
- 2x push buttons

Wiring (IMPORTANT)
For INPUT_PULLUP, each button must connect:
- One side -> Arduino pin (D12 or D13)
- Other side -> GND

No 5V connection is needed for the buttons in this mode.

Pins used
- SAVE button -> D12
- PLAY button -> D13

How to Run
- Upload this sketch.
- Open Serial Monitor at 9600 baud.
- Click/press buttons in Tinkercad.

Expected Behavior
Because INPUT_PULLUP is used:
- Not pressed -> reads 1 (HIGH)
- Pressed     -> reads 0 (LOW)

Notes
- If you always see 0:
  - the pin is stuck to GND (button wired wrong or not crossing breadboard gap)
- If you always see 1:
  - button not actually connected to GND when pressed
*/

const int BUTTON_SAVE = 12;
const int BUTTON_PLAY = 13;

void setup() {
  pinMode(BUTTON_SAVE, INPUT_PULLUP);
  pinMode(BUTTON_PLAY, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Button test ready (INPUT_PULLUP).");
  Serial.println("Expected: not pressed=1, pressed=0");
}

void loop() {
  int saveState = digitalRead(BUTTON_SAVE);
  int playState = digitalRead(BUTTON_PLAY);

  Serial.print("SAVE=");
  Serial.print(saveState);
  Serial.print(" | PLAY=");
  Serial.println(playState);

  delay(200);
}
