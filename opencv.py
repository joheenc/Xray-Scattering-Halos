import cv2
import numpy as np

img = cv2.imread('sampleobs3.png', 0)
# img = cv2.medianBlur(img, 1)
cimg = cv2.cvtColor(img,cv2.COLOR_GRAY2BGR)

# Parameter for HoughCircles():
# src_gray: Input image (grayscale)
# circles: A vector that stores sets of 3 values: x_{c}, y_{c}, r for each detected circle.
# CV_HOUGH_GRADIENT: Define the detection method. Currently this is the only one available in OpenCV
# dp = 1: The inverse ratio of resolution
# min_dist = src_gray.rows/8: Minimum distance between detected centers
# param_1 = 200: Upper threshold for the internal Canny edge detector
# param_2 = 100*: Threshold for center detection.
# min_radius = 0: Minimum radio to be detected. If unknown, put zero as default.
# max_radius = 0: Maximum radius to be detected. If unknown, put zero as default
circles = cv2.HoughCircles(img,cv2.HOUGH_GRADIENT,1,1,
                            param1=30,param2=50,minRadius=0,maxRadius=0)

circles = np.uint16(np.around(circles))
for i in circles[0,:]:
    # draw the outer circle
    cv2.circle(cimg,(i[0],i[1]),i[2],(255,0,255),2)
    # draw the center of the circle
    cv2.circle(cimg,(i[0],i[1]),2,(0,100,100),3)

cv2.imshow('detected circles',cimg)
cv2.waitKey(0)
cv2.destroyAllWindows()
