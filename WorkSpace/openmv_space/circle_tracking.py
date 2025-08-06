# 圆形检测例程
#
# 这个例子展示了如何用Hough变换在图像中找到圆。
# https://en.wikipedia.org/wiki/Circle_Hough_Transform
#
# 请注意，find_circles（）方法将只能找到完全在图像内部的圆。圈子之外的
# 图像/ roi被忽略...


# Circle对象有四个值: x, y, r (半径), 和 magnitude。
# magnitude是检测圆的强度。越高越好

# roi 是一个用以复制的矩形的感兴趣区域(x, y, w, h)。如果未指定，
# ROI 即图像矩形。操作范围仅限于roi区域内的像素。

# x_stride 是霍夫变换时需要跳过的x像素的数量。若已知圆较大，可增加
# x_stride 。

# y_stride 是霍夫变换时需要跳过的y像素的数量。若已知直线较大，可增加
# y_stride 。

# threshold 控制从霍夫变换中监测到的圆。只返回大于或等于阈值的圆。
# 应用程序的阈值正确值取决于图像。注意：一条圆的大小是组成圆所有
# 索贝尔滤波像素大小的总和。

# x_margin 控制所检测的圆的合并。 圆像素为 x_margin 、 y_margin 和
# r_margin的部分合并。

# y_margin 控制所检测的圆的合并。 圆像素为 x_margin 、 y_margin 和
# r_margin 的部分合并。

# r_margin 控制所检测的圆的合并。 圆像素为 x_margin 、 y_margin 和
# r_margin 的部分合并。

# r_min，r_max和r_step控制测试圆的半径。
# 缩小测试圆半径的数量可以大大提升性能。
# threshold = 3500比较合适。如果视野中检测到的圆过多，请增大阈值；
# 相反，如果视野中检测到的圆过少，请减少阈值。


import sensor, image, time
import struct
from machine import UART
import pyb

red_threshold  = (13, 49, 18, 61, 6, 47)
blue_threshold  = (21, 100, -14, 112, -98, -13)
black_threshold  = (0, 12, -79, 14, -26, 39)
yellow_threshold  = (48, 100, -60, 0, 17, 126)
sensor.reset()
sensor.set_pixformat(sensor.RGB565) # 灰度更快
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 500)
clock = time.clock()

def find_max_circle(circles):   ##有则返回圆，无则返回0
    max_size=0
    if circles:
        max_circle=circles[0]
        for circle in circles:
            if circle.r() > max_size:
                max_circle=circle
                max_size = circle.r()
        return max_circle
    else:
        return 0

uart1 = UART(1, 115200)

def SendData(color,cx,cy):#
    data=struct.pack(">bbbhhbb",
                     0xFF,
                     0xFE,
                     int(color),
                     int(cx),
                     int(cy),
                     0xFE,
                     0xFF
                     )
    uart1.write(data)
uart3 = UART(3, 115200)
uart_flag=1
def Uart_callback(line):
    data=uart3.read()
    uart_flag=data


def find_max_red_circles():     #有则返回圆，无则返回0
    red_circles=[]
    for c in img.find_circles(threshold = 2000, x_margin = 10, y_margin = 10, r_margin = 10,r_min = 2, r_max = 15, r_step = 2):
        roi=[c.x()-c.r(),c.y()-c.r(),c.r(),c.r()]
        statistics = img.get_statistics(roi=roi)
        if red_threshold[0]<statistics.l_mode()<red_threshold[1] and red_threshold[2]<statistics.a_mode()<red_threshold[3] and red_threshold[4]<statistics.b_mode()<red_threshold[5] :#if the circle is red
            red_circles.append(c)
    red_max=find_max_circle(red_circles)
    return red_max

def find_max_blue_circles():
    blue_circles=[]
    for c in img.find_circles(threshold = 2000, x_margin = 10, y_margin = 10, r_margin = 10,r_min = 2, r_max = 15, r_step = 2):
        roi=[c.x()-c.r(),c.y()-c.r(),c.r(),c.r()]
        statistics = img.get_statistics(roi=roi)
        if blue_threshold[0]<statistics.l_mode()<blue_threshold[1] and blue_threshold[2]<statistics.a_mode()<blue_threshold[3] and blue_threshold[4]<statistics.b_mode()<blue_threshold[5] :#if the circle is red
            blue_circles.append(c)
            #print(c)
    blue_max=find_max_circle(blue_circles)
    return blue_max

def find_max_black_circles():
    black_circles=[]
    for c in img.find_circles(threshold = 2000, x_margin = 10, y_margin = 10, r_margin = 10,r_min = 2, r_max = 15, r_step = 2):
        roi=[c.x()-c.r(),c.y()-c.r(),c.r(),c.r()]
        statistics = img.get_statistics(roi=roi)
        if black_threshold[0]<statistics.l_mode()<black_threshold[1] and black_threshold[2]<statistics.a_mode()<black_threshold[3] and black_threshold[4]<statistics.b_mode()<black_threshold[5] :#if the circle is red
            black_circles.append(c)
    black_max=find_max_circle(black_circles)
    return black_max

def find_max_yellow_circles():
    yellow_circles=[]
    for c in img.find_circles(threshold = 2000, x_margin = 10, y_margin = 10, r_margin = 10,r_min = 2, r_max = 15, r_step = 2):
        roi=[c.x()-c.r(),c.y()-c.r(),c.r(),c.r()]
        statistics = img.get_statistics(roi=roi)
        if yellow_threshold[0]<statistics.l_mode()<yellow_threshold[1] and yellow_threshold[2]<statistics.a_mode()<yellow_threshold[3] and yellow_threshold[4]<statistics.b_mode()<yellow_threshold[5] :#if the circle is red
            yellow_circles.append(c)
    yellow_max=find_max_circle(yellow_circles)
    return yellow_max
k=5000 #安全区 测距常数
def find_red_home():
    blobs = img.find_blobs([red_threshold])
    for blob in blobs:
        b = blob
        Lm = (b[2]+b[3])/2
        length = k/Lm
    # Draw a rect around the blob.
        if length<2500 and length > 1500:#如果距离在某一阈值之间，判定为安全区
            img.draw_rectangle(b[0:4]) # rect
            img.draw_cross(b[5], b[6]) # cx, cy
            print(length)
            return b
    return -1 #未找到安全区

def find_blue_home():
    blobs = img.find_blobs([blue_threshold])
    for blob in blobs:
        b = blob
        Lm = (b[2]+b[3])/2
        length = k/Lm
        print(length)
        img.draw_rectangle(blob.rect(),color=(255,255,255))
        if (length<500 and length>0):#如果距离在某一阈值之间，判定为安全区
            img.draw_rectangle(b[0:4],color=(0,0,255)) # rect
            img.draw_cross(b[5], b[6],color=(0,0,255)) # cx, cy
            print(length)
            return b
    return -1 #未找到安全区

def Pin_Init():
    P7_In  = pyb.Pin('P7') #P7,下拉输入
    P7_In.init(mode=pyb.Pin.IN,pull=pyb.Pin.PULL_DOWN)
    return P7_In.value()

PinState=Pin_Init()

while(True):
    clock.tick()
    #lens_corr(1.8)畸变矫正
    img = sensor.snapshot().lens_corr(1.8)
    if uart_flag==0:        #状态1：抓取红/蓝球
        if PinState==1:#red
            circle = find_max_red_circles()
            if circle:
                img.draw_circle(circle.x(), circle.y(), circle.r(), color = (255, 0, 0))
                SendData(1,circle.x(), circle.y())
                print(circle)
            else:
                SendData(-1,0,0)
                print('not found')
        else:#blue
            circle = find_max_blue_circles()
            if circle:
                img.draw_circle(circle.x(), circle.y(), circle.r(), color = (0, 0, 255))
                SendData(1,circle.x(), circle.y())
                print(circle)
            else:
                SendData(-1,0,0)
                print('u0 blue not found',circle)
    elif uart_flag==1:    #寻找安全区
        if PinState==1:#red
            red_home=find_red_home()
            if(red_home!=-1):
                SendData(4,red_home[5],red_home[6]) #4标识此时正在寻找安全区
            else:
                SendData(-1,0,0)        #未找到

        elif PinState==0:#blue
            blue_home=find_blue_home()
            if(blue_home!=-1):
                SendData(4,blue_home[5],blue_home[6]) #4标识此时正在寻找安全区
                #print()
            else:
                SendData(-1,0,0)        #未找到
                print('home_not_found')
    else:   #发uart_flag==2，此时寻找各种颜色的小球
        circles=[]
        if PinState==1:#red
            circle1 = find_max_red_circles()
        else:
            circle1 = find_max_blue_circles()
        circle2=find_max_yellow_circles()
        circle3=find_max_black_circles()
        if circle1:
            circles.append(circle1)
            img.draw_circle(circle1.x(), circle1.y(), circle1.r(), color = (255,0,255))
        if circle2:
            circles.append(circle2)
            img.draw_circle(circle2.x(), circle2.y(), circle2.r(), color = (255,255, 0))
        if circle3:
            circles.append(circle3)
            img.draw_circle(circle3.x(), circle3.y(), circle3.r(), color = (0, 0, 0))
        circle=find_max_circle(circles)
        if circle:
            img.draw_circle(circle.x(), circle.y(), circle.r(), color = (255, 255, 255))
            SendData(1,circle.x(), circle.y())
            print(circle)
        else:
            SendData(-1,0,0)
            print('nf',circle3)
