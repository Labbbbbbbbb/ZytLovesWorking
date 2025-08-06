# Linux安装Cuda Toolkit

https://docs.nvidia.com/cuda/cuda-installation-guide-linux/

## 3.1. 验证您拥有具有 CUDA 功能的 GPU[⃁](https://docs.nvidia.com/cuda/cuda-installation-guide-linux/#verify-you-have-a-cuda-capable-gpu)

要验证您的 GPU 是否具有 CUDA 功能,请转到发行版的系统属性等价,或者从命令行输入:

```
$ lspci | grep -i nvidia
```

如果您没有看到任何设置,请通过输入来更新 Linux 维护的 PCI 硬件数据库。`update-pciids`(一般发现在`/sbin`)在命令行并重新运行上一个`lspci`命令。

如果您的显卡来自 NVIDIA,并且在 [https://developer.nvidia.](https://developer.nvidia.com/cuda-gpus)com/cuda-gpus 中列出,则您的 GPU 具有 CUDA 功能。CUDA 工具包的发布说明还包含受支持产品列表。

## 3.2. 验证您是否支持了 Linux 版本[⃁](https://docs.nvidia.com/cuda/cuda-installation-guide-linux/#verify-you-have-a-supported-version-of-linux)

CUDA 开发工具仅支持 Linux 的某些特定发行版。这些列在CUDA Toolkit发布说明中。

要确定您正在运行的发行版和发布号,请在命令行中键入以下内容:

```
$ hostnamectl
```

## 3.3. 验证系统已安装 gcc[⃁](https://docs.nvidia.com/cuda/cuda-installation-guide-linux/#verify-the-system-has-gcc-installed)

因`gcc`使用 CUDA Toolkit 开发需要编译器。运行 CUDA 应用程序不需要。它通常作为Linux安装的一部分安装,在大多数情况下,使用支持的Linux版本安装的gcc将正常工作。

要验证系统上安装的 gcc 版本,请在命令行上键入以下内容:

```
gcc --version
```

如果出现错误消息,您需要从 Linux 发行版安装开发工具或获取一个版本`gcc`以及来自Web的随附工具链。



## 安装Cuda Toolkit

选择如下所示

![image-20250627211521864](/home/zyt/.config/Typora/typora-user-images/image-20250627211521864.png)

```
$wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-ubuntu2404.pin
$sudo mv cuda-ubuntu2404.pin /etc/apt/preferences.d/cuda-repository-pin-600
$wget https://developer.download.nvidia.com/compute/cuda/12.9.1/local_installers/cuda-repo-ubuntu2404-12-9-local_12.9.1-575.57.08-1_amd64.deb
$sudo dpkg -i cuda-repo-ubuntu2404-12-9-local_12.9.1-575.57.08-1_amd64.deb
$sudo cp /var/cuda-repo-ubuntu2404-12-9-local/cuda-*-keyring.gpg /usr/share/keyrings/
$sudo apt-get update
$sudo apt-get -y install cuda-toolkit-12-9
```



| NVIDIA Driver Instructions (choose one option)   |
| ------------------------------------------------ |
|                                                  |
| To install the open kernel module flavor:        |
| `sudo apt-get install -y nvidia-open`            |
| To install the proprietary kernel module flavor: |
| `sudo apt-get install -y cuda-drivers`           |
|                                                  |



**Ubuntu 20.04/22.04/24.04**

要从专有切换到打开:

```
# apt install --autoremove --purge nvidia-open-575 nvidia-driver-575-open
```

要从 open 切换到 postofist:

```
# apt install --autoremove --purge cuda-drivers-575 nvidia-driver-575
```



## Package Manager Installation

```
$sudo dpkg -i cuda-repo-ubuntu2404-12-9-local_12.9.1-575.57.08-1_amd64.deb
$sudo cp -r /var/ cuda-repo-ubuntu2404-12-9-local_12.9.1-575.57.08-1_amd64.deb /usr/share/keyrings/
$wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-keyring_1.1-1_all.deb
$sudo dpkg -i cuda-keyring_1.1-1_all.deb
$sudo apt update
$sudo apt install cuda-toolkit
$sudo apt install nvidia-gds 

```

验证：

https://github.com/nvidia/cuda-samples

然后是接下来这一系列

https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/latest/install-guide.html

但是在

Run a sample CUDA container时没有成功

![image-20250628005709499](/home/zyt/.config/Typora/typora-user-images/image-20250628005709499.png)

根据报错，![image-20250628142057615](/home/zyt/.config/Typora/typora-user-images/image-20250628142057615.png)

因为这个网站没有2404的版本所以可能导致没有下载下来

然后我找了一个2404的教程：
https://blog.csdn.net/engchina/article/details/145362226

但还是会遇到

```                     
$sudo docker run --rm --runtime=nvidia --gpus all ubuntu nvidia-smi     
Failed to initialize NVML: Unknown Error
```

**然后就在一个论坛看到了这个：**

```
I solved it, with the help of https://bobcares.com/blog/docker-failed-to-initialize-nvml-unknown-error/ !

I used just the second part of “Method 1”:

    nvidia-container configuration
    In the file

/etc/nvidia-container-runtime/config.toml

set the parameter

no-cgroups = false

After that restart docker and run test container:

sudo systemctl restart docker
sudo docker run --rm --gpus all nvidia/cuda:11.0-base nvidia-smi

With that, the container loaded and worked immediately. I have no idea why.
```

![image-20250628142519469](/home/zyt/.config/Typora/typora-user-images/image-20250628142519469.png)