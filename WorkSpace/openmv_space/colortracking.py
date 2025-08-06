import sensor, image, time
import struct
from machine import UART
import pyb

red_threshold  = (13, 49, 18, 61, 6, 47)
blue_threshold  = (4, 49, 9, 32, -113, -24)
black_threshold  = (0, 8, -12, 4, -3, 11)
yellow_threshold  = (48, 100, -60, 0, 17, 126)

uart = UART(1, 115200)

sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565) # use RGB565.
sensor.set_framesize(sensor.QVGA) # use QQVGA for speed.
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_gain(False)  # must be turned off for color tracking
sensor.set_auto_whitebal(False) # turn this off.
clock = time.clock() # Tracks FPS.

def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob=blob
            max_size = blob[2]*blob[3]
    return max_blob

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
    uart.write(data)

def Pin_Init():
    P7_In  = pyb.Pin('P7') #P7,下拉输入
    P7_In.init(mode=pyb.Pin.IN,pull=pyb.Pin.PULL_DOWN)
    return P7_In.value()

PinState=Pin_Init()
while(True):        #暂时的逻辑是这样，如果需要优化寻找顺序的话可能需要接收来自板子的信息
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot() # Take a picture and return the image.
    if(PinState==1):
        blobs_1 = img.find_blobs([red_threshold])
        blobs_all = img.find_blobs([red_threshold,yellow_threshold,black_threshold])
    else:
        blobs_1 = img.find_blobs([blue_threshold])
        blobs_all = img.find_blobs([blue_threshold,yellow_threshold,black_threshold])
    blobs_black = img.find_blobs([black_threshold])
    blobs_yellow = img.find_blobs([yellow_threshold])

    if blobs_1:
        max_blob = find_max(blobs_1)
        img.draw_rectangle(max_blob.rect()) # rect
        img.draw_cross(max_blob.cx(), max_blob.cy()) # cx, cy
        SendData(1,max_blob.cx(),max_blob.cy())
        print(1,max_blob.cx(),max_blob.cy())
    elif blobs_all:
        max_blob = find_max(blobs_all)
        img.draw_rectangle(max_blob.rect()) # rect
        img.draw_cross(max_blob.cx(), max_blob.cy()) # cx, cy
        SendData(2,max_blob.cx(),max_blob.cy())
        print(2,max_blob.cx(),max_blob.cy())
    else:
        SendData(-1,-1,-1)   #异常值，表示视野里没有目标颜色
        print('Not Found')
