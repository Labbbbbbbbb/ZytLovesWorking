
import numpy as np
import torch
from torch import nn
from d2l import torch as d2l

timer = d2l.Timer()
A = torch.zeros(256, 256)
B = torch.randn(256, 256)
C = torch.randn(256, 256)


# 逐列计算A=BC
timer.start()
A = torch.mm(B, C)
timer.stop()
print(f'逐列计算: {timer.avg():.5f} sec')
