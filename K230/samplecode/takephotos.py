
'''
Readme:take photo with JLC K230
    use gpio47(the 12th pin) as the button to capture photo
    and save it to CanMV/data/

    the current resolution is 640*480
    缺点：目前没有做按键消抖处理，摁下开关时会连拍很多张
'''


import time, os, sys
from machine import Pin,FPIOA
from media.sensor import *
from media.display import *
from media.media import *
import math

SAVE_FOLDER = "/data"  # 使用原始字符串避免转义问题
IMAGE_PREFIX = "capture"
IMAGE_FORMAT = ".jpg"

fpioa = FPIOA()
fpioa.set_function(47,FPIOA.GPIO47)     #引脚号12
button = Pin(47, Pin.IN, pull=Pin.PULL_UP)  #默认为低电平，按下按钮为高电平


def save_current_image(img, count):

    filename = f"{IMAGE_PREFIX}_{count}{IMAGE_FORMAT}"
    img.save(SAVE_FOLDER + "/" + filename, quality=85)  # quality可选50-95
    print(f"JPEG saved: {filename}")

sensor_id = 2
sensor = None

picture_width = 640
picture_height = 480


# 显示模式选择：可以是 "VIRT"、"LCD" 或 "HDMI"
DISPLAY_MODE = "VIRT"

# 根据模式设置显示宽高
if DISPLAY_MODE == "VIRT":
    # 虚拟显示器模式
    DISPLAY_WIDTH = picture_width
    DISPLAY_HEIGHT = picture_height
elif DISPLAY_MODE == "LCD":
    # 3.1寸屏幕模式
    DISPLAY_WIDTH = 800
    DISPLAY_HEIGHT = 480
elif DISPLAY_MODE == "HDMI":
    # HDMI扩展板模式
    DISPLAY_WIDTH = 1920
    DISPLAY_HEIGHT = 1080
else:
    raise ValueError("未知的 DISPLAY_MODE，请选择 'VIRT', 'LCD' 或 'HDMI'")

try:
    # 构造一个具有默认配置的摄像头对象
    sensor = Sensor(id=sensor_id,width=640, height=480)
    # 重置摄像头sensor
    sensor.reset()

    # 无需进行镜像和翻转
    # 设置不要水平镜像
    sensor.set_hmirror(False)
    # 设置不要垂直翻转
    sensor.set_vflip(False)

    sensor.set_framesize(width=picture_width, height=picture_height, chn=CAM_CHN_ID_0)
    # 设置通道0的输出像素格式为RGB565，要注意有些案例只支持GRAYSCALE格式
    sensor.set_pixformat(Sensor.RGB565, chn=CAM_CHN_ID_0)

    # 根据模式初始化显示器
    if DISPLAY_MODE == "VIRT":
        Display.init(Display.VIRT, width=DISPLAY_WIDTH, height=DISPLAY_HEIGHT, fps=60)
    elif DISPLAY_MODE == "LCD":
        Display.init(Display.ST7701, width=DISPLAY_WIDTH, height=DISPLAY_HEIGHT, to_ide=True)
    elif DISPLAY_MODE == "HDMI":
        Display.init(Display.LT9611, width=DISPLAY_WIDTH, height=DISPLAY_HEIGHT, to_ide=True)

    # 初始化媒体管理器
    MediaManager.init()
    # 启动传感器
    sensor.run()

    fps = time.clock()

    capture_count = 0

    while True:
        fps.tick()
        os.exitpoint()

        # 捕获通道0的图像
        img = sensor.snapshot(chn=CAM_CHN_ID_0)
        Display.show_image(img,x=0,y=0,layer = Display.LAYER_OSD0)

        # 检测gpio输入
        if button.value()==0:
            save_current_image(img, capture_count)
            capture_count += 1


        # 打印帧率到控制台
        print(fps.fps())
        print(button.value())

except KeyboardInterrupt as e:
    print("用户停止: ", e)
except BaseException as e:
    print(f"异常: {e}")
finally:
    # 停止传感器运行
    if isinstance(sensor, Sensor):
        sensor.stop()
    # 反初始化显示模块
    Display.deinit()
    os.exitpoint(os.EXITPOINT_ENABLE_SLEEP)
    time.sleep_ms(100)
    # 释放媒体缓冲区
    MediaManager.deinit()
