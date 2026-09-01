Software

1. Programming Language

The robot is programmed using Python.
The software controls the SPIKE Prime Hub, motors and sensors and coordinates the robot's autonomous behavior.

2. Main Program

The robot follows a simple autonomous sequence.
At the beginning, the robot drives forward. The sensors are used to detect the environment and determine when a turn is required.
When a turn is detected, the robot changes its steering direction and turns. After completing the required turns, the robot continues until the stopping condition is reached.
The current program counts the turns made by the robot. After 12 turns, the robot continues for approximately 30 cm and then stops.

3. Lane and Environment Detection

The robot uses its distance sensors to detect its surroundings while driving.
There is one distance sensor on each side of the robot. These sensors provide distance information that the program can use while navigating.

4. Color and Obstacle Detection

The robot uses the HuskyLens and SPIKE Prime color sensor as part of its detection system.
The HuskyLens detects color information. Because SPIKE Prime cannot directly interpret the HuskyLens, an ESP32 and servo are used to communicate the detected color change to the SPIKE Prime color sensor.
The software can therefore use the color information together with the distance-sensor information during autonomous operation.

5. Steering Control

The steering is controlled using the large SPIKE Prime motor.
The program uses a predetermined steering angle when a turn is required. The steering angle is one of the values that has been adjusted during testing.
The robot normally drives at a fixed speed and slows down when turning. This allows the robot to approach turns at a lower speed than its normal driving speed.

6. Obstacle Strategy

The robot uses its distance sensors together with its color-detection system to recognize situations that require a change in its path.
The distance sensors provide information about the distance to objects on the left and right sides of the robot. The HuskyLens and color sensor provide color information.
The robot uses these inputs to determine when it needs to turn and continue its route.

7. Turn Counting and Stopping

The program keeps track of the number of turns completed by the robot.
The current stopping sequence is:

Start

     ↓

Drive forward

     ↓

Detect turn

     ↓

Slow down

     ↓

Turn

     ↓

Count turn

     ↓

12 turns completed?

  ├── No → Continue driving

  └── Yes
  
      ↓
  
Drive 30 cm
 
      ↓
  
  Stop

This allows the robot to complete the required sequence without relying only on a manually controlled stop.

8. Software Testing and Tuning

During development, we adjusted several software parameters based on testing.
The main values that were tuned were:
•	Sensor thresholds
•	Motor speed
•	Turning angle
Changing these values allowed us to improve the robot's behavior during driving and turning.
The robot's normal driving speed is fixed, while the robot uses a lower speed during turns.

9. Development Approach

We initially experimented with a Raspberry Pi 4 based approach. This required a learning-based system and took a significant amount of time to develop. Pairing was also difficult.
We later changed to SPIKE Prime with Python, which allowed the team to make progress more quickly and simplify the development process.
The current software therefore focuses on using sensor inputs, predetermined motor settings, steering angles and turn counting to achieve consistent autonomous behavior.

