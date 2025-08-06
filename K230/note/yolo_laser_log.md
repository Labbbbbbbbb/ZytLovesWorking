第一次训练结果（136张图片，150epochs）

![image-20250727143351043](.\assets\image-20250727143351043.png)

![image-20250727144329596](.\assets\image-20250727144329596.png)

![results](.\assets\results.png)

![val_batch1_pred](.\assets\val_batch1_pred.jpg)

总之就是很难绷





第二次：由于数据集量少，加上对推理速度的需求，换成`yolov5n`再试试看

![results](.\assets\results-1753602589290-5.png)

![val_batch1_pred](.\assets\val_batch1_pred-1753602568568-3.jpg)

![image-20250727155019816](.\assets\image-20250727155019816.png)

可能能好一点？但是帧率确实上来了  从105ms->75ms每帧







第三次:  ` python train.py --img 640 --batch 16 --epochs 150 --data redlaser.yaml --weights yolov5n.pt --device 0 --hyp hyp.custom.yaml` 指定自定义的超参数文件（这一次的初始学习率从0.01-->0.005 

![image-20250727155238439](.\assets\image-20250727155238439.png)

![results](.\assets\results-1753603655431-7.png)

![val_batch0_pred](.\assets\val_batch0_pred.jpg)

是不是能好一点点。。。

