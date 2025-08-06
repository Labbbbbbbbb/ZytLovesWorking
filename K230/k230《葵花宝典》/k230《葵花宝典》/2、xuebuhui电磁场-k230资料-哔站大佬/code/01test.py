import time
import os
import sys

from media.sensor import *
from media.display import *
from media.media import *

sensor = None
display = None

try:
    print("camera_test - 初始化中...")

    # 构造传感器对象
    sensor = Sensor()
    sensor.reset()
    time.sleep_ms(300)  # 增加重置等待时间

    # 设置传感器参数
    sensor.set_framesize(Sensor.FHD)  # 1920x1080
    sensor.set_pixformat(Sensor.RGB565)

    # 初始化显示模块
    print("初始化显示模块...")
    display = Display.init(Display.LT9611, to_ide=True)

    # 初始化媒体管理器
    print("初始化媒体管理器...")
    MediaManager.init()

    # 启动传感器
    print("启动传感器...")
    sensor.run()
    time.sleep_ms(500)  # 增加启动等待时间

    # 检查传感器是否成功启动
    if hasattr(sensor, 'is_running') and sensor.is_running():
        print("传感器启动成功")
    else:
        print("警告: 传感器启动失败")

    print("初始化完成，开始捕获图像...")

    while True:
        os.exitpoint()

        # 捕获图像
        print("尝试捕获图像...")
        img = sensor.snapshot(chn=CAM_CHN_ID_0)

        if img:
            print("图像捕获成功")
            # 显示图像
            Display.show_image(img)
        else:
            print("警告: 图像捕获失败")

        time.sleep_ms(50)  # 增加循环间隔

except KeyboardInterrupt:
    print("用户通过键盘中断程序")
except Exception as e:
    print(f"发生异常: {e}")
    # 手动打印异常信息（替代traceback）
    import sys
    print(f"异常类型: {type(e).__name__}")
    print(f"异常信息: {e}")
    print(f"异常发生位置: line {sys.exc_info()[2].tb_lineno}")
finally:
    print("释放系统资源...")

    # 停止传感器
    if sensor:
        try:
            sensor.stop()
            print("传感器已停止")
        except Exception as e:
            print(f"停止传感器时出错: {e}")

    # 释放显示资源
    if display:
        try:
            Display.deinit()
            print("显示模块已释放")
        except Exception as e:
            print(f"释放显示模块时出错: {e}")

    # 释放媒体管理器
    try:
        MediaManager.deinit()
        print("媒体管理器已释放")
    except Exception as e:
        print(f"释放媒体管理器时出错: {e}")

    os.exitpoint(os.EXITPOINT_ENABLE_SLEEP)
    time.sleep_ms(300)  # 增加资源释放等待时间

    print("系统资源已成功释放")
