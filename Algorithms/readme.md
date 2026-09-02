4. Systems Thinking & Engineering Decisions

4.1 How the Subsystems Work Together

The robot is an integrated system rather than four independent parts. The mechanical system determines how quickly and accurately the robot can turn. The sensor architecture determines what information the software receives. The software converts sensor information into navigation decisions. Those decisions command the mechanical drive and steering system.
The main interaction chain is:
Field condition → sensors → ESP32/HuskyLens interface where required → SPIKE Hub → navigation decision → motor speed/steering → robot movement → new sensor readings
A change in one subsystem therefore affects the others. For example, increasing speed may improve straight-line travel time but can make turning less accurate. Similarly, moving a sensor changes the distance at which the software must react.

4.2 Engineering Constraints

•	The robot must operate autonomously.
•	The robot must complete 3 laps within the 3-minute objective.
•	The chassis must remain mechanically stable during repeated driving.
•	Steering must be controllable independently from propulsion.
•	Sensor information must be available in time for a turn decision.
•	HuskyLens must be integrated despite the SPIKE Prime compatibility limitation.
•	The design must remain practical to build and modify with the available LEGO system.

4.3 Major Engineering Decisions

Problem	Options	Decision	Reasoning
Main control platform	Raspberry Pi 4 or SPIKE Prime	SPIKE Prime	Reduced setup complexity and allowed faster development
Color detection	SPIKE color sensor only or HuskyLens	HuskyLens + ESP32 + servo interface	Improved color detection while retaining SPIKE as main controller
Drive transmission	Direct drive or gearing	Direct drive	Simple, compact and easy to modify
Steering	Single mechanical steering system or separate control	Separate large motor	Allows independent steering control
Navigation	Complex learning-based approach or simpler sensor logic	Sensor-based logic	Easier to test, tune and understand for the current task

4.4 Iteration Cycle

The development process followed an iterative engineering cycle:
Build → Test → Identify limitation → Change design → Retest → Select the better-performing configuration
Iteration 1 changed the main platform from Raspberry Pi 4 to SPIKE Prime because the first approach was too complex and slow to develop.
Iteration 2 changed the color-detection architecture by adding HuskyLens, ESP32 and a servo because the original color-sensing approach was less accurate.
Further tuning changed detection distances, motor speed and steering angle based on observed robot behavior.

4.5 Risk and Failure Analysis

Risk / failure point	Possible effect	Mitigation	Validation
Incorrect distance reading	Turn triggered too early/late	Use calibrated thresholds and repeat unstable detections	Repeated sensor tests
Color detection error	Wrong left/right decision	Use HuskyLens and validate color information	Repeated color tests
High speed during turn	Overshoot or missed path	Slow before turning	Compare turn behavior at different speeds
Steering backlash	Inconsistent turn angle	Use a fixed steering setting and check mechanical connections	Repeated turning tests
Wheel slip	Actual path differs from expected path	Use controlled speed and observe wheel/field interaction	Repeated distance/turn tests
Loose chassis connection	Mechanical instability	Inspect and reinforce critical LEGO connections	Repeated driving tests
Interface failure	HuskyLens information unavailable to SPIKE	Keep SPIKE color sensor as the communication pathway and retry detection	Integration tests

4.6 Testing Metrics

To make engineering decisions measurable, the final testing record should use repeatable metrics rather than subjective descriptions alone.

<img width="1132" height="337" alt="image" src="https://github.com/user-attachments/assets/43dc6ce5-7452-4c27-b7f9-34f1f2abaa60" />


4.7 Performance Objective

The algorithm's main objective is to complete 3 laps within 3 minutes while minimizing accidents. Therefore, the engineering goal is not simply maximum speed. The selected design balances speed, sensing reliability, steering control and mechanical stability.
The final configuration should be selected from repeated tests. If a higher speed reduces lap time but increases missed turns or accidents, the lower and more reliable speed is the better engineering choice for the competition objective.

4.8 Final Engineering Rationale

The final robot was developed by progressively reducing unnecessary complexity while improving the parts that directly affect autonomous performance. SPIKE Prime was selected as the main control platform because it accelerated development. Direct drive and LEGO construction were retained because they are simple and easy to modify. HuskyLens was added because improved color detection was valuable, while the ESP32 and servo interface solved the compatibility constraint.
The resulting architecture connects mechanical, electronic and software decisions: sensor placement determines when the software can detect a condition; detection thresholds determine when the robot begins a maneuver; motor speed and steering angle determine the physical result; and repeated testing is used to tune these parameters. This systems approach is intended to produce a robot that is not only functional, but also understandable, testable and reproducible.
