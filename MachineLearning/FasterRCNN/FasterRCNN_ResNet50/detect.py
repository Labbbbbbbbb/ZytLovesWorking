import torchvision
import numpy
import torch
import argparse
import cv2
import detect_utils
from PIL import Image

args = {
    'min_size': 800,
    'batch_size': 16,
    'learning_rate': 0.001,
    'epochs': 50,
    'input':'input/phone.jpg'

}
model = torchvision.models.detection.fasterrcnn_resnet50_fpn(pretrained=True,min_size=args['min_size'])

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
image = Image.open(args['input'])
model.eval().to(device)
boxes, classes, labels = detect_utils.predict(image, model, device, 0.8)
image = detect_utils.draw_boxes(boxes, classes, labels, image)
cv2.imshow('Image', image)
save_name = f"{args['input'].split('/')[-1].split('.')[0]}_{args['min_size']}"
cv2.imwrite(f"outputs/{save_name}.jpg", image)
cv2.waitKey(0)
