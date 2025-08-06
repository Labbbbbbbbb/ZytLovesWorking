# Pytorch

## 环境搭建

[深度学习环境配置超详细教程【Anaconda+PyTorch(GPU版)+CUDA+cuDNN】-CSDN博客](https://blog.csdn.net/qq_43874102/article/details/123164105)

*1.下载Anaconda3环境*

ps：Anaconda Prompt和Navigator都可以在windows搜索栏中找到

*2.下载pycharm以及破解*

3.用`nvidia-smi`查看显卡信息，如果发现查看不到，就去英伟达官网下

4.下载pytorch

![image-20240814145941774](.\assets\image-20240814145941774.png)

CUDA12

```
conda install pytorch torchvision torchaudio pytorch-cuda=12.4 -c pytorch -c nvidia
```



最好不要用`pip`下  似乎会有某个依赖不兼容



![image-20240814150211897](.\assets\image-20240814150211897.png)

(只有在下载了torch模块的解释器才能正确运行，所以最好使用conda管理)