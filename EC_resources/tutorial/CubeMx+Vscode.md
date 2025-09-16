# CubeMX+VSCodeEIDE安装  

## CubeMX安装

STM32用的**CubeMX**是ST公司出品的一款C代码生成器软件，它的图形化工具界面可以让使用者可视化地选择所需的芯片，并设置选用任一引脚、任一外设。另外的，**CubeMX**还提供了自动识别引脚冲突、设置时钟树、功耗预测等功能。使用**CubeMX**能减轻底层开发工作量，节约时间。

简单地说，我们要为单片机编程肯定需要大量的底层函数，然后才是我们便于调用的API，而CubeMX就是一个根据我们图形化的配置操作自动生成和封装好底层函数的代码生成软件。

### 下载安装包

##### 点击群文件里的安装程序

![image-20250831120954593](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831120954593.png)

##### 或者去官网自己下载这个安装包（两者效果是一样的）

> [STM32CubeMX：图形化工具 - 意法半导体STMicroelectronics](https://www.st.com.cn/content/st_com/zh/stm32cubemx.html#get_started_container)
>
> ![image-20250831120039684](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831120039684.png)
>
> 点击`下载软件`->` 接受`->` 创建MyST账户`->` 下载stm32cubemx`
>
> ![image-20250831120819076](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831120819076.png)
>
> 下载完压缩包后解压，这个`.exe`文件和群文件中的是一样的



### 运行安装程序

点击`.exe`程序

这两个选项都可以，看自己偏好，电脑里没有多个用户的话没影响

![image-20250831121931607](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831121931607.png)

然后一路点击`next`和同意

选择安装路径，以下为示例 （**注意：路径千万不要有中文字符，并且要确认装在一个新建的文件夹(如下图中的`\STM32CubeMX`里面，如果你只写了`F:\ProgramFile`的话会导致一堆文件散落在你的`ProgramFile`文件夹里**）

![image-20250831121454455](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831121454455.png)

以下默认即可

![image-20250831121508120](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831121508120.png)

`next`->下载->` Done`    然后就完成cube的下载啦



## Vscode+EIDE 安装

要下载的有：

- Vscode
- arm-none-eabi-gcc
- OpenOCD

### Vscode

[Visual Studio Code - Code Editing. Redefined](https://code.visualstudio.com/)下载最新版本即可

或点击群里的安装包

![image-20250904114418972](F:\ITS_Log\EC_resources\tutorial\assets\image-20250904114418972.png)

选项默认即可，非常简单

EIDE是Vscode的一个插件，它使vscode可以实现单片机编程IDE的功能，在左边工具栏里点这个![image-20250831161726901](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831161726901.png)图标，这是管理拓展的地方

搜索EIDE，安装拓展

![image-20250831132538587](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831132538587.png)

下载完成会在左侧工具栏看到这个图标

![image-20250831154423470](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831154423470.png)

vscode中还有很多好用的插件，下载如下几个

![image-20250831132213254](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831132213254.png)

如果需要汉化下载这个

![image-20250831132433591](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831132433591.png)

其他推荐的插件，大家可以自行选择：

![image-20250831153521327](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831153521327.png)

![image-20250831153658462](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831153658462.png)

![image-20250831153802629](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831153802629.png)

![image-20250831153930369](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831153930369.png)



### Arm-gnu-toolchain交叉编译链

由于vscode本身还是只起到了编辑器的作用，我们需要为它去配置编译工具，交叉编译链就是用来把高级语言跨平台翻译成arm架构的单片机能懂的语言

[Arm GNU Toolchain Downloads – Arm Developer](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)

![image-20250831152425707](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831152425707.png)

或者找群里的安装包也可以

![image-20250904114526357](F:\ITS_Log\EC_resources\tutorial\assets\image-20250904114526357.png)

运行安装程序，自行选择位置，建议不要装在C盘，最后把这一项勾上，就不需要再手动添加环境变量了

![image-20250831152450685](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831152450685.png)

然后在Vscode中打开EIDE的插件设置配置`Arm Embedded Toolchain`的下载地址

点击左侧的EIDE图标![image-20250831154436860](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831154436860.png)

![image-20250831154505392](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831154505392.png)

![image-20250831153021186](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831153021186.png)

将下载地址填入即可（填到`arm-none-eabi`的上一级即可）

验证安装完成：win+R输入CMD

![image-20250914145743875](F:\ITS_Log\EC_resources\tutorial\assets\image-20250914145743875.png)

输入`arm-none-eabi-gcc --version`，，输出：

![image-20250914150320875](F:\ITS_Log\EC_resources\tutorial\assets\image-20250914150320875.png)

说明已经成功安装

### OpenOCD

（Open On-Chip Debugger）开源片上调试器，debug时会用到它

[Download OpenOCD for Windows](https://gnutoolchains.com/arm-eabi/openocd/)

![image-20250831155021920](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831155021920.png)

或者同样直接下载群里的压缩包

![image-20250904114531945](F:\ITS_Log\EC_resources\tutorial\assets\image-20250904114531945.png)

**解压**，**放到自己习惯放软件的地方**，然后添加环境变量：

点进文件夹里，可以看到里面有个bin文件夹

![image-20250831155517921](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831155517921.png)

右键bin文件夹 复制文件地址

然后在屏幕底边任务栏的搜索那里搜“编辑系统环境变量”，会打开这个界面，点环境变量：

![image-20250831155725604](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831155725604.png)

点击Path（这里是系统变量，选择个人的用户变量也没问题

![image-20250831160002709](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831160002709.png)

点击新建，将刚复制过来的地址复制进去，记得把双引号删掉，以下图为例，我添加的是

```
F:\ProgramFile\openocd-20250710\OpenOCD-20250710-0.12.0\bin
```

![image-20250831155858685](F:\ITS_Log\EC_resources\tutorial\assets\image-20250831155858685.png)

验证安装：在cmd中输入`openocd -v`

![image-20250914150833831](F:\ITS_Log\EC_resources\tutorial\assets\image-20250914150833831.png)

说明系统可以找到openocd，安装成功

### 安装stlink驱动

这是我们的烧录工具需要的驱动

![image-20250914150614784](F:\ITS_Log\EC_resources\tutorial\assets\image-20250914150614784.png)

点击里面的`dpinst_amd64.exe`按照它的指引走完即可，非常快。