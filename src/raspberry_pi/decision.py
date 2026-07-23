from config import DEFAULT_SPEED, MAX_STEER, KP_STEER

def clamp(value, minimum, maximum):
    return max(minimum, min(value, maximum))

def make_decision(lane_error):
    steer = int(lane_error * KP_STEER)
    steer = clamp(steer, -MAX_STEER, MAX_STEER)

    speed = DEFAULT_SPEED

    return steer, speed
