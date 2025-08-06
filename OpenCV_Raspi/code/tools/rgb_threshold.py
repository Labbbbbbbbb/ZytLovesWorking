import numpy as np

import cv2

cap = cv2.VideoCapture(5)  
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 160)   # 设置图像宽度为160
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 120)  # 设置图像高度为120
cap.set(cv2.CAP_PROP_BRIGHTNESS, 1)     # 设置亮度，范围0-1
cap.set(cv2.CAP_PROP_AUTO_WB, 0)         # 关闭自动白平衡
# 关闭自动曝光（必须关闭才能手动设置）
cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)  # 0.25 或 0 表示手动曝光
# 调小曝光值（减少进光量）
cap.set(cv2.CAP_PROP_EXPOSURE, -6)  # 典型范围：-8（最暗）到 -1（较亮），或 0.1~0.001（秒）

# 全局定义段
# 1. 饱和度增强定义
# 调节通道强度aaaa
# 线性查找表（保持蓝色通道不变）
lutEqual = np.array([i for i in range(256)]).astype("uint8")

min_val, max_val = 1, 2
# 增强颜色通道的查找表（可调整参数）aaa
lutRaisen = np.array([0 if i < min_val else 255 if i > max_val else int((i - min_val) / (max_val - min_val) * 255) for i in range(256)]).astype("uint8")  # 增强红色


# 组合成三通道 LUT（R:增强, G:增强, B:不变）
lutSRG = np.dstack((lutEqual, lutRaisen, lutRaisen))  # 顺序：蓝，绿，红

# 2. 滑动条定义
cv2.namedWindow("TrackBars")


def empty(a):
    pass


cv2.resizeWindow("TrackBars", 640, 240)
cv2.createTrackbar("Red Min", "TrackBars", 0, 255, empty)
cv2.createTrackbar("Red Max", "TrackBars", 0, 255, empty)
cv2.createTrackbar("Blue Min", "TrackBars", 0, 255, empty)
cv2.createTrackbar("Blue Max", "TrackBars", 0, 255, empty)
cv2.createTrackbar("Green Min", "TrackBars", 0, 255, empty)
cv2.createTrackbar("Green Max", "TrackBars", 0, 255, empty)

#############################
# 运行段
#############################
while cap.isOpened():
    ret, frame = cap.read()
    if ret is True:
        # # 图像处理段
        # # # 1. 红绿对比度拉大
        # raisen = cv2.LUT(frame, lutSRG)
        # # 2. 掩膜生成aaa
        # r_min = cv2.getTrackbarPos("Red Min", "TrackBars")
        # r_max = cv2.getTrackbarPos("Red Max", "TrackBars")
        # b_min = cv2.getTrackbarPos("Blue Min", "TrackBars")
        # b_max = cv2.getTrackbarPos("Blue Max", "TrackBars")
        # g_min = cv2.getTrackbarPos("Green Min", "TrackBars")
        # g_max = cv2.getTrackbarPos("Green Max", "TrackBars")
        # lower = np.array([r_min, g_min, b_min])
        # upper = np.array([r_max, g_max, b_max])
        # #lower = np.array([0, 0, 0])
        # #upper = np.array([255, 255, 255])
        # mask = cv2.inRange(raisen, lower, upper)
        # #imgResult = cv2.bitwise_and(frame, frame, mask=mask)
        # imgResult = cv2.bitwise_and(frame,frame,mask=mask)
        # #cv2.imshow('result', imgResult)
        cv2.imshow('img',frame)

        key = cv2.waitKey(1)
        if key == 27:
            break
    else:
        break

cv2.destroyAllWindows()
cap.release()
