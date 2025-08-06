# Img_Processing

## 感光模块sensor

`sensor`是感光元件模块，一般mv文件开头都会有以下这一段用于设置摄像头参数的代码

```
import sensor#引入感光元件的模块

# 设置摄像头
sensor.reset()#初始化感光元件
sensor.set_pixformat(sensor.RGB565)#设置为彩色
sensor.set_framesize(sensor.QVGA)#设置图像的大小,一般Q越多越模糊
sensor.skip_frames()#跳过n张照片，在更改设置后，跳过一些帧，等待感光元件变稳定。

```

最后一个函数的原型是`sensor.skip_frames([n,time])`，作用是跳过一些刚开始不稳定的时候再开始读取图像，实现方式：

```
sensor.skip_frame(20) # 跳过20帧数
sensor.skip_frame(time=2000) # 跳过2000ms=2s
```

#### 拍照

`sensor.snapshot()` 拍摄一张照片，返回一个image对象。

#### 自动增益