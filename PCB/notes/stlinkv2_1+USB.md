参考[自制ST-Link V2-1（附BOM）](https://www.szlcsc.com/info/13006.html?lcsc_vid=RgAKUFxUQ1IIVVIEEVMLAVdXRwNXUFVQEwNZUlAARVExVlNTQVRaVVBfTlddVTsOAxUeFF5JWAIASQYPGQZABAsLWA%3D%3D)

[STLINK-V2.1(已验证) - 立创开源硬件平台](https://oshwhub.com/samomi/stlink-v21-os-yi-yan-zheng)

参考案例：

- 正点原子潘多拉开发板（资料在EC_resources里面

  v3版本包含USB+OTG和USB+DAP两种，其中DAP是MCU的串口经过DAP芯片（其实是另一块st芯片）出来的USB D+和D-，OTG是MCU(stm32L4系列)本身的OTG引脚直接引出的

  v2版本是stlinkv2.1+OTG

- 魔女开发板，板载CMSIS_DAP

- stm32 nucleo系列，板载stlinkv2.1

[STM32 Nucleo 板: 相关产品 - STMicroelectronics](https://www.st.com.cn/zh/evaluation-tools/stm32-nucleo-boards/products.html?querycriteria=productId=LN1847$$1574=Nucleo-64&aggOrder=0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21)

[GitCode - 全球开发者的开源社区,开源代码托管平台](https://gitcode.com/Open-source-documentation-tutorial/b0005/blob/main/MB1136C.pdf) 原理图



在diy1的设计中，stlinkv2.1有烧录功能并且链接了mcu上的一个硬件串口，同时mcu本身的虚拟串口也通过typec引出了

虚拟串口参考资料如下

[STM32 USB使用记录：使用CDC类虚拟串口（VCP）进行通讯_stm32 usb使用记录:使用cdc类虚拟串口(vcp)进行通讯-CSDN博客](https://blog.csdn.net/Naisu_kun/article/details/118192032)