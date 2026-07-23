import serial
import time
from config import SERIAL_PORT, BAUD_RATE

class SerialComm:
    def __init__(self):
        try:
            self.ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.2)
            time.sleep(2)
            self.connected = True
            print("UART connected")
        except Exception as e:
            self.ser = None
            self.connected = False
            print("UART not connected:", e)

    def send_drive_command(self, steer, speed):
        command = f"STEER:{steer},SPEED:{speed}\n"

        if self.connected:
            self.ser.write(command.encode())

        print(command.strip())

    def stop(self):
        if self.connected:
            self.ser.write(b"STOP\n")
        print("STOP")
