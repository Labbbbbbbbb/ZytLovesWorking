# ==================================================
# 一、导入区：库的导入及头文件申明
# ==================================================
import time
import os
import sys
from media.sensor import *
from media.display import *
from media.media import *

# ==================================================
# 二、配置区：所有可调参数都在这里
# ==================================================
DISPLAY_MODE = "LCD"
SENSOR_ID = 2






# ==================================================
# 三、程序主体
# ==================================================
# ---------- 1、全局运行状态，保存关键信息和控制流程 ---------
"""在此放置全局变量，其实也能放在配置区，放这里好找"""






# ----------------- 2、工具函数 ------------------------
"""必要的处理工具如9宫格分区"""




# -------------- 3、主循环级函数 ------------------------
"""比较重要的函数放这里"""





# ------------------ 4、主流程 -------------------------



sensor = None

try:
    print("程序执行开始...")

    # --- 初始化摄像头 ---
    sensor = Sensor(id=SENSOR_ID)
    sensor.reset()

    # --- 初始化显示设备 ---
    if DISPLAY_MODE == "LCD":
        DISPLAY_WIDTH, DISPLAY_HEIGHT = 800, 480
        Display.init(Display.ST7701, width=DISPLAY_WIDTH, height=DISPLAY_HEIGHT, to_ide=True)

    # 设置摄像头
    sensor.set_framesize(width=DISPLAY_WIDTH, height=DISPLAY_HEIGHT, chn=CAM_CHN_ID_0)
    sensor.set_pixformat(Sensor.RGB565, chn=CAM_CHN_ID_0)

    # --- 初始化媒体管理器和时钟 ---
    MediaManager.init()
    sensor.run()
    clock = time.clock()

    print("初始化完成，进入主循环...")
    while True:
        clock.tick()
        os.exitpoint()

        # 获取图像帧
        img = sensor.snapshot(chn=CAM_CHN_ID_0)




        """程序的核心部分放在这里"""






        # 显示FPS
        img.draw_string_advanced(10, 10, 30, f"fps: {clock.fps():.2f}", color=(255, 255, 0))
        Display.show_image(img)

except KeyboardInterrupt as e:
    # 捕获用户中断异常 (例如，通过Ctrl+C)
    print(f"用户停止: {e}")
except BaseException as e:
    # 捕获所有其他运行时异常
    print(f"程序出现异常: {e}")
finally:
    print("释放资源...")
    if sensor:
        sensor.stop()
    Display.deinit()
    MediaManager.deinit()
    print("程序已退出")
