### 陀螺仪不能用超声波洗。。。。

### 库函数在同目录下 JY_Lib,JY系列通用



![image-20250913191802907](F:\ITS_Log\WorkSpace\Projects_Log\zdiy1\notes\assets\image-20250913191802907.png)



食用教程：

```
#include "jy901s.h"
  /*2中初始化*/
  JY901S_Init();
  /*while循环*/
  JY901S_Update();
```

 fAcc、fGyro、fAngle是全局变量