Algorithms

1. Main Objective

The main objective of the algorithm is to complete 3 laps within 3 minutes while minimizing the number of accidents.
The algorithm therefore focuses on reliable detection, correct turning decisions and consistent movement rather than only maximizing speed.

2. Environment Detection

The robot uses distance sensors to detect its surroundings.
A front distance sensor is used to detect objects in front of the robot.
There are also distance sensors on the sides of the robot. These provide information about the robot's position relative to objects or walls beside it.
The robot also uses its color-detection system to identify relevant color changes.

3. Turn Decision Algorithm

When the robot reaches a situation where a turn is required, it follows this general sequence:
Detect -> Check color -> Decide left/right -> Steer -> Continue

The distance sensors provide information about the environment, while the color information is checked before the robot decides which direction to take.

4. Left/Right Decision

The robot does not use a fixed direction for every turn.
After detecting the relevant situation and checking the color, the algorithm decides whether the robot should turn left or right.
The steering motor then moves the front wheels in the selected direction.

5. Obstacle Handling

When an obstacle or relevant object is detected, the robot uses the following general process:
1.	Detect the object using the distance sensors.
2.	Check the color information.
3.	Decide whether to go left or right.
4.	Steer in the selected direction.
5.	Continue the route.

If the robot receives an unexpected sensor reading, the current approach is to retry the detection/decision process rather than immediately continuing with an incorrect decision.


6. Algorithm Choice

We chose this approach because it is straightforward and easy to program.
Rather than using a highly complicated algorithm, the robot uses sensor information and simple decisions to determine its actions. This makes the behavior easier to understand, test and modify during development.

7. Testing and Algorithm Improvements

During testing, we changed the distance-sensor detection lengths.
Different detection distances were tested to determine how the robot should respond to objects in its environment.
These changes were made based on testing rather than being selected arbitrarily. Adjusting the detectiton distance helped us improve the robot's ability to detect situations requiring a decision.

