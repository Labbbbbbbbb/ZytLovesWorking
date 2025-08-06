#在基础版上尝试新的识别算法
import time, os, sys

from media.sensor import *
from media.display import *
from media.media import *
import math

sensor_id = 2
sensor = None

picture_width = 640
picture_height = 480




#--------定义自己的变量START--------
threshold_red = [(45, 70, 10, 105, -78, -52)]

#--------定义自己的变量END--------

#--------定义自己的函数START--------
def red_blob(img, threshold_red,ROI=(0,0,picture_width,picture_height)):

    #识别红色激光光斑
    blobs = img.find_blobs(threshold_red, roi=ROI, x_stride=2, y_stride=2, area_threshold=0, pixels_threshold=0,merge=True,margin=10)
    if len(blobs)>=0.1 :#有色块
        # 选择像素最多的色块
        largest_blob = max(blobs, key=lambda b: b.pixels())
        # 获取中心坐标
        cx = largest_blob.cx()
        cy = largest_blob.cy()
        # 在图像上画圆标记光斑
        img.draw_circle(cx, cy, 3, color=(0, 255, 0))

        return cx, cy
    return 0, 0 #表示没有找到

#--------定义自己的函数END--------


# 显示模式选择：可以是 "VIRT"、"LCD" 或 "HDMI"
DISPLAY_MODE = "VIRT"

# 根据模式设置显示宽高
if DISPLAY_MODE == "VIRT":
    # 虚拟显示器模式
    DISPLAY_WIDTH = picture_width*2
    DISPLAY_HEIGHT = picture_height*2
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

    while True:
        fps.tick()
        os.exitpoint()

        # 捕获通道0的图像
        img = sensor.snapshot(chn=CAM_CHN_ID_0)

        # 在屏幕右上角显示原始图像
        Display.show_image(img,x=0,y=picture_height,layer = Display.LAYER_OSD0)

        # 图像处理放到这里
        #--------开始--------
        x, y = red_blob(img, threshold_red)
        print(x,y)
        #--------结束--------

        # 在屏幕左下角显示处理后的图像
        Display.show_image(img,x=DISPLAY_WIDTH-picture_width,y=0,layer = Display.LAYER_OSD1)

        # 打印帧率到控制台
        print(fps.fps())

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
