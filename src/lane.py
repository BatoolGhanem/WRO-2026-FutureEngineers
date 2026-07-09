import cv2

def detect_lane(frame):
    height, width, _ = frame.shape

    roi = frame[int(height * 0.55):height, :]

    gray = cv2.cvtColor(roi, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
    edges = cv2.Canny(blur, 50, 150)

    moments = cv2.moments(edges)

    if moments["m00"] == 0:
        return 0

    cx = int(moments["m10"] / moments["m00"])
    center = width // 2
    error = cx - center

    return error
