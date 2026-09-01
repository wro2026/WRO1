Mechanical Design


1. Chassis Design

Our robot chassis is constructed entirely from LEGO components. The approximate dimensions of the robot are 23 cm × 11 cm × 27 cm.
The chassis uses a hollow structure to reduce the overall weight of the robot. We chose LEGO because it is lightweight and easily modifiable. This allows us to make mechanical changes quickly during development and testing.
The robot has four wheels, with each wheel having an approximate diameter of 2 inches (5.08 cm).
The SPIKE Prime hub is positioned approximately in the middle of the robot.

2. Drive System

The robot uses the LEGO SPIKE Prime system for its movement and steering.
One SPIKE Prime small motor is used for driving the robot. The motor is connected directly to the front two wheels without using gears.
The front two wheels are mechanically connected through an axle, allowing the drive motor to transfer its movement to the wheels.
The robot can drive forwards and backwards.
We selected this arrangement because it is simple to build and easy to modify using LEGO components.

3. Steering System

One SPIKE Prime large motor is used for steering.
The two front wheels are connected by a LEGO bar. The large motor rotates this bar, which causes the two front wheels to turn together.
The steering system is mechanically separate from the drive motor. This allows the robot to control its driving and steering using two separate motors.
During testing, we found that the robot turns fairly well. Steering performance was considered satisfactory for the current design.

4. Torque and Speed

The drive wheels have an approximate diameter of 2 inches (5.08 cm).
The drive motor is connected directly to the wheels, with no intermediate gearing. This keeps the drivetrain simple and reduces the number of mechanical components between the motor and wheels.
We have not yet measured the robot's exact maximum speed or calculated the available wheel torque. These values will be measured during further testing so that the mechanical performance can be evaluated using actual data rather than estimates.

5. Mechanical Stability and Rigidity

The robot uses a four-wheel configuration and a LEGO chassis.
The hollow chassis was designed to reduce weight while maintaining the LEGO structure needed to support the motors, wheels and electronics.
The robot has been found to be stable during normal operation. We have not observed major problems with the chassis coming apart or bending during driving.

6. Design Choices and Trade-offs

A major design decision was the choice to use LEGO and SPIKE Prime instead of a Raspberry Pi 4-based system.
Our first version was made using a Raspberry Pi 4. We found this approach very difficult and time-consuming because we had to develop a learning-based system and also experienced difficulties with pairing and setup.
We therefore changed to LEGO SPIKE Prime.
The SPIKE Prime approach was much easier for our team to use, and we were able to make progress more quickly. It also allowed us to modify the mechanical design easily because the robot is constructed from LEGO components.
The main trade-off was between the flexibility and complexity of the Raspberry Pi approach and the simplicity and ease of modification provided by SPIKE Prime. We selected SPIKE Prime because it allowed us to develop and iterate the robot more efficiently.
7. Design Iterations and Testing

We have developed two major versions of the robot.

Version 1 — Raspberry Pi 4

Our first version used a Raspberry Pi 4.
The main problem with this version was its complexity. We found it very difficult to develop because the system required significant time to develop the learning approach. Pairing and setup also caused difficulties.
As a result, progress was slow.

Version 2 — LEGO SPIKE Prime

We changed the robot to a LEGO SPIKE Prime-based design.
This made development considerably easier and allowed us to make progress much more quickly. The LEGO construction also made the robot easier to modify during development.
The current version uses:
•	LEGO chassis
•	Four wheels
•	2-inch diameter wheels
•	One SPIKE Prime small motor for driving
•	One SPIKE Prime large motor for steering
•	Direct drive from the small motor to the front wheels
•	A LEGO bar connecting the front wheels for steering
The robot currently turns fairly well and is stable during operation.
Further testing will be used to measure speed, steering performance and other mechanical characteristics quantitatively.
