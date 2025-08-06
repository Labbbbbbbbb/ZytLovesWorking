https://blog.csdn.net/Miraitowain/article/details/133467744



Problem:

serial.serialutil.SerialException: [Errno 2] could not open port /dev/ttyAMA0: [Errno 2] No such file or directory: '/dev/ttyAMA0'

`/boot/config.txt`

把disable-bt改成`dtoverlay=pi3-miniuart-bt`

```
enable_uart=1
dtoverlay=pi3-miniuart-bt
```

注：如果发现霉有效果或者说根本霉有`serial0`,可能是需要到`/boot/firmware/config.txt`去把这两行加上

![image-20250523212003117](../../../../Users/86189/AppData/Roaming/Typora/typora-user-images/image-20250523212003117.png)

It should normally be done (as an easy method)
*sudo raspi-config*
Select option 5, Interfacing options,
then option P6, Serial,
Select No to Serial Console
Select Yes to Serial Port.
Reboot.

you'll find it on /dev/serial0

though that is if you are using the two GPIO UART pins.
Not for USB Serial, with that you just plug it in and use /dev/ttyUSB0 as mentioned above.