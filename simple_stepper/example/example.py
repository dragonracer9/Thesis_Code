from typing import Any

from rich import print
from rich.progress import track
from rich.traceback import install
install(show_locals=True)

import serial
import numpy as np
from numpy.typing import NDArray
import time

arduino = serial.Serial(port='COM3',   baudrate=115200, timeout=.1)


def transmit(x : NDArray) -> Any:
    for i in track(range(5)):
        arduino.write(bytes(x[i],   'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return   data


# while True:
#     num = input("Enter a number: ")

packet = np.array([0, 0, 0, 0, 0])
value  = transmit(packet)
print(value)
