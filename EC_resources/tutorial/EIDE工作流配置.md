> 本文档是整个cube＋EIDE工作流配置的教程，每次新建工程都会重复这样的工作，建议熟练掌握
>
> 参考文档：[note/EC/EIDE工作流.md at main · lynliam/note · GitHub](https://github.com/lynliam/note/blob/main/EC/EIDE工作流.md)

## CubeMX配置方法

### 1.芯片选型

打开Cubemx，左侧一栏为历史工程，中间这一栏是新建工程。我们一直都是用下图框选的这个选项，点击ACCESS TO MCU SELECTOR,选择芯片型号

![image-20250902162646293](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902162646293.png)

会弹出来这个界面 ，在搜索框里搜索stm32f103c8t6, 这是我们培训期间使用的板子型号

![image-20250902163524549](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902163524549.png)

右图的芯片型号栏里剩下两个芯片，点击第一个，再点击右上角的Start Project (或者双击两次板子型号也可以直接开启项目)

![image-20250902163728509](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902163728509.png)

开启工程会见到如下界面（简单介绍一下，看不懂没关系，后面用着用着就熟了）

Pinout&Configuration:左边这一栏是系统核心和外设的列表，中间这一栏是详细配置信息，右边是芯片的的引脚图，可视化地呈现了对每一个引脚的配置

![image-20250902164250044](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902164250044.png)

Clock Configuration:时钟树的配置

![image-20250902164612295](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902164612295.png)

Project Manager:配置项目信息，包括项目命名等

![image-20250902164652613](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902164652613.png)



### 2.基础配置

**接下来是重点：CubeMX的一些基础配置，就是后面每一次配置新工程都必走的流程**

左栏点击RCC，High Speed Clock选择Crystal/Ceramic Resonator，意思是为高速时钟源选择晶振

![image-20250902165038413](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902165038413.png)

左栏点击SYS, Debug选择Serial Wire, 选择调试模式为SWD模式

![image-20250902165339261](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902165339261.png)

然后来到时钟树的配置：

把下框的数字8改为72，回车

![image-20250902165624202](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902165624202.png)

弹出这个界面就选OK。这里配置的是芯片运行主频，c8t6的最高主频是72MHz, 我们拉到最大就好

![image-20250902165606838](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902165606838.png)

然后来到Project Manager,配置自己习惯的路径，不要含中文不要含中文

![image-20250902170314297](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902170314297.png)

![image-20250903213928367](F:\ITS_Log\EC_resources\tutorial\assets\image-20250903213928367.png)

OK,点击右上角的GENERATE CODE

![image-20250902170620809](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902170620809.png)

第一次使用该型号芯片时会出现让你下载相应压缩包文件的弹窗，按照它的指示下载、注册ST账号即可，如果遇到一些网络问题就来战队下载

最后代码生成完毕之后会弹出这个，close即可。如果感兴趣也可以openfolder看一下

![image-20250902170714057](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902170714057.png)

## EIDE配置

打开Vscode，点击左侧工具栏EIDE图标![image-20250902171223102](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902171223102.png)，新建项目

![image-20250902171238314](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902171238314.png)

选择空项目

![image-20250902171303003](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902171303003.png)

然后选择Cortex-M项目

![image-20250902171350007](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902171350007.png)

它会让你输入项目名称，记住这里的命名一定一定要和刚才在cube填入的工程名称一模一样，如ITS_peixun ，敲击Enter

![image-20250902171525947](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902171525947.png)

然后会让你选择文件存放位置，要存放在项目的上一级目录，比如我的整个路径是`F:\ITS_Log\WorkSpace\stm32_workspace\ITS_peixun`，那我这里就应该选择放在stm32_workspace底下，因为这里其实是让EIDE创建的项目复写原来的Cube生成的代码文件，如果路径选错了会得到很抽象的工程文件，建议全删了重新来过

如果路径选对了，右下角会出现这个弹窗，说明复写是成功的，点Yes！！（没有出现弹窗的乖乖去重配好了）

![image-20250902171731731](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902171731731.png)

还是在右下角，会继续出现这个弹窗，选择继续即可

![image-20250902171807291](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902171807291.png)

然后就会转换到这个工作区（没有的话点一下左边的eide图标

![image-20250902172352483](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902172352483.png)

接下来是eide项目的配置

#### 添加源文件夹：

![image-20250902172528043](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902172528043.png)

选择普通文件夹：

![image-20250902172630794](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902172630794.png)

选择下框两个文件夹，点击添加原文件夹到项目

![image-20250902174128565](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902174128565.png)

接续添加源文件，选择这个图标

![image-20250902174240141](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902174240141.png)

然后把下面的显示文件种类选为any或asm

![image-20250902174311695](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902174311695.png)

可以看到多出了很多文件，把下图所选的哪个.s文件添加进来

![image-20250902174403858](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902174403858.png)



#### 然后是构建配置：stm32f103c8t6的CPU类型是Cortex-M3没问题

![image-20250902174454674](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902174454674.png)

链接脚本路径点击这个图标![image-20250902174625914](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902174625914.png)打开资源管理器

看到最后那个STM32F103的.ld文件，右键它选择复制相对路径

![image-20250902174658212](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902174658212.png)

然后回到刚才的地方，点这个修改

![image-20250902174815465](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902174815465.png)

直接ctrl+v复制进来，回车即可

![image-20250902174844179](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902174844179.png)

#### 烧录配置：点击下框按钮切换烧录器为STLink

![image-20250902174958866](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902174958866.png)



#### C/C++属性：

##### 包含目录



> ## （==强烈建议==）如果设置中勾选了 `EIDE.Source Tree: Auto Search Include Path`（如下图），那么 EIDE 会自动搜索项目资源中添加的文件夹，这里就不用手动填入了
>
> 打开插件设置
>
> ![image-20250902180423341](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902180423341.png)
>
> ![1662708868034](F:\ITS_Log\EC_resources\tutorial\assets\1662708868034.png)

##### 添加预处理器定义

打开Makefile，在一百来行这里有个CDEFS，把这两行复制下来，-D删掉，中间用一个分号连接变成这样：`USE_HAL_DRIVER;STM32F103xB`，复制下来

![image-20250902175438470](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902175438470.png)

点击这个绿色的加号，复制，回车

![image-20250902175646811](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902175646811.png)

![image-20250902180153378](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902180153378.png)

最后还有一个要勾选的：点击左下角的设置

![image-20250903215046845](F:\ITS_Log\EC_resources\tutorial\assets\image-20250903215046845.png)

在框内输入search，然后点击Embedded IDE

![image-20250903215156911](F:\ITS_Log\EC_resources\tutorial\assets\image-20250903215156911.png)

保证这两个都勾上了（其实第一个就是刚才添加的），本次主要是添加第二个

![image-20250903215245988](F:\ITS_Log\EC_resources\tutorial\assets\image-20250903215245988.png)

配置至此告一段落啦



![image-20250903214237655](F:\ITS_Log\EC_resources\tutorial\assets\image-20250903214237655.png)

编译一下验证是否配置正确：

![image-20250902184951234](F:\ITS_Log\EC_resources\tutorial\assets\image-20250902184951234.png)

在战队拿完板子可以进行烧录：

![ef3c9abe4615ed52bc5735f4ce330f38](F:\ITS_Log\EC_resources\tutorial\assets\ef3c9abe4615ed52bc5735f4ce330f38.png)