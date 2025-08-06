import cv2
import sys
sys.path.insert(0, "F:/ITS_Log/MachineLearning/Yolo/yv5_env")
from yolov5 import YOLO
# 加载模型
model = YOLO('F:/ITS_Log/MachineLearning/Yolo/yv5_env/yolov5/runs/train/exp16/weights/best.pt')
# 初始化视频流
cap = cv2.VideoCapture(0)
if not cap.isOpened():
    raise ValueError("无法打开视频流！")
print("视频流已连接...")
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

# 循环播放视频帧
while cap.isOpened():
    # 从视频中读取一帧
    success, frame = cap.read()
    if success:
        # 在框架上运行 YOLOv8 推理
        results = model(frame)
        # 在框架上可视化结果
        annotated_frame = results[0].plot()
        # 显示带标注的框架
        cv2.imshow('title', annotated_frame)
        # 如果按下“q”，则中断循环
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break
    else:
        # 如果到达视频末尾，则中断循环
        break

# 释放视频捕获对象并关闭显示窗口
cap.release()
cv2.destroyAllWindows()


#原文链接：https: // blog.csdn.net / weixin_42607526 / article / details / 140552888