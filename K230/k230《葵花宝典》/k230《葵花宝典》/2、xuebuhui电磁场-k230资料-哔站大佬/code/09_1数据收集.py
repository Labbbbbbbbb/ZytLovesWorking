import time
import os
from media.sensor import *
from media.display import *
from media.media import *

# 优化参数配置
PICTURE_WIDTH = 320   # QVGA分辨率，平衡清晰度与性能
PICTURE_HEIGHT = 240
SENSOR_ID = 2
DISPLAY_WIDTH = 800
DISPLAY_HEIGHT = 480

# 矩形检测优化参数
DETECT_THRESHOLD = 3000  # 检测阈值（降低以提高灵敏度）
AREA_THRESHOLD = 200     # 最小面积过滤
ROI = (50, 50, 220, 140)  # 中心区域检测（减少计算量）
DISPLAY_CENTER = (DISPLAY_WIDTH - PICTURE_WIDTH) // 2, (DISPLAY_HEIGHT - PICTURE_HEIGHT) // 2

try:
    print("系统初始化中...")

    # 初始化传感器
    sensor = Sensor(id=SENSOR_ID)
    sensor.reset()
    sensor.set_framesize(width=PICTURE_WIDTH, height=PICTURE_HEIGHT, chn=CAM_CHN_ID_0)
    sensor.set_pixformat(Sensor.RGB565, chn=CAM_CHN_ID_0)
    sensor.set_fps(30)  # 限制帧率以优化性能

    # 初始化显示
    Display.init(Display.ST7701, width=DISPLAY_WIDTH, height=DISPLAY_HEIGHT, to_ide=True)

    # 初始化媒体管理器
    MediaManager.init()
    sensor.run()
    time.sleep_ms(200)  # 等待传感器稳定

    print("优化后的矩形检测系统已就绪")

    frame_count = 0
    start_time = time.ticks_ms()

    while True:
        os.exitpoint()

        # 捕获图像并转换为灰度图（减少计算量）
        img = sensor.snapshot(chn=CAM_CHN_ID_0).to_grayscale()

        # 检测矩形（带ROI和面积过滤）
        rects = img.find_rects(
            threshold=DETECT_THRESHOLD,
            area_threshold=AREA_THRESHOLD,
            roi=ROI,
            merge=True
        )

        # 处理检测结果（仅绘制最大矩形）
        if rects:
            # 按面积排序，取最大矩形
            largest_rect = max(rects, key=lambda r: r.w() * r.h())
            x, y, w, h = largest_rect.rect()

            # 绘制矩形和中心标记
            img.draw_rectangle((x, y, w, h), color=(255, 0, 0), thickness=2)
            img.draw_cross(x + w//2, y + h//2, color=(0, 255, 0), size=5)

            # 每10帧输出一次信息
            frame_count += 1
            if frame_count % 10 == 0:
                # 计算帧率
                current_time = time.ticks_ms()
                fps = 10000 / (current_time - start_time)
                start_time = current_time

                print(f"[帧{frame_count}] 检测到矩形: 面积={w*h}, 位置=({x},{y}), 帧率={fps:.1f} FPS")

        # 显示图像（使用预计算的居中位置）
        Display.show_image(img, x=DISPLAY_CENTER[0], y=DISPLAY_CENTER[1])

except KeyboardInterrupt:
    print("程序被用户中断")
except Exception as e:
    print(f"异常发生: {e}")
finally:
    # 释放资源
    if sensor:
        sensor.stop()
    Display.deinit()
    MediaManager.deinit()
    print("系统资源已释放")
