import cv2  # OpenCV库，用于图像处理
import numpy as np  # NumPy库，用于数值计算
import time  # 时间库
import struct
import serial
import serial.tools.list_ports
serial_port = serial.Serial("/dev/ttyAMA0", 115200, timeout=0.5)

# 摄像头运动补偿相关变量
prev_gray = None
orb = cv2.ORB_create(nfeatures=500)  # ORB特征检测器
matcher = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)  # 特征匹配器
camera_motion_threshold = 10  # 摄像头运动检测阈值

# 初始化摄像头 - 循环尝试不同设备号
cap = None
while cap is None or not cap.isOpened():
    for device_id in [0, 1, 2]:
        print(f"尝试打开摄像头设备 {device_id}...")
        cap = cv2.VideoCapture(device_id)
        if cap.isOpened():
            print(f"成功打开摄像头设备 {device_id}")
            break
        else:
            print(f"摄像头设备 {device_id} 打开失败")
            cap.release()

if cap is None or not cap.isOpened():
    print("错误：无法打开任何摄像头设备 (0, 1, 2)")
    exit(1)

cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)   # 设置图像宽度
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)  # 设置图像高度
cap.set(cv2.CAP_PROP_BRIGHTNESS, 0.2)    # 设置亮度，范围0-1，降低亮度
cap.set(cv2.CAP_PROP_AUTO_WB, 0)         # 关闭自动白平衡
cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0)   # 关闭自动曝光
cap.set(cv2.CAP_PROP_EXPOSURE, -8)       # 调小曝光值

while cap.isOpened():
    # 读取摄像头帧
    ret, frame = cap.read()
    if not ret:
        break
    
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # 摄像头运动估计和补偿
    camera_motion_detected = False
    stable_frame = gray.copy()
    
    if prev_gray is not None:
        # 使用ORB特征点检测摄像头运动
        kp1, des1 = orb.detectAndCompute(prev_gray, None)
        kp2, des2 = orb.detectAndCompute(gray, None)
        
        if des1 is not None and des2 is not None and len(des1) > 10 and len(des2) > 10:
            # 特征匹配
            matches = matcher.match(des1, des2)
            matches = sorted(matches, key=lambda x: x.distance)
            
            if len(matches) > 20:  # 有足够的匹配点
                # 提取匹配点坐标
                src_pts = np.float32([kp1[m.queryIdx].pt for m in matches[:20]]).reshape(-1, 1, 2)
                dst_pts = np.float32([kp2[m.trainIdx].pt for m in matches[:20]]).reshape(-1, 1, 2)
                
                # 计算单应性矩阵（摄像头运动）
                H, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)
                
                if H is not None:
                    # 检测是否为显著的摄像头运动（主要是平移和旋转）
                    motion_magnitude = np.linalg.norm(H - np.eye(3))
                    
                    if motion_magnitude > 0.1:  # 摄像头确实在运动
                        camera_motion_detected = True
                        print(f"摄像头运动检测: 运动幅度={motion_magnitude:.3f}")
                        
                        # 进行运动补偿 - 将当前帧对齐到前一帧
                        try:
                            H_inv = np.linalg.inv(H)
                            stable_frame = cv2.warpPerspective(gray, H_inv, (gray.shape[1], gray.shape[0]))
                        except:
                            stable_frame = gray.copy()
    
    # 更新前一帧
    prev_gray = gray.copy()
    
    # 在稳定化后的图像中检测固定目标
    # 使用自适应阈值处理（与CV4一致）
    thresh = cv2.adaptiveThreshold(
        stable_frame, 255, 
        cv2.ADAPTIVE_THRESH_GAUSSIAN_C, 
        cv2.THRESH_BINARY_INV, 11, 2
    )
    # 去噪处理
    denoised = cv2.medianBlur(thresh, 5)
    # 形态学操作 - 先开操作去小噪点，再闭操作连接间隙
    kernel_small = np.ones((3, 3), np.uint8)
    opened = cv2.morphologyEx(denoised, cv2.MORPH_OPEN, kernel_small)
    kernel_close = np.ones((7, 7), np.uint8)
    closed = cv2.morphologyEx(opened, cv2.MORPH_CLOSE, kernel_close)
    
    # 连通域分析，过滤小区域（与CV4一致）
    num_labels, labels, stats, centroids = cv2.connectedComponentsWithStats(closed, connectivity=8)
    
    cleaned = np.zeros_like(closed)
    min_area = 220  # 候选矩形最小面积
    
    for i in range(1, num_labels):
        if stats[i, cv2.CC_STAT_AREA] >= min_area:
            cleaned[labels == i] = 255
    
    # 边缘检测（与CV4一致）
    edges = cv2.Canny(cleaned, 50, 150)
    
    # 查找轮廓（与CV4一致）
    contours, hierarchy = cv2.findContours(edges, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    
    # 候选矩形列表
    valid_targets = []
    
    for i, cnt in enumerate(contours):
        # 轮廓近似
        epsilon = 0.02 * cv2.arcLength(cnt, True)
        approx = cv2.approxPolyDP(cnt, epsilon, True)
        
        # 检查是否为四边形
        if len(approx) == 4:
            area = cv2.contourArea(approx)
            
            # 面积过滤 - 与CV4一致
            if area < 4000 or area > 80000:  
                continue
                
            # 长宽比检测
            rect = cv2.minAreaRect(approx)
            w, h = rect[1]
            if w < h:
                w, h = h, w  # 确保w是长边
            
            aspect_ratio = w / h
            
            # 矩形长宽比过滤（与CV4一致）
            if not (1.1 <= aspect_ratio <= 2.1):
                continue
                
            # 计算轮廓的凸性
            hull = cv2.convexHull(approx)
            hull_area = cv2.contourArea(hull)
            solidity = area / hull_area if hull_area > 0 else 0
            
            # 矩形度检测（与CV4一致）
            if solidity < 0.85:
                continue
                
            # 计算中心坐标
            points = approx.reshape(-1, 2)
            center_x = np.mean(points[:, 0])
            center_y = np.mean(points[:, 1])
            
            # 检查是否为黑色边框（检查轮廓内部是否主要为白色）
            mask = np.zeros(stable_frame.shape, dtype=np.uint8)
            cv2.fillPoly(mask, [approx], 255)
            
            if cv2.countNonZero(mask) > 0:
                inner_mean = cv2.mean(stable_frame, mask=mask)[0]
                
                # 内部应该比较亮（白色背景） - 与CV4一致
                if inner_mean > 120:  # 内部较亮，说明是黑色边框
                    # 评分系统（与CV4一致）
                    score = 0
                    
                    # 矩形特征评分（主要）
                    if 1.2 <= aspect_ratio <= 2.0:  # 理想长宽比
                        score += 20
                    elif 1.1 <= aspect_ratio <= 2.3:  # 可接受长宽比
                        score += 10
                        
                    if solidity > 0.9:  # 高矩形度
                        score += 25
                    elif solidity > 0.85:  # 中等矩形度
                        score += 15
                        
                    if area > 7000:  # 面积加分
                        score += 20
                    elif area > 5000:
                        score += 10
                    
                    # 内部颜色符合要求加分
                    score += 15
                    
                    valid_targets.append({
                        'contour': approx,
                        'area': area,
                        'aspect_ratio': aspect_ratio,
                        'solidity': solidity,
                        'center': (center_x, center_y),
                        'score': score,
                        'inner_brightness': inner_mean
                    })
    
    # 按评分排序，取最佳目标
    valid_targets.sort(key=lambda x: x['score'], reverse=True)
    
    
    # 绘制检测结果
    for i, target in enumerate(valid_targets[:3]):  # 最多显示3个最佳目标
        color = [(0, 255, 0), (255, 0, 0), (0, 0, 255)][i]  # 绿、红、蓝
        
        # 绘制外轮廓
        cv2.drawContours(frame, [target['contour']], -1, color, 3)
        
        # 绘制中心点和信息
        center = tuple(map(int, target['center']))
        cv2.circle(frame, center, 8, color, -1)
        
        # 显示评分和信息
        text = f"Score:{target['score']:.0f} AR:{target['aspect_ratio']:.1f}"
        cv2.putText(frame, text, (center[0]-50, center[1]-25), 
                   cv2.FONT_HERSHEY_SIMPLEX, 0.6, color, 2)
    
    # 显示摄像头运动状态
    motion_text = "Camera Moving" if camera_motion_detected else "Camera Stable"
    cv2.putText(frame, motion_text, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, 
               (0, 0, 255) if camera_motion_detected else (0, 255, 0), 2)
    
    # 显示处理结果
    cv2.imshow('Stabilized', stable_frame)
    cv2.imshow('thresh', thresh)
    cv2.imshow('cleaned', cleaned)
    cv2.imshow('edges', edges)
    cv2.imshow('Detection', frame)
    
    if valid_targets:
        best_target = valid_targets[0]
        center_x, center_y = best_target['center']
        print(f"检测到固定目标: ({center_x:.1f}, {center_y:.1f}), 得分: {best_target['score']:.0f}")
        
        # 限制坐标范围
        center_x = max(0, min(639, center_x))
        center_y = max(0, min(479, center_y))
        
        xh = int(center_x) >> 8  # x高位
        xl = int(center_x) & 0xFF  # x低位
        yh = int(center_y) >> 8  # y高位
        yl = int(center_y) & 0xFF  # y低位
    else:
        xh = 0
        xl = 0
        yh = 0
        yl = 0
        
    pack_data = struct.pack(
        ">BB4BBB",
        0xAA,
        0xBB,
        0xCC,
        0xDD,
        xh,
        xl,
        yh,     
        yl
    )
    serial_port.write(pack_data)
    print(f"发送数据: {xh}, {xl}, {yh}, {yl}")
    
    key = cv2.waitKey(1)
    if key == 32 or key == 27:  # 空格或ESC退出
        break

# 释放资源
cap.release()
cv2.destroyAllWindows()
