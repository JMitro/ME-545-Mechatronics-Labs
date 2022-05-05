# ME 545 Lab 5
# Group 5: Juliette Mitrovich, Sheila Moroney, Sujani Patel
# Isololate a tennis ball from an image, find its center coordinates, and send
# the value through the serial monitor

# Needed libraries
import numpy as np
import time
import string
import cv2
import serial
 

# initiate serial communication with Arduino
ser = serial.Serial('/dev/ttyACM0',9600)

# get the video from the camera using cv2
cap = cv2.VideoCapture(0)

while(True):
    ret, frame = cap.read()
    # set the upper and lower limit to isolate the green color of the tennis ball
    lower_green = np.array([27,35,30])
    upper_green = np.array([67,255,255])
    
    hsv = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)

    # create a mask for the picture and erode it to isolate the tennis ball
    mask = cv2.inRange(hsv, lower_green, upper_green)
    mask = cv2.erode(mask, None, iterations = 2)

    # find the contours of the tennis ball
    cnts = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
    hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
 
    # create a circle aroud the tennis ball and find the center coordinate
    if len(cnts) > 0:
        c = max(cnts, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        center = (int(x), int(y))
        radius = int(radius)
        if radius > 3:
            # display the circle on the image
            cv2.circle(frame, center, radius, (0, 255, 255), 2)
            cv2.circle(frame, center, 1, (255, 0, 0), 10)
            x_send = int(x*255/650)
 # change x value so it ranges from 0-255
            print (x_send)
 # print the value being sent for performance tracking
            ser.write([x_send])
 # send the data through the serial port

    cv2.imshow('mask',mask)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
 
 
cap.release()
cv2.destroyAllWindows()
ser.close()
