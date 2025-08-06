

import sensor, image, time,pyb
from pyb import UART
import struct
import json


red_threshold  = (13, 49, 18, 61, 6, 47)
blue_threshold  = (4, 49, 9, 32, -113, -24)
black_threshold  = (0, 8, -12, 4, -3, 11)
yellow_threshold  = (48, 100, -60, 0, 17, 126)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((0,20,320,200))#QVGA find Region Of Interest
sensor.skip_frames(10)
sensor.set_auto_whitebal(False)
clock = time.clock()
uart2 = UART(1, 115200)
uart3 = UART(3, 115200)
def Pin_Init():
    P7_In  = pyb.Pin('P7',pyb.Pin.IN,pyb.Pin.PULL_DOWN) #P7,下拉输入
    return P7_In.value()

def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob.pixels() > max_size:
            max_blob=blob
            max_size = blob.pixels()
    return max_blob
######
def detect(max_blob):#输入的是寻找到色块中的最大色块
    #print(max_blob.solidity())
    shape=0
    if max_blob.solidity()>0.90 or max_blob.density()>0.84:
        img.draw_rectangle(max_blob.rect(),color=(255,255,255))
        shape=1

    elif max_blob.density()>0.6:
        img.draw_circle((max_blob.cx(), max_blob.cy(),int((max_blob.w()+max_blob.h())/4)))
        shape=2

    elif max_blob.density()>0.4:
        img.draw_rectangle(max_blob.rect(),color=(0,0,0))
        shape=3

    return shape
#######
def SendData3(color,cx,cy):
    data=struct.pack(">bbbhhbb",
                     0xFF,
                     0xFE,
                     int(color),
                     int(cx),
                     int(cy),
                     0xFE,
                     0xFF
                     )
    uart3.write(data)
def SendData2(error_flag):
    data=struct.pack(">bbbbb",
                     0xFF,
                     0xFE,
                     int(error_flag),
                     0xFE,
                     0xFF
                     )
    uart2.write(data)

Pin_State = Pin_Init()      #红色为1，蓝色为0
Roi=[17,32,157,156]   #待定，，滚轮口处的roi值
while(True):
    #clock.tick()
    img = sensor.snapshot() # Take a picture and return the image.
    blobs_black = img.find_blobs([black_threshold])
    blobs_red   = img.find_blobs([red_threshold])
    blobs_blue  = img.find_blobs([blue_threshold])
    blobs_yellow = img.find_blobs([yellow_threshold])
    blobs_all = img.find_blobs([yellow_threshold,black_threshold,red_threshold,blue_threshold])
####判断入口处小球的颜色
#m1:ROI+statistic
    statistics = img.get_statistics(roi=Roi)
    if red_threshold[0]<statistics.l_mode()<red_threshold[1] and red_threshold[2]<statistics.a_mode()<red_threshold[3] and red_threshold[4]<statistics.b_mode()<red_threshold[5] :#if the circle is red
        if Pin_State == 0:
            SendData2(1)        #发出1表示异常情况
            print(1)
    if blue_threshold[0]<statistics.l_mode()<blue_threshold[1] and blue_threshold[2]<statistics.a_mode()<blue_threshold[3] and blue_threshold[4]<statistics.b_mode()<blue_threshold[5] :#if the circle is blue
        if Pin_State == 1:
            SendData2(1)        #发出1表示异常情况
            print(1)
#m2:findmax看最大区域是红色还是蓝色
    #if ((find_max(blobs_all)==find_max(blobs_red)) && (Pin_State==0)):
    #    SendData2(1)        #发出1表示异常情况
    #if find_max(blobs_all)==find_max(blobs_blue) && Pin_State==1:
    #    SendData2(1)        #发出1表示异常情况
#######pid区域 返回坐标值
    if blobs_red and (Pin_State==1):
        max_blob_red=find_max(blobs_red)
        #shape_red=detect(max_blob_red)
        img.draw_rectangle(max_blob_red.rect(),color=(255,0,0))
        img.draw_cross(max_blob_red.cx(), max_blob_red.cy(),color=(255,0,0))
        SendData3(1,max_blob_red.cx(),max_blob_red.cy())
        print(1,max_blob_red.cx(),max_blob_red.cy())
    else:
        print('not found red !')


    if blobs_blue and (Pin_State==0):
        max_blob_blue=find_max(blobs_blue)
        #shape_blue=detect(max_blob_blue)
        img.draw_rectangle(max_blob_blue.rect(),color=(0,0,255))
        img.draw_cross(max_blob_blue.cx(), max_blob_blue.cy(),color=(0,0,255))
        SendData3(1,max_blob_blue.cx(),max_blob_blue.cy())
        print(1,max_blob_blue.cx(),max_blob_blue.cy())
    else:
        print('not found blue !')


    if blobs_black:
        max_blob_black=find_max(blobs_black)
        #shape_black=detect(max_blob_black)
        img.draw_rectangle(max_blob_black.rect(),color=(0,0,0))
        img.draw_cross(max_blob_black.cx(), max_blob_black.cy(),color=(0,0,0))
        SendData3(2,max_blob_black.cx(),max_blob_black.cy())
        print(2,max_blob_black.cx(),max_blob_black.cy())
    else:
        print('not found black !')

    if blobs_yellow:
        max_blob_yellow=find_max(blobs_yellow)
        #shape_yellow=detect(max_blob_yellow)
        img.draw_rectangle(max_blob_yellow.rect(),color=(255,255,0))
        img.draw_cross(max_blob_yellow.cx(), max_blob_yellow.cy(),color=(255,255,0))
        SendData3(3,max_blob_yellow.cx(),max_blob_yellow.cy())
        print(3,max_blob_yellow.cx(),max_blob_yellow.cy())
######pid区域
    else:
        print('not found yellow !')




