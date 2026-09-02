2. Power & Sensor Architecture

2.1 System Architecture

The SPIKE Prime Hub is the main controller. It controls the drive and steering motors and processes information used for autonomous navigation. An ESP32 is also used as an interface because the SPIKE Prime system cannot directly interpret the HuskyLens camera.
The main information flow is:
Distance sensors → SPIKE Prime Hub → navigation decisions
HuskyLens → ESP32 → servo interface → SPIKE Prime color sensor → SPIKE Prime Hub
SPIKE Prime rechargeable battery → SPIKE Prime Hub → SPIKE Prime components

2.2 Power System

The robot is powered by the SPIKE Prime rechargeable battery. The SPIKE Prime motors and sensors use the Hub's normal connections. The ESP32 and servo form the additional HuskyLens interface.
The present documentation does not contain measured current values for each component, so unsupported current figures are not stated. For a complete final power budget, current should be measured for the drive motor, steering motor, Hub electronics, ESP32, servo and sensors under representative operating conditions.

<img width="1094" height="378" alt="image" src="https://github.com/user-attachments/assets/e36cee03-14f0-460b-828a-d4423d17754b" />


2.3 Sensor Selection

The robot uses distance sensors, the SPIKE Prime color sensor and HuskyLens.
•	Distance sensors provide information about objects or walls around the robot.
•	The HuskyLens provides color information from the front of the robot.
•	The SPIKE Prime color sensor provides the input pathway through which the HuskyLens color information is communicated to the SPIKE system.
Initially, the robot relied on the SPIKE Prime color sensor. During development, the team found that HuskyLens gave more accurate color detection. Instead of abandoning SPIKE Prime, an ESP32 and servo interface was added. This preserved SPIKE Prime as the main controller while improving color detection.

2.4 Sensor Placement and Field Geometry

The distance sensors are positioned on the sides of the robot so that they can observe nearby walls or objects relative to the robot's lateral position. This information helps the robot determine when a navigation decision is required.
The HuskyLens is mounted at the front and faces forward because color information must be obtained from the area ahead of the robot. The SPIKE Prime color sensor is positioned so that the servo-operated interface can communicate the detected color change.
Placement is therefore linked to the geometry of the field: side sensors observe lateral boundaries, while the forward-facing camera observes information in the robot's path.

2.5 Calibration and Reliability

Sensor thresholds and detection distances are treated as adjustable parameters rather than permanent assumptions. During testing, different distance thresholds were tried to determine when the robot should respond to an object or field condition.
A practical calibration procedure is to collect repeated readings at known distances, identify a stable threshold that separates the required field condition from normal travel, and then repeat the test several times. The final threshold should be selected from the repeated test results.
For reliability, an unexpected or unstable sensor reading should not immediately trigger a turn. The software should repeat the detection/decision process and only continue when a valid condition is identified. This reduces the effect of a single noisy reading.

2.6 Electronics Design Change

The major electronics iteration was the addition of HuskyLens, ESP32 and servo. The original SPIKE color sensor was not sufficiently accurate for the team's color-detection requirement. HuskyLens improved detection, but it could not directly communicate with SPIKE Prime. The interface solved this compatibility constraint.
This demonstrates a subsystem trade-off: improving sensing accuracy increased electronics complexity. The team accepted the additional ESP32 and servo because the resulting detection system was more useful while SPIKE Prime could remain the main controller.

