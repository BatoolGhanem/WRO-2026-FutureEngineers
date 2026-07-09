import cv2

cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Camera not opened")
    exit()

ret, frame = cap.read()

if ret:
    cv2.imwrite("camera_test.jpg", frame)
    print("Image saved: camera_test.jpg")
else:
    print("Failed to capture image")

cap.release()
