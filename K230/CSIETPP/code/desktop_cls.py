from ultralytics import YOLO
# 加载已训练的YOLO11模型
model = YOLO("F:\\ITS_Log\\K230\\CSIETPP\\yolo11\\runs\\classify\\train\\weights\\best.pt")
# 对单张图片进行预测
results = model.predict(source="F:\\ITS_Log\\K230\\CSIETPP\\OIP.jpg", save=True, conf=0.5)
# 显示预测结果
results[0].show()