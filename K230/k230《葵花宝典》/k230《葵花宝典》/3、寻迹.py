import time
import os
import sys
from media.sensor import *
from media.display import *
from media.media import *
from time import ticks_ms

# ------------------- 配置部分 -------------------
DISPLAY_MODE = "LCD"
SENSOR_ID = 2

# 三个ROI区域的巡线参数配置
ROI_CONFIGS = [
    {  # 顶部区域
        'threshold': (20, 70),
        'min_area': 2500,
        'stability_frames': 5,
        'color': (255, 0, 0),  # 红色
        'weight': 0.2         # 权重值，范围0~1
    },
    {  # 中间区域
        'threshold': (15, 65),
        'min_area': 2000,
        'stability_frames': 4,
        'color': (0, 255, 0),  # 绿色
        'weight': 0.3         # 权重值，范围0~1
    },
    {  # 底部区域
        'threshold': (33, 69),
        'min_area': 3000,
        'stability_frames': 6,
        'color': (0, 0, 255),  # 蓝色
        'weight': 0.5         # 权重值，范围0~1
    }
]

ROI_HEIGHT = 140
ROI_WIDTH = 650
STABILIZATION_TIME = 1.0           # 系统稳定等待时间(秒)
# 左侧坐标显示位置（x固定在左侧，y依次排列）
COORD_DISPLAY_POS = [(10, 80), (10, 150), (10, 220)]  # ROI1、ROI2、ROI3的显示位置
# 权重点显示位置
WEIGHTED_POS = (10, 300)          # 权重点坐标显示位置
# 差值显示位置
DIFF_DISPLAY_POS = (10, 370)      # 差值显示位置

# ------------------- 程序主体 -------------------
sensor = None

try:
    print("开始执行黑色线条检测程序...")

    # --- 1. 初始化摄像头 ---
    sensor = Sensor(id=SENSOR_ID)
    sensor.reset()

    # --- 2. 初始化显示设备 ---
    if DISPLAY_MODE == "LCD":
        DISPLAY_WIDTH, DISPLAY_HEIGHT = 800, 480
        Display.init(Display.ST7701, width=DISPLAY_WIDTH, height=DISPLAY_HEIGHT, to_ide=True)

    # 设置摄像头
    sensor.set_framesize(width=DISPLAY_WIDTH, height=DISPLAY_HEIGHT, chn=CAM_CHN_ID_0)
    sensor.set_pixformat(Sensor.RGB565, chn=CAM_CHN_ID_0)

    # --- 3. 初始化媒体管理器和时钟 ---
    MediaManager.init()
    sensor.run()
    clock = time.clock()

    # 定义3个检测区域
    ROIS = [
        (150, 20, ROI_WIDTH, ROI_HEIGHT),    # 顶部区域
        (150, 170, ROI_WIDTH, ROI_HEIGHT),   # 中间区域
        (150, 320, ROI_WIDTH, ROI_HEIGHT)    # 底部区域
    ]

    # 计算各ROI自身的初始中心坐标（初始显示用）
    initial_coords = []
    for roi in ROIS:
        roi_x, roi_y, roi_w, roi_h = roi
        roi_center_x = roi_x + roi_w // 2
        roi_center_y = roi_y + roi_h // 2
        initial_coords.append((roi_center_x, roi_center_y))

    # 初始化变量：存储上一次的坐标（初始为各ROI自身中心）
    last_coordinates = initial_coords.copy()

    # 稳定性检测变量
    stabilization_start = ticks_ms()
    detection_history = [[] for _ in range(len(ROIS))]  # 存储各区域历史检测结果
    stable_detections = [None] * len(ROIS)              # 稳定检测结果

    print("等待系统稳定...")
    while ticks_ms() - stabilization_start < STABILIZATION_TIME * 1000:
        img = sensor.snapshot(chn=CAM_CHN_ID_0)
        Display.show_image(img)
        time.sleep_ms(50)

    print("初始化完成，进入主循环...")
    while True:
        clock.tick()
        os.exitpoint()

        img = sensor.snapshot(chn=CAM_CHN_ID_0)

        # 绘制ROI区域和标题
        for i, roi in enumerate(ROIS):
            config = ROI_CONFIGS[i]
            img.draw_rectangle(*roi, color=config['color'], thickness=2)
            img.draw_string_advanced(roi[0]+10, roi[1]+10, 20,
                                   f"ROI {i+1} (T:{config['threshold']})",
                                   color=config['color'])

        # 处理每个ROI区域，更新坐标
        for i, (roi, config) in enumerate(zip(ROIS, ROI_CONFIGS)):
            roi_img = img.copy(roi=roi)
            roi_gray = roi_img.to_grayscale()
            roi_bin = roi_gray.binary([config['threshold']])

            # 查找黑色区域
            blobs = roi_bin.find_blobs([(100, 255)], merge=True, margin=10)

            current_detection = None
            if blobs:
                # 筛选面积足够大的区域
                valid_blobs = [b for b in blobs if b.pixels() > config['min_area']]

                if valid_blobs:
                    largest_blob = max(valid_blobs, key=lambda b: b.pixels())
                    current_detection = largest_blob

            # 稳定性检查
            if current_detection:
                detection_history[i].append(current_detection)
                if len(detection_history[i]) > config['stability_frames']:
                    detection_history[i].pop(0)

                # 连续多帧稳定检测到，更新坐标
                if len(detection_history[i]) >= config['stability_frames']:
                    # 计算平均位置
                    avg_x = sum(b.cx() for b in detection_history[i]) // len(detection_history[i])
                    avg_y = sum(b.cy() for b in detection_history[i]) // len(detection_history[i])

                    # 转换为全局坐标
                    gx = avg_x + roi[0]
                    gy = avg_y + roi[1]

                    stable_detections[i] = (gx, gy)
                    # 更新上一次坐标为当前坐标
                    last_coordinates[i] = (gx, gy)
            else:
                # 未检测到，保留上一次坐标
                detection_history[i] = []
                stable_detections[i] = None

            # 绘制稳定检测结果（白色矩形和十字）
            if stable_detections[i]:
                gx, gy = stable_detections[i]
                # 计算矩形大小（基于历史平均大小）
                if detection_history[i]:
                    avg_w = sum(b.w() for b in detection_history[i]) // len(detection_history[i])
                    avg_h = sum(b.h() for b in detection_history[i]) // len(detection_history[i])
                else:
                    avg_w, avg_h = 50, 50  # 默认大小
                img.draw_rectangle(gx - avg_w//2, gy - avg_h//2, avg_w, avg_h, color=(255,255,255), thickness=3)
                img.draw_cross(gx, gy, color=(255,255,255), size=20, thickness=3)

        # 计算权重点坐标
        total_weight = 0.0
        weighted_x = 0.0
        weighted_y = 0.0

        for i in range(len(ROIS)):
            if stable_detections[i]:  # 如果该ROI检测到目标
                config = ROI_CONFIGS[i]
                x, y = stable_detections[i]
                weight = config['weight']

                weighted_x += x * weight
                weighted_y += y * weight
                total_weight += weight

        # 计算最终权重点（如果有检测到的区域）
        weighted_point = None
        if total_weight > 0:
            final_x = int(weighted_x / total_weight)
            final_y = int(weighted_y / total_weight)
            weighted_point = (final_x, final_y)

            # 绘制权重点（红色十字）
            img.draw_cross(final_x, final_y, color=(255, 0, 0), size=30, thickness=3)
            img.draw_string_advanced(final_x+20, final_y+20, 20, "权重点", color=(255, 0, 0))

        # 计算红色十字与底部ROI x坐标的差值
        x_diff = None
        if weighted_point and last_coordinates[2]:  # 确保两个坐标都有效
            # 权重点x坐标减去底部ROI x坐标
            x_diff = weighted_point[0] - last_coordinates[2][0]

        # 在左侧显示坐标信息
        for i in range(len(ROIS)):
            config = ROI_CONFIGS[i]
            display_x, display_y = COORD_DISPLAY_POS[i]
            # 显示标题
            img.draw_string_advanced(display_x, display_y - 30, 20,
                                   f"ROI {i+1} 坐标:",
                                   color=config['color'])
            # 显示坐标值
            coord_x, coord_y = last_coordinates[i]
            img.draw_string_advanced(display_x, display_y, 22,
                                   f"({coord_x}, {coord_y})",
                                   color=config['color'])

        # 显示权重点坐标
        display_x, display_y = WEIGHTED_POS
        img.draw_string_advanced(display_x, display_y - 30, 20, "权重点坐标:", color=(255, 0, 0))
        if weighted_point:
            wx, wy = weighted_point
            img.draw_string_advanced(display_x, display_y, 22, f"({wx}, {wy})", color=(255, 0, 0))
        else:
            img.draw_string_advanced(display_x, display_y, 22, "未计算", color=(255, 0, 0))

        # 显示x坐标差值（红色十字与底部ROI）
        diff_x, diff_y = DIFF_DISPLAY_POS
        img.draw_string_advanced(diff_x, diff_y - 30, 20, "x坐标差值:", color=(255, 0, 255))  # 紫色显示
        if x_diff is not None:
            img.draw_string_advanced(diff_x, diff_y, 22, f"{x_diff}", color=(255, 0, 255))
        else:
            img.draw_string_advanced(diff_x, diff_y, 22, "无法计算", color=(255, 0, 255))

        # 显示FPS
        img.draw_string_advanced(10, 10, 30, f"fps: {clock.fps():.2f}", color=(255,255,0))
        Display.show_image(img)

except KeyboardInterrupt:
    print("用户停止程序")
finally:
    print("释放资源...")
    if sensor: sensor.stop()
    Display.deinit()
    MediaManager.deinit()
    print("程序已退出")
