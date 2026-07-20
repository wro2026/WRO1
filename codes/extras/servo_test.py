"""
servo_test.py
-------------
Tests the SG90 steering servo and helps find the centre and the
left/right steering limits.

Wiring:
  Servo signal (orange) -> Pi GPIO 18 (physical pin 12)
  Servo power  (red)    -> 5 V supply (not the Pi's 5V pin)
  Servo ground (brown)  -> common ground (servo GND + L298N GND + Pi GND)

Notes:
  - All grounds must be connected together, or the servo will not move.
  - The SG90 uses ~1000-2000 us pulse widths (0.001 - 0.002 s below).

How to use:
  Type an angle and press Enter. Type 'q' to quit.
  1. Find the centre: the angle where the wheels point straight.
  2. Find the limits: increase slowly; stop as soon as the servo strains
     or buzzes, because the SG90 has plastic gears that can strip.

Status: tested and working.
"""

from gpiozero import AngularServo
from gpiozero.pins.pigpio import PiGPIOFactory

factory = PiGPIOFactory()

servo = AngularServo(18, min_angle=-90, max_angle=90,
                     min_pulse_width=0.001,   # SG90: 1000 us
                     max_pulse_width=0.002,   # SG90: 2000 us
                     pin_factory=factory)

print("Type an angle (-90 to 90). 'q' to quit.")

while True:
    x = input("angle: ")
    if x == 'q':
        break
    try:
        servo.angle = float(x)
    except ValueError:
        print("Please type a number.")

servo.detach()
