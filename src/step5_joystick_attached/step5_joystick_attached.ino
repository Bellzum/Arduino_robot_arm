// Original code by Ryan Chan. Updated code by MilesPeterson101
// Modified: Joystick (A0/A1) + SW on D11 + fixed button handling + proper save/play behavior

#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

const int LED1 = 2;
const int LED2 = 3;
const int LED3 = 4;
const int LED4 = 7;
const int LED5 = 8;

const int button1 = 12; // Save position (right button)
const int button2 = 13; // Start playback (left button)

// Joystick
const int joyX = A0;   // VRx
const int joyY = A1;   // VRy
const int joySW = 11;  // SW (press = LOW when using INPUT_PULLUP)

// Keep pot3 for servo3
const int pot3 = A2;

// Saved positions (5 max)
int servo1PosSaves[] = {1, 1, 1, 1, 1};
int servo2PosSaves[] = {1, 1, 1, 1, 1};
int servo3PosSaves[] = {1, 1, 1, 1, 1};

// Angles we actually command
int s1 = 90;
int s2 = 90;
int s3 = 90;

// Save/play state
int button1Presses = 0;     // 0..5
bool playbackEnabled = false;
bool joystickEnabled = true; // toggled by SW

// Tuning for joystick direction control
const int DEADZONE = 80;  // ignore noise around center
const int STEP = 2;       // degrees per loop when joystick pushed
const int LOOP_DELAY = 20;

// Simple debounce + edge detect
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

  // If your buttons are wired with external pulldown resistors, keep INPUT.
  // If your buttons are wired to GND when pressed (recommended), switch to INPUT_PULLUP and invert logic.
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  // Joystick switch: connect SW to D11 and other side to GND
  pinMode(joySW, INPUT_PULLUP);

  Serial.begin(9600);

  // Start centered
  servo1.write(s1);
  servo2.write(s2);

  // Servo3 from pot
  int pot3Val = analogRead(pot3);
  s3 = map(pot3Val, 0, 1023, 0, 179);
  servo3.write(s3);

  setLEDs(0);
  Serial.println("Ready: Save with D12, Play with D13, Toggle joystick with SW(D11).");
}

void loop() {
  // --- Read servo3 pot always ---
  int pot3Val = analogRead(pot3);
  s3 = map(pot3Val, 0, 1023, 0, 179);
  servo3.write(s3);

  // --- Joystick SW toggles joystick control ON/OFF ---
  int swNow = digitalRead(joySW); // HIGH idle, LOW pressed
  bool swPressed = (swNow == LOW && lastSW == HIGH);
  if (swPressed && (millis() - lastDebounceTime > DEBOUNCE_MS)) {
    joystickEnabled = !joystickEnabled;
    lastDebounceTime = millis();

    Serial.print("Joystick control: ");
    Serial.println(joystickEnabled ? "ON" : "OFF");
  }
  lastSW = swNow;

  // --- If joystick control enabled, update servo1 & servo2 by direction ---
  if (joystickEnabled && !playbackEnabled) {
    int x = analogRead(joyX);
    int y = analogRead(joyY);

    int dx = x - 512;
    int dy = y - 512;

    if (abs(dx) < DEADZONE) dx = 0;
    if (abs(dy) < DEADZONE) dy = 0;

    // X -> servo1
    if (dx > 0) s1 += STEP;
    else if (dx < 0) s1 -= STEP;

    // Y -> servo2 (invert if it feels backward)
    if (dy > 0) s2 += STEP;
    else if (dy < 0) s2 -= STEP;

    s1 = constrain(s1, 0, 179);
    s2 = constrain(s2, 0, 179);

    servo1.write(s1);
    servo2.write(s2);
  }

  // --- Read buttons with edge detection (prevents multiple triggers while held) ---
  int b1Now = digitalRead(button1);
  int b2Now = digitalRead(button2);

  bool b1Pressed = (b1Now == HIGH && lastB1 == LOW);
  bool b2Pressed = (b2Now == HIGH && lastB2 == LOW);

  lastB1 = b1Now;
  lastB2 = b2Now;

  // --- Save position (button1) ---
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

  // --- Start playback (button2) ---
  if (b2Pressed && (millis() - lastDebounceTime > DEBOUNCE_MS)) {
    lastDebounceTime = millis();

    if (button1Presses < 5) {
      Serial.println("Save all 5 positions first (LED1-LED5 on).");
    } else {
      playbackEnabled = true;
      Serial.println("Playback started (loop forever).");
    }
  }

  // --- Playback loop: move positions 1..5 repeatedly forever ---
  if (playbackEnabled) {
    for (int i = 0; i < 5; i++) {
      servo1.write(servo1PosSaves[i]);
      servo2.write(servo2PosSaves[i]);
      servo3.write(servo3PosSaves[i]);

      Serial.println("Saved Angles:");
      Serial.println(servo1PosSaves[i]);
      Serial.println(servo2PosSaves[i]);
      Serial.println(servo3PosSaves[i]);

      delay(1050);
    }
    // repeats forever because playbackEnabled stays true and loop() repeats
  }

  delay(LOOP_DELAY);
}
