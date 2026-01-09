# Arduino_robot_arm
# Servo Memory Controller (Arduino / Tinkercad)

This project is a beginner Arduino project that controls 3 servo motors using:
- potentiometers (manual control)
- buttons (save & play)
- LEDs (visual feedback)

The code is developed step by step and tested in Tinkercad.

## Hardware
- Arduino Uno
- 3x Servo motors
- 3x Potentiometers
- 2x Push buttons
- 5x LEDs + resistors
- Breadboard & jumper wires

## Pin Mapping

### Servos
| Servo | Pin |
|-----|-----|
| Servo 1 | D5 |
| Servo 2 | D6 |
| Servo 3 | D9 |

### Potentiometers
| Pot | Pin |
|---|---|
| Pot 1 | A0 |
| Pot 2 | A1 |
| Pot 3 | A2 |

### Buttons
| Button | Pin |
|---|---|
| Save | D12 |
| Play | D13 |

### LEDs
| LED | Pin |
|---|---|
| LED1 | D2 |
| LED2 | D3 |
| LED3 | D4 |
| LED4 | D7 |
| LED5 | D8 |

## Code Structure
- `step1_servo_test.ino` → servo movement only
- `step2_servo_led_test.ino` → servo + LED
- `step3_button_test.ino` → button input test
- `step4_full_memory_controller.ino` → full system

## Simulation
Tested using **Tinkercad Circuits**.
