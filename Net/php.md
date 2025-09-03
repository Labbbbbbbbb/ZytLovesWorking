[PHP 教程 | 菜鸟教程](https://www.runoob.com/php/php-tutorial.html)

[PHP开发工具 | 菜鸟教程](https://www.runoob.com/w3cnote/php-develop-tools.html)

[WampServer download | SourceForge.net](https://sourceforge.net/projects/wampserver/)

若安装WampServer时遇到这个：

![img](F:\ITS_Log\Net\assets\file-309e8b97-596d-41e6-8c43-9f112372ccb3response-content-disposition=attachment%3B+filename%3Dimage.png&AccessKeyId=OD83TSXECLFQNNSZ3IF6&Expires=1756969554&Signature=A4KynTbr5G6L9RuCi9E%2BhiK03W0%3D)

提示列出了三个未安装的组件：

- `VC_2013_REDIST_X86`： Visual C++ 2013 的 32位 (x86) 版本
- `VC_2013_REDIST_X64`： Visual C++ 2013 的 64位 (x64) 版本 (Update 5)
- `VC_2022_REDIST_X64`： Visual C++ 2015-2022 的 64位 (x64) 版本

- **按照程序所说的，前往这个网址：** https://wampserver.aviatechno.net/
- **在页面上找到 “Visual C++ Redistribuable Packages” 这一部分。**
- 根据页面上的指导（"Best way to install Visual C++ Redistribuable Packages"），**下载并安装**所有列出的 VC++ 安装包（通常包括 x86 和 x64 版本）。
- 安装完所有必需的 VC++ 组件后，**重新运行** WampServer 的安装程序。

![image-20250903150803705](F:\ITS_Log\Net\assets\image-20250903150803705.png)

![image-20250903150825970](F:\ITS_Log\Net\assets\image-20250903150825970.png)