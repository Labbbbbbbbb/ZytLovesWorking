报错：

> \>>> import torch OMP: Error #15: Initializing libiomp5md.dll, but found libiomp5md.dll already initialized. OMP: Hint This means that multiple copies of the OpenMP runtime have been linked into the program. That is dangerous, since it can degrade performance or cause incorrect results. The best thing to do is to ensure that only a single OpenMP runtime is linked into the process, e.g. by avoiding static linking of the OpenMP runtime in any library. As an unsafe, unsupported, undocumented workaround you can set the environment variable KMP_DUPLICATE_LIB_OK=TRUE to allow the program to continue to execute, but that may cause crashes or silently produce incorrect results. For more information, please see http://www.intel.com/software/products/support/.

[Everything](https://www.voidtools.com/zh-cn/) 搜索 libiomp5md.dll

发现有Anaconda目录下存在两个libiomp5md.dll

![image-20250701011816115](F:\ITS_Log\tzb\assets\image-20250701011816115.png)

第二个路径是conda虚拟环境下的

第一个路径是torch下的

建议先备份路径

然后把第二个路径中的 libiomp5md.dll 剪切到其他地方问题完美解决

![9f38f9a5f6a62f84d1f5f2b8e44860a](F:\ITS_Log\tzb\assets\9f38f9a5f6a62f84d1f5f2b8e44860a.png)