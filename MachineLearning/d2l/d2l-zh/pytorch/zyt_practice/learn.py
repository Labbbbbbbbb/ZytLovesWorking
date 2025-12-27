#matplotlib inline   #for jupyter notebook
import random
import torch
from d2l import torch as d2l
from torch import nn


net=nn.Linear(2,1)
net.weight.data.normal_(0,0.01)
net.bias.data.fill_(0)

for param in net.parameters():
    # 可以在这里打印参数信息，或将其传入优化器
    print(param.shape)