import time

import numpy as np
from numpy.typing import NDArray
from typing import Any

import cv2 as cv

from rich import print
from rich.progress import track
from rich.traceback import install
install(show_locals=True)

import serial

arduino = serial.Serial(port='COM3',   baudrate=115200, timeout=.1)

LIMIT = 100 # dunno, it's just a variable, man

cap = cv.VideoCapture(0)
if not cap.isOpened():
    print("Cannot open camera")
    exit()    
    
# Define the codec and create VideoWriter object
fourcc = cv.VideoWriter_fourcc(*'XVID')
out = cv.VideoWriter('output.avi', fourcc, 20.0, (1200, 960))

def var_of_laplace(img):
    return cv.Laplacian(img, cv.CV_64F).var()

def sobel_grad(img):
    scale = 1
    delta = 0
    ddepth = cv.CV_16S
    src = cv.GaussianBlur(img, (3, 3), 0)
    gray = cv.cvtColor(src, cv.COLOR_BGR2GRAY)    
    grad_x = cv.Sobel(gray, ddepth, 1, 0, ksize=3, scale=scale, delta=delta, borderType=cv.BORDER_DEFAULT)
    grad_y = cv.Sobel(gray, ddepth, 0, 1, ksize=3, scale=scale, delta=delta, borderType=cv.BORDER_DEFAULT)    
    abs_grad_x = cv.convertScaleAbs(grad_x)
    abs_grad_y = cv.convertScaleAbs(grad_y)
    return cv.addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0)

def transmit(x : NDArray) -> Any:
    for i in track(range(5)):
        arduino.write(bytes(x[i],   'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return  data

def recieve():
    data = arduino.readline()
    return  data


def do_focus_thing(focus_measure, dir):
    if dir==0: 
        return
    ## TODO move motor
    ## TODO in arduino add protocol for sending finished move (blocking the python script from spamming it)
        
    
direction = 0 # 0 nothing, 1 fwd, -1 bck
prev_focus_measure = 0
    
while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    height, width, channels = frame.shape
    
    # if frame is read correctly ret is True
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break
    # Our operations on the frame come here
    cv.rectangle(frame,(384,0),(510,128),(0,0,0),3) # img top-left bottom-right colour shift
    
    crop_height_top = height//4
    crop_height_bottom = 3*height//4
    
    crop_width_left = width//4
    crop_width_right = 3*width//4
    
    image = frame[crop_height_top:crop_height_bottom, crop_width_left:crop_width_right]
    
    focus_measure = var_of_laplace(image)
    
    if focus_measure < LIMIT:
        direction = np.sign(focus_measure-prev_focus_measure)
        do_focus_thing(focus_measure=focus_measure, dir=direction)
        
        
    prev_focus_measure = focus_measure
    
    font = cv.FONT_HERSHEY_SIMPLEX
    cv.putText(frame,'vdamani',(0,0), font, 1,(0,0,0),2,cv.LINE_AA)
    
    # Display the resulting frame
    cv.imshow('frame', frame)
    out.write(frame)
    
    if cv.waitKey(1) == ord('q'):
        break
 
# When everything done, release the capture
cap.release()
out.release()
cv.destroyAllWindows()


