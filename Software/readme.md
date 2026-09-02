3. Software Architecture & Obstacle Strategy

3.1 Programming Architecture

The robot is programmed in Python. The software coordinates motor control, sensor input, steering and autonomous navigation.
The software can be understood as five functional stages: sensing, validation, decision, motion control and route completion.

<img width="1104" height="310" alt="image" src="https://github.com/user-attachments/assets/69b159bf-8ed7-4e35-aca6-4abfa151f2ac" />


3.2 Main Autonomous State Machine

The robot follows the following state sequence:

1.	START: initialize the controller, motors, sensors and required variables.

2.	DRIVE: move forward at the normal driving speed.

3.	DETECT: monitor the distance sensors for a navigation condition.

4.	VALIDATE: confirm the detection and check the relevant color information.

5.	DECIDE: determine whether the required direction is left or right.

6.	SLOW: reduce drive speed before the turn.

7.	TURN: move the steering motor to the selected steering angle.

8.	COUNT: increment the completed-turn counter.

9.	CONTINUE: return to forward driving.

10.	STOP: after 12 turns, drive approximately 30 cm and stop.

The state-machine structure was selected because it separates sensing, decision-making and movement. This makes the behavior easier to test and tune than a single long sequence of motor commands.

3.3 Turn Decision Algorithm

The decision process is:
Detect condition → Check color → Decide left/right → Steer → Continue
The robot does not use one fixed direction for every turn. Instead, the relevant sensor condition and color information are considered before the steering direction is selected.
Conceptually, the control logic is:
IF a turn condition is detected: validate the reading; check color; select LEFT or RIGHT; reduce speed; apply the steering angle; complete the turn; count the turn; return to driving.

3.4 Obstacle Strategy and Edge Cases

Distance sensors provide information about objects or walls. If an object is detected, the robot checks the color information before selecting a path. The selected direction is then passed to the steering system.
•	If the sensor reading is unexpected or unstable, repeat the detection/decision process rather than immediately making a turn.
•	If a valid condition is detected but the color information does not provide the expected classification, the software should repeat the sensing step before committing to a direction.
•	During turns, the robot reduces drive speed to reduce overshoot.
•	After each successful turn, the turn counter is updated so the route can be completed without a manual stop.

3.5 Steering Control

The large SPIKE Prime motor controls the front-wheel steering mechanism. A predetermined steering angle is used for turns. This angle was adjusted during testing to obtain reliable turning behavior.
The robot normally drives at a fixed speed and uses a lower speed during turns. This creates a simple two-speed strategy: efficient forward travel and controlled turning.

3.6 Turn Counting and Stopping

The current route-control sequence is designed to complete 12 turns. Once the counter reaches 12, the robot continues for approximately 30 cm and then stops.
This stopping strategy reduces dependence on a manual operator and gives the autonomous run a defined end condition.

3.7 Software Testing and Tuning

The main software parameters tuned during development were sensor thresholds, detection distance, motor speed and steering angle. These parameters were changed after observing robot behavior during testing.

<img width="1135" height="407" alt="image" src="https://github.com/user-attachments/assets/9da94b8c-185c-4337-b8dd-42debd01b56a" />


The final report should retain the measured values from the team's actual tests. Values should not be invented merely to make the documentation appear complete.
