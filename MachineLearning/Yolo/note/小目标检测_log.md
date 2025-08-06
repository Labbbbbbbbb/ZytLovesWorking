## 训练一

`python train.py --img 256 --batch 16 --epochs 300 --data custom.yaml --weights yolov5s.pt`

![results](.\assets\results.png)

此时默认使用`yolov5\data\hyps\hyp.scratch-low.yaml"`为超参数文件

> q:epochs是否会太高呢 obj_loss有上升趋势可能是过拟合的征兆 但此时两个mAP，尤其是小目标敏感的mAP还在上升

## 训练二

`python train.py --batch 64  --data custom.yaml --img 256 --epochs 200 --hyp data/hyps/cq09.yaml  --weights yolov5l.pt`

改用`yolov5l`，使用自定义超参数文件，调整了`boxloss`,`objloss`,`clsloss`的权重使其更适合小目标，同时调整了学习率和`warmup_epochs`

```
#cq09.yaml

lr0: 0.02 # initial learning rate (SGD=1E-2, Adam=1E-3) default=0.01
lrf: 0.05 # final OneCycleLR learning rate (lr0 * lrf) default=0.01
momentum: 0.937 # SGD momentum/Adam beta1
weight_decay: 0.0005 # optimizer weight decay 5e-4
warmup_epochs: 5.0 # warmup epochs (fractions ok)		default=3
warmup_momentum: 0.8 # warmup initial momentum
warmup_bias_lr: 0.1 # warmup initial bias lr
box: 0.1 # box loss gain, boxloss负责边界框的回归任务，优化预测框的位置与大小。其核心作用是使预测框尽可能接近真实框。default=0.05
cls: 0.3 # cls loss gain,class loss关注分类任务，确保模型能够正确预测物体类别。default=0.5
cls_pw: 1.0 # cls BCELoss positive_weight
obj: 1.5 # obj loss gain (scale with pixels) default=1
obj_pw: 1.0 # obj BCELoss positive_weight
iou_t: 0.20 # IoU training threshold
anchor_t: 4.0 # anchor-multiple threshold
# anchors: 3  # anchors per output layer (0 to ignore)
fl_gamma: 0.0 # focal loss gamma (efficientDet default gamma=1.5)
hsv_h: 0.015 # image HSV-Hue augmentation (fraction)
hsv_s: 0.7 # image HSV-Saturation augmentation (fraction)
hsv_v: 0.4 # image HSV-Value augmentation (fraction)
degrees: 0.0 # image rotation (+/- deg)
translate: 0.1 # image translation (+/- fraction)
scale: 0.5 # image scale (+/- gain)
shear: 0.0 # image shear (+/- deg)
perspective: 0.0 # image perspective (+/- fraction), range 0-0.001
flipud: 0.0 # image flip up-down (probability)
fliplr: 0.5 # image flip left-right (probability)
mosaic: 1.0 # image mosaic (probability)
mixup: 0.0 # image mixup (probability)
copy_paste: 0.0 # segment copy-paste (probability)
```

暂时不进行数据增强或清洗

epochs给个200试一下先

训练结果：

![0631707b36403f56727860728487ad8](.\assets\0631707b36403f56727860728487ad8.png)

![results2](.\assets\results2.png)



以上两次训练结果分别放在/detect的exp和exp2，训练二的效果还是明显好于一的，mAP和P_R曲线的数据都更好看了 

![f14b457f94c4d771e965da45d924990](.\assets\f14b457f94c4d771e965da45d924990.png)

但还是会存在漏检错检，比如当无人机只是一个小圆点，或者叠在云层里之类的，效果也并不十分好

![0833f2cc9ccccfbb811c89f866cb904](/\assets\0833f2cc9ccccfbb811c89f866cb904.png)

## 训练三

考虑数据增强和清洗

更改epochs（再调大一点

更改验证集和训练集的分配？可能得仔细看看数据集
