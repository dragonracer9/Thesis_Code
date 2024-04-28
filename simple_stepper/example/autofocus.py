from typing import Any

from rich import print
from rich.progress import track
from rich.traceback import install
install(show_locals=True)

import serial
import numpy as np
from numpy.typing import NDArray
import time

import cv2 as cv

arduino = serial.Serial(port='COM3',   baudrate=115200, timeout=.1)


def transmit(x : NDArray) -> Any:
    for i in track(range(5)):
        arduino.write(bytes(x[i],   'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return   data


def focus():
    return 

# while True:
#     num = input("Enter a number: ")

packet = np.array([0, 0, 0, 0, 0])
value  = transmit(packet)
print(value)


if __name__ == "__main__":
    print("Hello, User!")
    # define a video capture object 
    vid = cv.VideoCapture(0) 
    
    while(True): 
        
        # Capture the video frame 
        # by frame 
        ret, frame = vid.read() 
    
        # Display the resulting frame 
        cv.imshow('frame', frame) 
        
        # the 'q' button is set as the 
        # quitting button you may use any 
        # desired button of your choice 
        if cv.waitKey(1) & 0xFF == ord('q'): 
            break
    
    # After the loop release the cap object 
    vid.release() 
    # Destroy all the windows 
    cv.destroyAllWindows()