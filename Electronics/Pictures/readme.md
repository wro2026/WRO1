Electronics

1. Controller

The robot uses the LEGO SPIKE Prime Hub as its main controller.
The SPIKE Prime Hub controls the robot's motors and receives information from the sensors. The robot also uses an ESP32 as part of the color-detection system because the SPIKE Prime system cannot directly understand the HuskyLens camera.

2. Sensors

The robot uses:
•	Distance sensors
•	SPIKE Prime Color Sensor
•	HuskyLens
The distance sensors are positioned on the sides of the robot. They are used to detect the distance between the robot and objects or walls.
The camera of the HuskyLens is positioned at the front of the robot and faces forward.

3. Color Detection System

The HuskyLens is used to detect colors. However, because the SPIKE Prime system cannot directly understand the HuskyLens, an ESP32 is used as an interface between the HuskyLens and the SPIKE Prime system.
The HuskyLens detects a change in color. The ESP32 receives this information and uses a servo to communicate the detected color change to the SPIKE Prime color sensor. The SPIKE Prime system can then use the information received through its color sensor.
This allows the robot to use the HuskyLens for more accurate color detection while still integrating the information into the SPIKE Prime system.

4. Power and Connections

The robot is powered by the SPIKE Prime rechargeable battery.
The SPIKE Prime components are connected to the SPIKE Prime Hub using their normal connections. The ESP32 and servo are used as part of the HuskyLens color-detection interface.
No additional power system or voltage regulation has been documented at this stage.

5. Sensor Selection and Placement

The distance sensors were selected to provide information about the robot's surroundings and are positioned on the sides of the robot.
The HuskyLens is mounted facing forward so that it can observe the area in front of the robot and detect colors.
The SPIKE Prime color sensor is used as the interface through which the color information can be communicated to the SPIKE Prime system.

6. Design Changes and Testing

Initially, the robot used only the SPIKE Prime color sensor for color detection.
During development, we found that using the HuskyLens provided more accurate color detection. We therefore added the HuskyLens and developed an interface using an ESP32 and servo so that the color information could be communicated to the SPIKE Prime system.
This change improved the accuracy of the color-detection system while allowing us to continue using SPIKE Prime as the main robot controller.

7. Reliability

The main electronics-related design challenge was integrating the HuskyLens with SPIKE Prime.
Since SPIKE Prime could not directly understand the HuskyLens, an additional ESP32 and servo interface was introduced. This allowed the two systems to work together while keeping SPIKE Prime as the main controller.
Further testing will be used to evaluate the reliability of the sensor system under different field conditions.
