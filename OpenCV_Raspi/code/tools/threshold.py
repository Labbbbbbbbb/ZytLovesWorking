"""双目摄像头饱和度增强实验
    @Time   2023.10.24
    @Author SSC
"""
import numpy as np
import cv2



# 初始化摄像头aaaaaaaaa
cap = cv2.VideoCapture(0)  
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 160)   # 设置图像宽度为320
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 120)  # 设置图像高度为240
cap.set(cv2.CAP_PROP_BRIGHTNESS, 1.0)    # 设置亮度，范围0-1
cap.set(cv2.CAP_PROP_AUTO_WB, 0)         # 关闭自动白平衡
# 关闭自动曝光（必须关闭才能手动设置）
cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)  # 0.25 或 0 表示手动曝光
# 调小曝光值（减少进光量）
cap.set(cv2.CAP_PROP_EXPOSURE, -6)  # 典型范围：-8（最暗）到 -1（较亮），或 0.1~0.001（秒）cap.set(10, -2)

# 全局定义段
# 1. 饱和度增强定义
# # 调节通道强度
lutEqual = np.array([i for i in range(256)]).astype("uint8")
min_val, max_val = 150, 151
lutRaisen = np.array([0 if i < min_val else 255 if i > max_val else int((i - min_val) / (max_val - min_val) * 255) for i in range(256)]).astype("uint8")
# lutRaisen = np.array([int(102+0.6*i) for i in range(256)]).astype("uint8")
# # 调节饱和度
lutSRaisen = np.dstack((lutRaisen, lutEqual, lutEqual))  # Saturation raisen
# 2. 滑动条定义
cv2.namedWindow("TrackBars")


def empty(a):
    pass


cv2.resizeWindow("TrackBars", 640, 240)
cv2.createTrackbar("Hue Min", "TrackBars", 0, 179, empty)
cv2.createTrackbar("Hue Max", "TrackBars", 0, 179, empty)
cv2.createTrackbar("Sat Min", "TrackBars", 0, 255, empty)
cv2.createTrackbar("Sat Max", "TrackBars", 0, 255, empty)
cv2.createTrackbar("Val Min", "TrackBars", 0, 255, empty)
cv2.createTrackbar("Val Max", "TrackBars", 0, 255, empty)

#############################
# 运行段
#############################
while cap.isOpened():
    ret, frame = cap.read()
    if ret is True:
        # 图像处理段
        # # 1. 饱和度增强
        hsv = cv2.cvtColor(frame, cv2.COLOR_RGB2HSV)  # 色彩空间转换, RGB->HSV

        cv2.imshow('hsv', hsv)
        blendSRaisen = cv2.LUT(hsv, lutSRaisen)             # 饱和度增大
        # 2. 掩膜生成
        h_min = cv2.getTrackbarPos("Hue Min", "TrackBars")
        h_max = cv2.getTrackbarPos("Hue Max", "TrackBars")
        s_min = cv2.getTrackbarPos("Sat Min", "TrackBars")
        s_max = cv2.getTrackbarPos("Sat Max", "TrackBars")
        v_min = cv2.getTrackbarPos("Val Min", "TrackBars")
        v_max = cv2.getTrackbarPos("Val Max", "TrackBars")
        lower = np.array([h_min, s_min, v_min])
        upper = np.array([h_max, s_max, v_max])
        mask = cv2.inRange(blendSRaisen, lower, upper)
        cv2.imshow("Mask",mask)
        imgResult = cv2.bitwise_and(frame, frame, mask=mask)
        #cv2.imshow('result', imgResult)
        # cv2.imshow('r',blendSRaisen)
        key = cv2.waitKey(1)
        if key == 27:
            break
    else:
        break

cv2.destroyAllWindows()
cap.release()
