1. Mobility & Mechanical Design

1.1 Chassis Design

Our robot chassis is constructed entirely from LEGO components. The approximate overall dimensions are 23 cm × 11 cm × 27 cm. The chassis uses a hollow LEGO structure to reduce unnecessary mass while retaining the structural members needed to mount the motors, wheels, hub and sensors.
LEGO was selected because it allows rapid mechanical iteration. During development, parts can be repositioned without permanently modifying the structure. This was particularly important when we changed the steering geometry and sensor positions during testing.
The robot has four wheels with an approximate diameter of 2 inches (5.08 cm). The SPIKE Prime Hub is positioned approximately near the middle of the chassis. This keeps the main mass close to the center of the vehicle and provides a practical mounting arrangement for the drive and steering systems.

1.2 Drive System

The robot uses one SPIKE Prime small motor for propulsion. The motor is connected directly to the front two wheels without intermediate gearing. The front wheels are mechanically linked through an axle so that the drive motor transfers rotation to both wheels.
The direct-drive arrangement was selected because it has few mechanical parts, is easy to inspect, and is simple to modify. The robot can drive forwards and backwards.
The main trade-off is that direct drive provides less opportunity to change wheel speed or torque through gearing. We accepted this trade-off because the current competition objective prioritizes predictable movement and reliable turning over a complex drivetrain.

1.3 Steering System

One SPIKE Prime large motor controls steering. The two front wheels are connected by a LEGO steering bar. When the steering motor rotates the bar, both front wheels change direction together.
The steering motor is mechanically separate from the drive motor. Separating propulsion and steering makes the control system easier to tune because drive speed and steering angle can be adjusted independently.
Testing showed that the current steering arrangement can complete the required turns reliably. Steering performance was improved by adjusting the steering angle and reducing drive speed during turns.

1.4 Torque and Speed Reasoning

The drive-wheel diameter is approximately 5.08 cm, giving a wheel circumference of approximately 15.96 cm. The theoretical linear speed can therefore be estimated from wheel rotational speed using v = πdN/60, where d is wheel diameter in metres and N is wheel speed in revolutions per minute.
For example, once the actual motor output speed under load is measured, it can be converted directly into the robot's theoretical wheel speed. The measured ground speed should then be compared with the theoretical value because wheel slip, load and surface friction can reduce the actual speed.
Wheel torque is important because the robot must accelerate the chassis and overcome rolling resistance while turning. The effective tractive force can be related to wheel torque by F = T/r, where T is wheel torque and r is wheel radius. Because the exact loaded motor torque has not yet been measured, we do not claim an unsupported numerical torque value. Instead, the design uses direct drive and moderate speed to favor predictable motion.
For the final build, the recommended validation measurements are: time over a fixed 1 m distance, maximum repeatable speed, loaded wheel torque if measurement equipment is available, and turning radius. These measurements should be recorded on the final competition robot rather than estimated from unloaded motor specifications.

1.5 Mechanical Stability and Rigidity

The four-wheel configuration provides four contact points with the field. The hollow chassis reduces mass while retaining the LEGO members needed to support the motors, wheels and electronics.
During normal driving tests, the chassis remained assembled and no major bending or separation was observed. The steering assembly also remained mechanically connected while changing direction.
The main mechanical risks are loose LEGO connections, steering backlash, wheel slip and excessive speed during turns. These risks are reduced by checking connections before runs, using a consistent steering setting, slowing before turns, and testing the robot repeatedly on the competition surface.

1.6 Design Choices and Trade-offs

Decision	Alternative considered	Reason for final choice	Engineering trade-off
LEGO chassis	More permanent/custom chassis	Fast modification and iteration	Less rigid than some custom structures
SPIKE Prime drive	Raspberry Pi-based system	Simpler motor/control integration	Less software flexibility
Direct drive	Intermediate gearing	Fewer mechanical parts	Less ability to trade speed for torque
Separate steering motor	Single combined mechanism	Independent steering control	Requires an additional motor

A major system-level decision was changing from the original Raspberry Pi 4 approach to SPIKE Prime. The Raspberry Pi version was difficult and time-consuming because development included a learning-based system and pairing/setup problems. The SPIKE Prime version allowed the team to make mechanical and software changes more quickly. The final choice therefore prioritized reliable development and repeatable testing.

1.7 Design Iterations and Testing

Version 1: Raspberry Pi 4

The first version used a Raspberry Pi 4. The main difficulty was system complexity. Development of the learning approach required significant time, while pairing and setup also caused delays. The slow development cycle made mechanical iteration harder.

Version 2: LEGO SPIKE Prime

The second version changed to SPIKE Prime with a LEGO chassis. This reduced setup complexity and made mechanical changes faster. The current design uses four wheels, 2-inch wheels, one small drive motor, one large steering motor, direct drive and a LEGO steering bar.
Testing and tuning focused on steering angle, drive speed and detection behavior. The robot slows before turns because a lower speed reduces overshoot and makes the steering response more predictable.
