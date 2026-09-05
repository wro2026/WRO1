
# =============================================================================
# Pybricks (SPIKE Prime / Inventor Hub)
# WRO Obstacle Challenge — NO parallel parking
# -----------------------------------------------------------------------------
# Merge of:
#   * WRO Open Challenge corridor driver (90 deg gyro arcs at openings)
#   * HuskyLens color dodges (GREEN = LEFT, RED = RIGHT)
#
# PORT MAP
#     A = drive motor (rear, with encoder)
#     B = steering motor (front wheels)
#     C = distance sensor LEFT
#     D = distance sensor FRONT
#     E = distance sensor RIGHT
#     F = HuskyLens UART, Color Recognition, 9600 baud
#
# Teach the HuskyLens:
#     Color ID 1 = GREEN pillar  -> pass on the LEFT
#     Color ID 2 = RED   pillar  -> pass on the RIGHT
#
# WHAT THE CAR DOES
#     CRUISE     wall-center + IMU heading hold, watch for openings AND colors
#     COLOR      GREEN: 35 deg LEFT while the pillar is visible, then hold
#                50 deg RIGHT for 30 cm, straighten the wheels, then go
#                20 cm straight. RED is the mirror: 35 deg RIGHT, then
#                50 deg LEFT for 30 cm, straighten, 20 cm straight.
#                Corridor heading is KEPT (gyro is not zeroed) so the next
#                90 deg corner still lines up. Wall-centering is boosted
#                briefly so the car returns to the middle of the lane.
#     APPROACH   opening seen; hold heading until the front wall is close
#     TURN       closed-loop 90 deg gyro arc (never stops mid-run)
#     POST       short gyro-straight, then back to CRUISE
#     FINAL      after MAX_TURNS (12 = 3 laps of 4 corners), center for
#                FINAL_DRIVE_MM and stop. No parking.
#
# The car is not allowed to stop until every 90 deg turn is done.
# A corner turn is ONLY started when a real opening is detected (the open
# side reads farther than OPEN_DISTANCE_MM = 1000 mm) AND the front wall
# has been reached. A front wall alone -- e.g. a color block that the
# HuskyLens has not identified yet -- NEVER triggers a turn; the car just
# slows down and lets the color dodge handle it.
#
# STARTUP
#     1. Point the car straight down the corridor.
#     2. Physically straighten the front wheels.
#     3. Press Run. That heading becomes 0.
# =============================================================================

from pybricks.hubs import PrimeHub
from pybricks.pupdevices import Motor, UltrasonicSensor
from pybricks.parameters import Port, Stop, Icon, Color
from pybricks.tools import wait, StopWatch
from pybricks.iodevices import UARTDevice


# ------------------------- PORTS -------------------------
hub = PrimeHub()
drive = Motor(Port.A)
steer = Motor(Port.B)
left_sensor = UltrasonicSensor(Port.C)
front_sensor = UltrasonicSensor(Port.D)
right_sensor = UltrasonicSensor(Port.E)


# --------------------- DRIVE SETTINGS -------------------
# Regulated rear-motor speed, deg/s. Negative if the car drives backward.
DRIVE_SPEED = -500
TURN_SPEED = -280           # slower in the 90 deg arc (same SIGN as DRIVE)
STEERING_SPEED = 400       # deg/s of the steering servo

# Steering MOTOR angle, not the road-wheel angle.
MAX_STEERING_DEG = 32
MAX_TURN_STEERING_DEG = 40          # also the hard clamp for dodges/recovery
# STEERING_SIGN: +1 if a POSITIVE motor angle turns the car LEFT
# (same meaning as the working SPIKE 3 program). Flip to -1 if reversed.
STEERING_SIGN = 1
# YAW_SIGN: used after converting Pybricks heading (clockwise +)
# into a left-positive yaw like the SPIKE 3 program. Flip if turns go
# the wrong way.
YAW_SIGN = 1


# ---------------------- STOP SETTINGS -------------------
# Front-wall stopping is ONLY used in FINAL, after all MAX_TURNS are done.
STOP_DISTANCE_MM = 180
FRONT_CONFIRMATIONS = 2
FRONT_STOP_MAX_YAW_DEG = 12
EMERGENCY_STOP_DISTANCE_MM = 10


# ------------------- CRUISE CONTROLLER GAINS ------------
WALL_KP = 0.11
WALL_DEADBAND_MM = 8
WALL_FILTER_AMOUNT = 0.25
HEADING_KP = 1.20
YAW_DAMPING = 1.80

# After a color dodge, wall-centering is stronger for this long so the
# car hunts the middle of the lane instead of staying offset.
RECENTER_MS = 3000
RECENTER_WALL_KP = 0.20
RECENTER_DEADBAND_MM = 4


# ------------------- TURN DETECTION ---------------------
SPIKE_JUMP_MM = 500
SPIKE_CONFIRMATIONS = 4
BASELINE_FILTER_AMOUNT = 0.12
MAX_WALL_DISTANCE_MM = 2000
# An opening ONLY counts when the open side reads FARTHER than this.
# The GREEN/RED color blocks stand INSIDE the corridor, so the side
# sensors never read > 1000 mm for them -- a block can never look like
# an opening, and a front wall without a real opening never triggers a
# turn (the HuskyLens color dodge handles blocks instead).
OPEN_DISTANCE_MM = 1000


# ------------------- TURN EXECUTION ---------------------
TURN_AT_FRONT_DISTANCE_MM = 900
FRONT_CLOSE_CONFIRMATIONS = 1
WALL_RETURN_CONFIRMATIONS = 2
WALL_RETURN_MARGIN_MM = 150     # side wall counts as "back" if within baseline + this
MAX_APPROACH_DISTANCE_MM = 700
POST_TURN_DRIVE_MM = 100
ODOMETER_MM_PER_MOTOR_DEG = 0.49    # ~56 mm wheel; pi*56/360

TURN_ANGLE_DEG = 90
TURN_KP = 2.0
TURN_DONE_TOLERANCE_DEG = 6
TURN_TIMEOUT_MS = 6000

MAX_TURNS = 12                      # 3 laps x 4 corners
FINAL_DRIVE_MM = 300                # settle between walls, then stop

# When something is straight ahead but NO opening was detected, never turn:
# it is almost certainly a color block the HuskyLens has not identified yet.
# Slow down so the camera has time to see the color, and the color dodge
# takes over from there.
BLOCK_AHEAD_SLOWDOWN_MM = 500
BLOCK_AHEAD_SPEED = -200


# ------------------- COLOR (HuskyLens) ------------------
#  GREEN  -> pass on the LEFT   (dodge LEFT, recover RIGHT)
#  RED    -> pass on the RIGHT  (dodge RIGHT, recover LEFT)
#  Logical steering convention here: + = wheels RIGHT, - = wheels LEFT.
GREEN_ID = 1                        # color ID of the GREEN pillar
RED_ID = 2                          # color ID of the RED pillar
GREEN_STEER = -35                   # -35 logical  = LEFT dodge
GREEN_RECOVER = 50                  # +50 logical  = RIGHT recovery
RED_STEER = 35                      # +35 logical  = RIGHT dodge
RED_RECOVER = -50                   # -50 logical  = LEFT recovery
COLOR_RECOVER_MM = 300              # 30 cm HOLDING the recovery angle
RECOVER_STRAIGHT_MM = 200           # 20 cm straight after straightening
COLOR_LOST_CONFIRM_MS = 300

HUSKY_PORT = Port.F
HUSKY_BAUD = 9600
HUSKY_POWER_PIN = 0                 # 2 if the camera needs hub pin power
HUSKY_TIMEOUT_MS = 60


LOOP_TIME_MS = 50
PRINT_DEBUG = False


# ------------------------ MODES -------------------------
MODE_CRUISE = 0
MODE_APPROACH = 1
MODE_TURN = 2
MODE_POST = 3
MODE_FINAL = 4
MODE_NAMES = ("CRUISE", "APPROACH", "TURN", "POST", "FINAL")


# ------------------------------------------------------------------ helpers
def clamp(value, low, high):
    if value < low:
        return low
    if value > high:
        return high
    return value


def distance_is_valid(value):
    return value is not None and value > 0


def read_mm(sensor):
    """Pybricks returns 2000 mm when nothing is seen; map that to -1."""
    try:
        d = sensor.distance()
    except OSError:
        return -1
    if d is None or d <= 0 or d >= 2000:
        return -1
    return d


def angle_difference(new_angle, old_angle):
    difference = new_angle - old_angle
    while difference > 180:
        difference -= 360
    while difference < -180:
        difference += 360
    return difference


def wrap_angle(angle):
    while angle > 180:
        angle -= 360
    while angle <= -180:
        angle += 360
    return angle


def yaw_left_positive():
    """
    Pybricks hub.imu.heading() is clockwise-positive (RIGHT).
    The Open Challenge controller is left-positive. Convert here so the
    rest of the math matches the working SPIKE 3 program.
    """
    return wrap_angle(YAW_SIGN * (-hub.imu.heading()))


def traveled_mm(start_position):
    return abs(drive.angle() - start_position) * ODOMETER_MM_PER_MOTOR_DEG


def update_side_reading(reading_mm, baseline, open_count, open_threshold_mm):
    """
    Wall tracking for one side. An 'open' count only builds up when the
    side truly opens up: the reading jumps away from the wall baseline AND
    is farther than open_threshold_mm (1000 mm). A missing reading means
    no wall within 2 m -- that is open too. Wall-like readings (a color
    block in the corridor) always stay below the threshold and never count.
    """
    if distance_is_valid(reading_mm):
        if baseline is None:
            if reading_mm <= MAX_WALL_DISTANCE_MM:
                baseline = reading_mm
            return baseline, 0
        if (
            reading_mm > baseline + SPIKE_JUMP_MM
            and reading_mm > open_threshold_mm
        ):
            return baseline, open_count + 1
        baseline = baseline + (reading_mm - baseline) * BASELINE_FILTER_AMOUNT
        return baseline, 0
    if baseline is not None:
        return baseline, open_count + 1
    return baseline, 0



def u16le(data, index):
    return data[index] | (data[index + 1] << 8)


# ------------------------------------------------------------------ motors
def start_drive(speed=None):
    drive.run(DRIVE_SPEED if speed is None else speed)


def stop_drive():
    drive.brake()


def apply_steering(conceptual_left_deg, last_target, limit=None):
    """
    conceptual_left_deg: + = turn LEFT (SPIKE 3 convention).
    STEERING_SIGN maps that onto the motor.
    """
    if limit is None:
        limit = MAX_TURN_STEERING_DEG
    target = int(STEERING_SIGN * conceptual_left_deg)
    target = clamp(target, -limit, limit)
    if target != last_target:
        steer.run_target(STEERING_SPEED, target, then=Stop.HOLD, wait=False)
    return target


def steer_logical(logical_right_deg, wait_done=True):
    """logical_right_deg: + = wheels RIGHT, - = wheels LEFT."""
    # conceptual_left = -logical_right, then STEERING_SIGN
    motor_angle = STEERING_SIGN * (-logical_right_deg)
    motor_angle = clamp(motor_angle, -MAX_TURN_STEERING_DEG, MAX_TURN_STEERING_DEG)
    steer.run_target(STEERING_SPEED, motor_angle, then=Stop.HOLD, wait=wait_done)


# ------------------------------------------------------------------ HuskyLens
CMD_KNOCK = bytes((0x55, 0xAA, 0x11, 0x00, 0x2C, 0x3C))
CMD_BLOCKS_LEARNED = bytes((0x55, 0xAA, 0x11, 0x00, 0x24, 0x34))
CMD_OK = 0x2E
CMD_INFO = 0x29
CMD_BLOCK = 0x2A


def _checksum(payload):
    s = 0
    for b in payload:
        s += b
    return s & 0xFF


def cmd_blocks_by_id(color_id):
    frame = bytearray((0x55, 0xAA, 0x11, 0x02, 0x27, color_id & 0xFF, 0x00))
    frame.append(_checksum(frame))
    return bytes(frame)


class HuskyLens:
    def __init__(self, port, baudrate=9600, power_pin=0):
        kwargs = {"baudrate": baudrate, "timeout": 200}
        if power_pin:
            kwargs["power_pin"] = power_pin
        self.uart = UARTDevice(port, **kwargs)
        wait(300)
        self.uart.clear()

    def _read_exact(self, n, timeout_ms):
        watch = StopWatch()
        data = b""
        while len(data) < n and watch.time() < timeout_ms:
            waiting = self.uart.waiting()
            if waiting:
                take = min(waiting, n - len(data))
                data += self.uart.read(take)
            else:
                wait(1)
        return data

    def _read_packet(self, timeout_ms):
        watch = StopWatch()
        while watch.time() < timeout_ms:
            hdr = self._read_exact(2, max(5, timeout_ms - watch.time()))
            if len(hdr) < 2:
                return None, b""
            if hdr[0] == 0x55 and hdr[1] == 0xAA:
                rest = self._read_exact(3, 40)
                if len(rest) < 3:
                    return None, b""
                address, length, cmd = rest[0], rest[1], rest[2]
                if address != 0x11:
                    continue
                payload = self._read_exact(length + 1, 40)
                if len(payload) < length + 1:
                    return None, b""
                return cmd, payload[:length]
            if hdr[1] == 0x55:
                extra = self._read_exact(1, 10)
                if extra == b"\xAA":
                    rest = self._read_exact(3, 40)
                    if len(rest) == 3 and rest[0] == 0x11:
                        length, cmd = rest[1], rest[2]
                        payload = self._read_exact(length + 1, 40)
                        if len(payload) >= length + 1:
                            return cmd, payload[:length]
        return None, b""

    def knock(self):
        try:
            self.uart.clear()
            self.uart.write(CMD_KNOCK)
            cmd, _ = self._read_packet(150)
            return cmd == CMD_OK
        except OSError:
            return False

    def seen_ids(self, wanted=(1, 2)):
        found = set()
        try:
            self.uart.clear()
            self.uart.write(CMD_BLOCKS_LEARNED)
            cmd, payload = self._read_packet(HUSKY_TIMEOUT_MS)
            if cmd != CMD_INFO or len(payload) < 2:
                return self._seen_by_id(wanted)
            n = u16le(payload, 0)
            for _ in range(n):
                bcmd, bpay = self._read_packet(40)
                if bcmd != CMD_BLOCK or len(bpay) < 10:
                    break
                block_id = u16le(bpay, 8)
                if block_id in wanted:
                    found.add(block_id)
            return found
        except OSError:
            return found

    def _seen_by_id(self, wanted):
        found = set()
        for color_id in wanted:
            try:
                self.uart.clear()
                self.uart.write(cmd_blocks_by_id(color_id))
                cmd, payload = self._read_packet(HUSKY_TIMEOUT_MS)
                if cmd == CMD_INFO and len(payload) >= 2 and u16le(payload, 0) > 0:
                    found.add(color_id)
                    self._read_packet(40)
            except OSError:
                pass
        return found


# ------------------------------------------------------------------ color dodge
def drive_while_color_visible(husky, color_id, logical_steer):
    """Hold 45 deg and keep driving until that color has been gone a moment."""
    steer_logical(logical_steer, wait_done=False)
    start_drive()
    lost = StopWatch()
    while True:
        steer_logical(logical_steer, wait_done=False)
        start_drive()
        if color_id in husky.seen_ids((color_id,)):
            lost.reset()
        elif lost.time() >= COLOR_LOST_CONFIRM_MS:
            break
        wait(10)


def drive_mm_while_steering(distance_mm, steer_start, steer_end):
    """Drive distance_mm, slewing steering from start to end. Keep rolling.
    Pass steer_start == steer_end to HOLD a constant angle for the distance."""
    target_motor = distance_mm / ODOMETER_MM_PER_MOTOR_DEG
    if target_motor < 1:
        return
    start_ang = drive.angle()
    steer_logical(steer_start, wait_done=True)
    start_drive()
    last_logical = None
    while abs(drive.angle() - start_ang) < target_motor:
        travelled = abs(drive.angle() - start_ang)
        progress = travelled / target_motor
        if progress > 1:
            progress = 1
        logical = steer_start + (steer_end - steer_start) * progress
        if logical != last_logical:
            steer_logical(logical, wait_done=False)
            last_logical = logical
        wait(10)


def color_dodge(husky, color_id, first_steer, recover_steer):
    """Dodge at first_steer while the color is visible, then recover.
       GREEN: first_steer=-35 (LEFT),  recover_steer=+50 (RIGHT)
       RED:   first_steer=+35 (RIGHT), recover_steer=-50 (LEFT)
       Sequence: hold the recovery angle for COLOR_RECOVER_MM (30 cm),
       straighten the wheels, then drive RECOVER_STRAIGHT_MM (20 cm)
       straight. The car never stops."""
    drive_while_color_visible(husky, color_id, first_steer)
    drive_mm_while_steering(COLOR_RECOVER_MM, recover_steer, recover_steer)
    steer_logical(0, wait_done=True)            # straighten tires, keep rolling
    drive_mm_while_steering(RECOVER_STRAIGHT_MM, 0, 0)
    start_drive()


# ------------------------------------------------------------------ main run
def drive_course(husky, connected):
    """
    One full attempt. Returns only after MAX_TURNS + FINAL_DRIVE_MM
    (or a FINAL-phase front wall). Never returns mid-run.
    """
    reference_yaw = yaw_left_positive()
    corridor_yaw = reference_yaw          # current corridor heading (kept, never zeroed)

    filtered_wall_error = 0.0
    previous_yaw_deg = reference_yaw
    last_steering_target = 0
    close_front_count = 0

    baseline_left = None
    baseline_right = None
    open_count_left = 0
    open_count_right = 0

    mode = MODE_CRUISE
    turn_direction = 0
    turn_target_yaw = reference_yaw
    turn_watch = StopWatch()
    phase_start_position = drive.angle()
    turn_side_baseline = None
    front_close_count = 0
    wall_return_count = 0
    turns_completed = 0

    green_latched = False
    red_latched = False
    clock = StopWatch()
    recenter_until = 0

    print("Reference yaw: %.1f deg" % reference_yaw)
    start_drive()

    while True:
        loop_watch = StopWatch()

        yaw_deg = yaw_left_positive()
        yaw_change = angle_difference(yaw_deg, previous_yaw_deg)
        previous_yaw_deg = yaw_deg
        heading_error = angle_difference(yaw_deg, corridor_yaw)

        front_mm = read_mm(front_sensor)
        left_mm = read_mm(left_sensor)
        right_mm = read_mm(right_sensor)

        all_turns_done = turns_completed >= MAX_TURNS

        # ---- FINAL-only stop (cannot fire before all 90 deg turns) ----
        if mode == MODE_FINAL and all_turns_done:
            emergency_stop = (
                distance_is_valid(front_mm)
                and front_mm <= EMERGENCY_STOP_DISTANCE_MM
            )
            normal_front_wall = (
                distance_is_valid(front_mm)
                and front_mm <= STOP_DISTANCE_MM
                and abs(heading_error) <= FRONT_STOP_MAX_YAW_DEG
            )
            if normal_front_wall:
                close_front_count += 1
            else:
                close_front_count = 0
            if emergency_stop or close_front_count >= FRONT_CONFIRMATIONS:
                stop_drive()
                steer_logical(0, wait_done=True)
                print("Final approach front wall - stopping")
                return
        else:
            close_front_count = 0

        # ---- GREEN / RED pillars (CRUISE only) ----
        # Do this BEFORE opening detection so a pillar is never mistaken
        # for a corner. Corridor heading (corridor_yaw) is kept.
        if mode == MODE_CRUISE and connected:
            seen = husky.seen_ids((GREEN_ID, RED_ID))
            if PRINT_DEBUG and seen:
                print("COLORS seen:", sorted(seen))

            # GREEN -> dodge LEFT (logical -35), recover RIGHT (+50)
            if GREEN_ID in seen and not green_latched:
                green_latched = True
                print("GREEN - pass on the LEFT")
                hub.light.on(Color.GREEN)
                hub.display.icon(Icon.ARROW_LEFT)
                color_dodge(husky, GREEN_ID, GREEN_STEER, GREEN_RECOVER)
                baseline_left = None
                baseline_right = None
                open_count_left = 0
                open_count_right = 0
                filtered_wall_error = 0.0
                last_steering_target = 0
                previous_yaw_deg = yaw_left_positive()
                recenter_until = clock.time() + RECENTER_MS
                hub.light.on(Color.WHITE)
                hub.display.icon(Icon.TRUE)
                start_drive()
                continue

            if GREEN_ID not in seen:
                green_latched = False

            # RED -> dodge RIGHT (logical +35), recover LEFT (-50)
            if RED_ID in seen and not red_latched:
                red_latched = True
                print("RED - pass on the RIGHT")
                hub.light.on(Color.RED)
                hub.display.icon(Icon.ARROW_RIGHT)
                color_dodge(husky, RED_ID, RED_STEER, RED_RECOVER)
                baseline_left = None
                baseline_right = None
                open_count_left = 0
                open_count_right = 0
                filtered_wall_error = 0.0
                last_steering_target = 0
                previous_yaw_deg = yaw_left_positive()
                recenter_until = clock.time() + RECENTER_MS
                hub.light.on(Color.WHITE)
                hub.display.icon(Icon.TRUE)
                start_drive()
                continue

            if RED_ID not in seen:
                red_latched = False

        # ---- opening detection (CRUISE) ----
        if mode == MODE_CRUISE:
            baseline_left, open_count_left = update_side_reading(
                left_mm, baseline_left, open_count_left, OPEN_DISTANCE_MM
            )
            baseline_right, open_count_right = update_side_reading(
                right_mm, baseline_right, open_count_right, OPEN_DISTANCE_MM
            )

            left_triggered = open_count_left >= SPIKE_CONFIRMATIONS
            right_triggered = open_count_right >= SPIKE_CONFIRMATIONS

            if left_triggered or right_triggered:
                if left_triggered and (
                    not right_triggered or open_count_left >= open_count_right
                ):
                    turn_direction = 1
                    turn_side_baseline = baseline_left
                    print("Opening on the LEFT - waiting for front wall")
                else:
                    turn_direction = -1
                    turn_side_baseline = baseline_right
                    print("Opening on the RIGHT - waiting for front wall")
                front_close_count = 0
                wall_return_count = 0
                mode = MODE_APPROACH
                phase_start_position = drive.angle()


        # ---- APPROACH: hold heading until the arc should start ----
        if mode == MODE_APPROACH:
            opening_side_mm = left_mm if turn_direction > 0 else right_mm

            # If the "opening" wall closes up again, it was a false spike:
            # cancel the turn and re-detect from CRUISE.
            wall_back = (
                distance_is_valid(opening_side_mm)
                and turn_side_baseline is not None
                and opening_side_mm <= turn_side_baseline + WALL_RETURN_MARGIN_MM
            )
            if wall_back:
                wall_return_count += 1
            else:
                wall_return_count = 0
            if (
                wall_return_count >= WALL_RETURN_CONFIRMATIONS
                and (
                    not distance_is_valid(front_mm)
                    or front_mm > TURN_AT_FRONT_DISTANCE_MM + SPIKE_JUMP_MM
                )
            ):
                print("Opening closed again - back to CRUISE")
                mode = MODE_CRUISE
                baseline_left = None
                baseline_right = None
                open_count_left = 0
                open_count_right = 0
                continue

            start_turn = False

            if (
                distance_is_valid(front_mm)
                and front_mm <= TURN_AT_FRONT_DISTANCE_MM
            ):
                front_close_count += 1
            else:
                front_close_count = 0
            if front_close_count >= FRONT_CLOSE_CONFIRMATIONS:
                start_turn = True
            elif traveled_mm(phase_start_position) >= MAX_APPROACH_DISTANCE_MM:
                # Opening detected but the front wall never came close enough;
                # turn into the opening anyway rather than plowing ahead.
                print("Approach distance timeout - turning into the opening")
                start_turn = True

            if start_turn:
                turn_target_yaw = wrap_angle(
                    corridor_yaw + turn_direction * TURN_ANGLE_DEG
                )
                turn_watch.reset()
                mode = MODE_TURN
                print(
                    "TURN %s to %.1f deg"
                    % ("LEFT" if turn_direction > 0 else "RIGHT", turn_target_yaw)
                )
                hub.display.icon(
                    Icon.ARROW_LEFT if turn_direction > 0 else Icon.ARROW_RIGHT
                )
                continue

        # ---- TURN: closed-loop 90 deg gyro arc (never stops mid-run) ----
        if mode == MODE_TURN:
            turn_error = angle_difference(turn_target_yaw, yaw_deg)
            steer_cmd_turn = clamp(
                TURN_KP * turn_error,
                -MAX_TURN_STEERING_DEG,
                MAX_TURN_STEERING_DEG,
            )
            last_steering_target = apply_steering(
                steer_cmd_turn, last_steering_target, MAX_TURN_STEERING_DEG
            )
            start_drive(TURN_SPEED)

            if abs(turn_error) <= TURN_DONE_TOLERANCE_DEG:
                corridor_yaw = turn_target_yaw
                mode = MODE_POST
                phase_start_position = drive.angle()
                print("Turn done -> POST")
                continue
            if turn_watch.time() >= TURN_TIMEOUT_MS:
                corridor_yaw = turn_target_yaw
                mode = MODE_POST
                phase_start_position = drive.angle()
                print("TURN TIMEOUT - forcing POST")
                continue

        # ---- POST: short gyro-straight, then back to CRUISE ----
        if mode == MODE_POST:
            if traveled_mm(phase_start_position) >= POST_TURN_DRIVE_MM:
                turns_completed += 1
                print(
                    "Turns completed: %d / %d"
                    % (turns_completed, MAX_TURNS)
                )
                baseline_left = None
                baseline_right = None
                open_count_left = 0
                open_count_right = 0
                filtered_wall_error = 0.0
                recenter_until = clock.time() + RECENTER_MS
                if turns_completed >= MAX_TURNS:
                    mode = MODE_FINAL
                    phase_start_position = drive.angle()
                    print("FINAL: all turns done - driving to the finish")
                    hub.display.icon(Icon.TRUE)
                    continue
                mode = MODE_CRUISE
                print("Back to CRUISE")
                continue

        # ---- FINAL: settle for FINAL_DRIVE_MM, then stop ----
        if mode == MODE_FINAL:
            if traveled_mm(phase_start_position) >= FINAL_DRIVE_MM:
                stop_drive()
                steer_logical(0, wait_done=True)
                print("Course complete - stopping")
                hub.display.icon(Icon.TRUE)
                return

        # ---- steering controller + drive ----
        # IMPORTANT: this whole block is SKIPPED during MODE_TURN. The TURN
        # block above already commands the steering and the drive speed, and
        # it runs later in the loop -- if this block ran too it would
        # OVERRIDE the turn command every cycle (the cruise controller is
        # clamped to 32 deg and uses corridor_yaw, not turn_target_yaw).
        # That override is why the car turned the wrong way, turned very
        # late, or never turned at all and hit the front wall.
        if mode != MODE_TURN:
            steer_cmd = 0.0
            kp_wall_use = WALL_KP
            deadband_use = WALL_DEADBAND_MM
            if mode == MODE_CRUISE and clock.time() < recenter_until:
                kp_wall_use = RECENTER_WALL_KP
                deadband_use = RECENTER_DEADBAND_MM

            # heading hold (left-positive). NOTE: the P term is negative on
            # purpose -- heading_error > 0 means the car points LEFT of the
            # corridor, so it must steer RIGHT (negative) to come back.
            steer_cmd = -HEADING_KP * heading_error - YAW_DAMPING * yaw_change

            # wall-centering (CRUISE and FINAL only; needs both side walls)
            if (
                (mode == MODE_CRUISE or mode == MODE_FINAL)
                and distance_is_valid(left_mm)
                and distance_is_valid(right_mm)
            ):
                raw_wall = (left_mm - right_mm) / 2.0   # + = too close to RIGHT wall
                if abs(raw_wall) <= deadband_use:
                    raw_wall = 0.0
                filtered_wall_error += (
                    raw_wall - filtered_wall_error
                ) * WALL_FILTER_AMOUNT
                steer_cmd += kp_wall_use * filtered_wall_error

            last_steering_target = apply_steering(
                steer_cmd, last_steering_target, MAX_STEERING_DEG
            )

            # Speed: in CRUISE, a close front wall with NO opening detected
            # means a color block that has not been identified yet -- slow
            # down and keep going straight. No turn, no stop.
            if (
                mode == MODE_CRUISE
                and distance_is_valid(front_mm)
                and front_mm <= BLOCK_AHEAD_SLOWDOWN_MM
            ):
                start_drive(BLOCK_AHEAD_SPEED)
            else:
                start_drive()

        if PRINT_DEBUG:
            print(
                MODE_NAMES[mode],
                "yaw=%.0f" % yaw_deg,
                "f=%d l=%d r=%d" % (front_mm, left_mm, right_mm),
                "steer=%d" % last_steering_target,
            )

        wait(LOOP_TIME_MS)


# ------------------------------------------------------------------ entry
def main():
    print("Starting WRO obstacle course ...")

    husky = None
    connected = False
    try:
        husky = HuskyLens(HUSKY_PORT, HUSKY_BAUD, HUSKY_POWER_PIN)
        connected = husky.knock()
    except OSError as err:
        print("HuskyLens error:", err)
        connected = False

    if connected:
        print("HuskyLens connected")
        hub.light.on(Color.WHITE)
        hub.display.icon(Icon.HEART)
    else:
        print("HuskyLens NOT connected - running blind")
        hub.light.on(Color.ORANGE)
        hub.display.icon(Icon.SAD)
    wait(1000)

    try:
        drive_course(husky, connected)
    except Exception as err:
        print("Run aborted:", err)
        stop_drive()
        steer_logical(0, wait_done=False)
        hub.light.on(Color.ORANGE)
        raise


main()
