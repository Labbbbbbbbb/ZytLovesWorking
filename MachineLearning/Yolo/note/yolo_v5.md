在自定义数据集上训练 YOLOv5s 模型的步骤如下。你需要准备好数据集、标注文件，并配置 YOLOv5 的训练脚本。

------

### **1. 环境准备**

```
conda create -n yolo python=3.8 
```

并进入该环境中

确保你已经安装了 YOLOv5 所需的依赖：

1. 克隆 YOLOv5 仓库：

   复制

   ```
   git clone https://github.com/ultralytics/yolov5
   cd yolov5
   ```

2. 安装依赖：

   复制

   ```
   pip install -r requirements.txt
   ```

------

### **2. 准备自定义数据集**

打标工具：`label image`  或者`roboflow`

**在zythugebrain上跑labelimg只需要运行：**

```
& "C:\Users\zyt\AppData\Roaming\Python\Python39\Scripts\labelimg.exe"
```

(把labelimg下在这个文件夹并且用完整路径寻找)



YOLOv5 要求数据集按照以下结构组织：

复制

```
datasets/
└── custom/
    ├── images/
    │   ├── train/  # 训练图像
    │   └── val/    # 验证图像
    └── labels/
        ├── train/  # 训练标签
        └── val/    # 验证标签
```

#### **2.1 图像数据**

- 将训练图像放入 `datasets/custom/images/train/`。
- 将验证图像放入 `datasets/custom/images/val/`。

#### **2.2 标签数据**

- 每个图像需要对应一个 `.txt` 标签文件，文件名与图像文件名相同。
- 标签文件格式：`<class_id> <x_center> <y_center> <width> <height>`，其中：
  - `class_id`：类别索引（从 0 开始）。
  - `x_center, y_center`：边界框中心点的归一化坐标（相对于图像宽度和高度）。
  - `width, height`：边界框的归一化宽度和高度（相对于图像宽度和高度）。

例如：

复制

```
0 0.5 0.5 0.2 0.3  # 类别 0，边界框中心 (0.5, 0.5)，宽度 0.2，高度 0.3
```

#### **2.3 数据集划分**

- 确保训练集和验证集的图像和标签文件一一对应。

- 可以使用工具（如 `splitfolders`）自动划分数据集：

  bash

  复制

  ```
  pip install splitfolders
  python -m splitfolders --ratio 0.8 0.2 --input path_to_images --output datasets/custom
  ```

------

### 从数据集库拿素材：coco128

下载链接：`https://github.com/ultralytics/yolov5/releases/download/v1.0/coco128.zip`。下载解压以后我们将文件夹放置在与/yolov5同级目录下（放在`datasets`文件夹里面，yolov5文件夹里自带有`coco128.yaml`配置文件，不必另外写

```
但是coco128.yaml的train和val好像用的是同一个文件夹？这合理吗
```





### **3. 配置数据集文件**

在 YOLOv5 中，需要创建一个数据集配置文件（如 `custom.yaml`）：

1. 在 `data/` 目录下创建 `custom.yaml` 文件：

   yaml

   复制

   ```
   path: ../datasets/custom  # 数据集路径
   train: images/train       # 训练图像路径
   val: images/val           # 验证图像路径
   nc: 2                     # 类别数量
   names: ['class1', 'class2']  # 类别名称
   ```

2. 根据你的数据集修改 `nc`（类别数量）和 `names`（类别名称）。

------

**注意label文件夹里面会出现一个classes.txt，里面可能会有一些自带的类，不用的话可以先删了  否则会导致nc数量不是你预想的数**

### **4. 开始训练**

使用以下命令训练 YOLOv5s 模型：

```
python train.py --img 640 --batch 16 --epochs 100 --data custom.yaml --weights yolov5s.pt
```

- **参数说明**：
  - `--img 640`：输入图像大小为 640x640。
  - `--batch 16`：批次大小为 16（根据 GPU 显存调整）。
  - `--epochs 100`：训练 100 个 epoch。
  - `--data custom.yaml`：指定数据集配置文件。
  - `--weights yolov5s.pt`：使用预训练的 YOLOv5s 权重。（这个要安装完依赖才会出现

------

### **5. 训练过程**

1. 训练开始后，YOLOv5 会显示训练进度、损失值、精度等信息。

2. 训练完成后，模型权重会保存在 `runs/train/exp/weights/` 目录下：
   - `best.pt`：最佳模型权重。
   - `last.pt`：最后一个 epoch 的模型权重。
   
   并且训练完后终端会告诉你训练结果放在哪个位置了 一般是`\run\train\exp*\`底下，里面可以看到
   
   ![image-20250303195051792](.\assets\image-20250303195051792.png)

------

可以由此看出训练成果

![results](.\assets\results1.png)

其中`results.png`可以比较清楚地看出一些指标随横轴`epochs`的变化   一般训练到学习率开始保持不变的`epochs`就是合适的`epochs`

### **6. 验证模型**

训练完成后，可以使用验证图片评估模型性能：

```
python detect.py --source ./data/images/ --weights ./runs/train/exp4/weights/best.pt
```

- `data\images/`下有一些自带的验证图片，也可以删了把自己想验证的一些图片放进去

  ![image-20250303195922803](.\assets\image-20250303195922803.png)

- **参数说明**：
  
  - `--weights`：指定模型权重路径。（此例中为训练过后得到的那组权重
  - `--source`：指定所要验证的图片的路径。

------

### **7. 推理测试**

使用训练好的模型进行推理测试：

```
python detect.py --weights runs/train/exp/weights/best.pt --source path_to_test_images
```

- **参数说明**：
  - `--weights`：指定模型权重路径。
  - `--source`：指定测试图像路径或视频文件。
  - （与上面的验证一样，只是路径改为任何需要检测的图片文件夹即可

------

### **8. 可视化结果**

1. 推理结果会保存在 `runs/detect/exp/` 目录下。
2. 打开生成的图像或视频文件，查看检测效果。

![image-20250303200001723](.\assets\image-20250303200001723.png)