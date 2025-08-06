import torch
import torchvision
from torchvision.models.detection import FasterRCNN
from torchvision.models.detection.rpn import AnchorGenerator

# 加载预训练的 ResNet-50 模型
backbone = torchvision.models.resnet50(weights=True)
backbone = torch.nn.Sequential(*list(backbone.children())[:-2])
backbone.out_channels = 2048

# 定义 RPN 的 anchor 生成器
rpn_anchor_generator = AnchorGenerator(
sizes=((32, 64, 128, 256, 512),),
aspect_ratios=((0.5, 1.0, 2.0),)
)

# 定义 ROI 对齐层
roi_pooler = torchvision.ops.MultiScaleRoIAlign(
featmap_names=['0'],
output_size=7,
sampling_ratio=2
)

# 创建 Faster R-CNN 模型
model = FasterRCNN(
backbone,
num_classes=91,
rpn_anchor_generator=rpn_anchor_generator,
box_roi_pool=roi_pooler
)

# 打印模型结构
print(model)