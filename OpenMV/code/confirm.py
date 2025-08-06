import sensor, image, time
import struct
from machine import UART

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 1000)

uart = UART(1, 115200)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

def SendData(color):
    data=struct.pack(">bbhbb",
                     0xAA,
                     0xAB,
                     int(color),
                     0xAB,
                     0xAA
                     )
    uart.write(data)



while(True):
    clock.tick()
    img = sensor.snapshot()
    # 基于霍夫圆变换
    # threshold = 3500比较合适。如果视野中检测到的圆过多，请增大阈值；
    # 相反，如果视野中检测到的圆过少，请减少阈值。
    # 考虑国赛时形状的不确定性，可以手动确定好ROI区域直接统计颜色信息
    for c in img.find_circles(threshold = 3500, x_margin = 10, y_margin = 10, r_margin = 10,
            r_min = 2, r_max = 100, r_step = 2):
        area = (c.x()-c.r(), c.y()-c.r(), 2*c.r(), 2*c.r())
        #area为识别到的圆的区域，即圆的外接矩形框
        statistics = img.get_statistics(roi=area)#像素颜色统计
        print(statistics)
        #(0,100,0,120,0,120)是红色的阈值，所以当区域内的众数（也就是最多的颜色），范围在这个阈值内，就说明是红色的圆。
        #l_mode()，a_mode()，b_mode()是L通道，A通道，B通道的众数。
        if 0<statistics.l_mode()<100 and 0<statistics.a_mode()<127 and 0<statistics.b_mode()<127:#if the circle is red
            img.draw_circle(c.x(), c.y(), c.r(), color = (255, 0, 0))#识别到的红色圆形用红色的圆框出来
            SendData(1) #若为红色就发送1
        else:
            pass
            #其他颜色阈值#
    print("FPS %f" % clock.fps())
