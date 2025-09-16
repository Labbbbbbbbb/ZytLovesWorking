在v2.0中的问题：

- MPU6050的EP没有接地（人怎么能这么蠢），并且pitch轴并不好用，v3.0需要转换mpu的方向，同时集成另一款mpu
- stm32f103c8t6的flash太小（虽然在ld文件内直接把flash的大小改成128kb是可以骗过编译器的，但是不知道实际能不能用，可能可以换成CBT6试一下）但是这里想采用激进一点的做法：换成闪存达1M的H743并且再用Qspi外接一块flash（旱的旱死涝的涝死）
- VCPUSB没有上拉电阻导致不能用





[标准协议_SDK快速上手（串口连接）](https://wit-motion.yuque.com/wumwnr/docs/hkv8c9?)

![image-20250913191802907](F:\ITS_Log\WorkSpace\Projects_Log\zdiy1\notes\assets\image-20250913191802907.png)