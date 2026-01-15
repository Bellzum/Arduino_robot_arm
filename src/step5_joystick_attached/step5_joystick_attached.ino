// Original code by Ryan Chan. Updated code by MilesPeterson101
// Revised: Joystick controls servo1 + (servo2 OR servo3). SW toggles which one.
// Save positions with button1 (D12), play cycle with button2 (D13).
// Joystick: VRx->A0, VRy->A1, SW->D11 (INPUT_PULLUP). Optional pot3->A2.

#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

const int LED1 = 2;
const int LED2 = 3;
const int LED3 = 4;
const int LED4 = 7;
const int LED5 = 8;

const int button1 = 12; // Save position
const int button2 = 13; // Start playback

// Joystick
const int joyX = A0;   // VRx
const int joyY = A1;   // VRy
const int joySW = 11;  // SW (pressed = LOW with INPUT_PULLUP)

// Optional pot for servo3 (used only in Mode 0)
const int pot3 = A2;

// Saved positions (5 max)
int servo1PosSaves[] = {1, 1, 1, 1, 1};
int servo2PosSaves[] = {1, 1, 1, 1, 1};
int servo3PosSaves[] = {1, 1, 1, 1, 1};

// Angles
int s1 = 90;
int s2 = 90;
int s3 = 90;

// State
int button1Presses = 0;     // 0..5
bool playbackEnabled = false;

// Mode: which servo the joystick Y axis controls
int joyMode = 0;
// 0: Y -> servo2, servo3 comes from pot3
// 1: Y -> servo3, servo2 stays where it is

// Joystick tuning
const int DEADZONE = 80;
const int STEP = 2;
const int LOOP_DELAY = 20;

// Debounce + edge detect
int lastB1 = LOW;
int lastB2 = LOW;
int lastSW = HIGH; // INPUT_PULLUP idle = HIGH
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_MS = 120;

void setLEDs(int countOn) {
  digitalWrite(LED1, countOn >= 1 ? HIGH : LOW);
  digitalWrite(LED2, countOn >= 2 ? HIGH : LOW);
  digitalWrite(LED3, countOn >= 3 ? HIGH : LOW);
  digitalWrite(LED4, countOn >= 4 ? HIGH : LOW);
  digitalWrite(LED5, countOn >= 5 ? HIGH : LOW);
}

void setup() {
  servo1.attach(5);
  servo2.attach(6);
  servo3.attach(9);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

  // Keep these as INPUT if you are using external pulldown resistors.
  // If your buttons are wired to GND when pressed, change to INPUT_PULLUP and invert logic.
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  pinMode(joySW, INPUT_PULLUP);

  Serial.begin(9600);

  servo1.write(s1);
  servo2.write(s2);
  servo3.write(s3);

  setLEDs(0);
  Serial.println("Ready:");
  Serial.println("- Save: D12");
  Serial.println("- Play: D13");
  Serial.println("- SW(D11) toggles mode: Y->servo2 <-> Y->servo3");
}

void loop() {
  // ---------- SW toggles joyMode ----------
  int swNow = digitalRead(joySW);         // HIGH idle, LOW pressed
  bool swPressed = (swNow == LOW && lastSW == HIGH);

  if (swPressed && (millis() - lastDebounceTime > DEBOUNCE_MS)) {
    joyMode = 1 - joyMode;               // toggle 0 <-> 1
    lastDebounceTime = millis();

    Serial.print("Mode: ");
    Serial.println(joyMode == 0 ? "Y->servo2 (servo3 from pot3)" : "Y->servo3 (servo2 fixed)");
  }
  lastSW = swNow;

  // ---------- If not playing, allow live control ----------
  if (!playbackEnabled) {
    // Optional: in Mode 0, servo3 follows pot3
    if (joyMode == 0) {
      int pot3Val = analogRead(pot3);
      s3 = map(pot3Val, 0, 1023, 0, 179);
      s3 = constrain(s3, 0, 179);
      servo3.write(s3);
    }

    // Read joystick
    int x = analogRead(joyX);
    int y = analogRead(joyY);

    int dx = x - 512;
    int dy = y - 512;

    if (abs(dx) < DEADZONE) dx = 0;
    if (abs(dy) < DEADZONE) dy = 0;

    // X always controls servo1
    if (dx > 0) s1 += STEP;
    else if (dx < 0) s1 -= STEP;

    // Y controls servo2 or servo3 depending on mode
    if (joyMode == 0) {
      if (dy > 0) s2 += STEP;
      else if (dy < 0) s2 -= STEP;
      s2 = constrain(s2, 0, 179);
      servo2.write(s2);
    } else {
      if (dy > 0) s3 += STEP;
      else if (dy < 0) s3 -= STEP;
      s3 = constrain(s3, 0, 179);
      servo3.write(s3);
    }

    s1 = constrain(s1, 0, 179);
    servo1.write(s1);
  }

  // ---------- Buttons edge detect ----------
  int b1Now = digitalRead(button1);
  int b2Now = digitalRead(button2);

  bool b1Pressed = (b1Now == HIGH && lastB1 == LOW);
  bool b2Pressed = (b2Now == HIGH && lastB2 == LOW);

  lastB1 = b1Now;
  lastB2 = b2Now;

  // ---------- Save position ----------
  if (b1Pressed && (millis() - lastDebounceTime > DEBOUNCE_MS)) {
    lastDebounceTime = millis();

    if (button1Presses < 5) {
      servo1PosSaves[button1Presses] = s1;
      servo2PosSaves[button1Presses] = s2;
      servo3PosSaves[button1Presses] = s3;

      button1Presses++;
      setLEDs(button1Presses);

      Serial.print("Pos ");
      Serial.print(button1Presses);
      Serial.println(" Saved");
    } else {
      Serial.println("All 5 positions already saved.");
    }
  }

  // ---------- Start playback ----------
  if (b2Pressed && (millis() - lastDebounceTime > DEBOUNCE_MS)) {
    lastDebounceTime = millis();

    if (button1Presses < 5) {
      Serial.println("Save all 5 positions first (LED1-LED5 on).");
    } else {
      playbackEnabled = true;
      Serial.println("Playback started (loop forever).");
    }
  }

  // ---------- Playback loop (forever) ----------
  if (playbackEnabled) {
    for (int i = 0; i < 5; i++) {
      servo1.write(servo1PosSaves[i]);
      servo2.write(servo2PosSaves[i]);
      servo3.write(servo3PosSaves[i]);
      delay(1050);
    }
  }

  delay(LOOP_DELAY);
}
