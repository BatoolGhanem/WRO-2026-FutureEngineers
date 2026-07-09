import time
from camera import Camera
from lane import detect_lane
from decision import make_decision
from serial_comm import SerialComm

def main():
    camera = Camera()
    uart = SerialComm()

    print("WRO Raspberry Pi program started")

    try:
        while True:
            frame = camera.read()

            if frame is None:
                uart.stop()
                time.sleep(0.2)
                continue

            lane_error = detect_lane(frame)
            steer, speed = make_decision(lane_error)

            uart.send_drive_command(steer, speed)
            time.sleep(0.05)

    except KeyboardInterrupt:
        print("Keyboard interrupt")

    finally:
        camera.release()
        uart.stop()
        print("Program stopped")

if __name__ == "__main__":
    main()
