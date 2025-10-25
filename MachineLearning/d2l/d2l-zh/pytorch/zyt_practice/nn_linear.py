import numpy as np
import torch
from d2l import torch as d2l
from torch.utils import data
from torch import nn

true_w = torch.tensor([1.0, -1.0])
true_b=5.0
features, labels = d2l.synthetic_data(true_w, true_b, 1000)     #生成数据集

def load_array(data_arrays, batch_size, is_train=True):  #@save
    """构造一个PyTorch数据迭代器"""
    dataset = data.TensorDataset(*data_arrays)
    return data.DataLoader(dataset, batch_size, shuffle=is_train)
'''
data.TensorDataset(*data_arrays)：将输入的张量（如 features 和 labels）包装成一个数据集，
其中每个样本是 (feature, label) 的对应组合（按索引匹配）。

data.DataLoader(...)：将数据集转换为一个迭代器，支持按 batch_size 批量读取数据，
且在训练模式（is_train=True）下会随机打乱数据顺序。

最终 data_iter 是一个可迭代对象，每次迭代返回一个批量的 (特征批量, 标签批量)。
使用next(iter(data_iter))访问迭代器的第一项（一个batch的数据集
'''
data_iter = load_array((features, labels), batch_size=10)



net = nn.Sequential(nn.Linear(2, 1))
'''
在 PyTorch 中，nn.Linear 是用于实现线性变换（全连接层） 的类，对应数学中的 affine transformation（仿射变换），
其核心功能是对输入数据执行 “矩阵乘法 + 偏置” 的运算。

'''

net[0].weight.data.normal_(0, 0.01)
net[0].bias.data.fill_(0)
'''
初始化权重参数
net[0].weight.data.normal_(0, 0.01)
net[0]：假设 net 是用 nn.Sequential 定义的模型（如之前提到的 nn.Sequential(nn.Linear(2, 1))），net[0] 表示访问模型中的第一个层（这里是 nn.Linear 层）。
weight：nn.Linear 层的权重参数（即数学中的 W 矩阵）。
data：获取参数的底层数据（张量），在 PyTorch 中，参数通常封装在 Parameter 对象中，data 用于直接访问其存储的张量值。
normal_(0, 0.01)：这是一个原地操作（方法名末尾的 _ 表示原地修改），将权重张量初始化为均值为 0、标准差为 0.01 的正态分布（高斯分布）。
net[0].bias.data.fill_(0)
bias：nn.Linear 层的偏置参数（即数学中的 b 向量）。
fill_(0)：也是一个原地操作，将偏置张量的所有元素填充为 0。
'''

#loss = nn.MSELoss()
#loss=nn.HuberLoss()#结合 MSE 和 MAE 的优点，对异常值稳健且梯度更稳定。
#loss = nn.CrossEntropyLoss()分类任务常用
loss = nn.L1Loss() #MAE损失函数


trainer = torch.optim.SGD(net.parameters(), lr=0.03) #定义优化算法随机梯度下降（SGD）
'''
parameters() 是模型的一个方法，返回模型中所有需要学习的参数（即 nn.Linear 层的 weight 和 bias 等）。
优化器需要知道要更新哪些参数，因此这里将模型的所有参数传递给 SGD。
'''

num_epochs = 3
for epoch in range(num_epochs):
    for X, y in data_iter:  #X,y是一个batch的数据和标签，这个for循环会遍历所有epoch中的所有batch
        l = loss(net(X) ,y)
        trainer.zero_grad()
        l.backward()
        trainer.step()
    l = loss(net(features), labels)
    print(f'epoch {epoch + 1}, loss {l:f}')
    
'''
l.backward() 是反向传播（backpropagation） 的核心操作，用于计算损失 l 对模型中所有可学习参数（权重、偏置等）的梯度（导数）。
这些梯度会被存储在参数的 .grad 属性中，供后续的优化器（如 trainer.step()）使用来更新参数。
'''

w = net[0].weight.data
print('w的估计误差：', true_w - w.reshape(true_w.shape))
b = net[0].bias.data
print('b的估计误差：', true_b - b)


'''
weight.data：权重参数的底层张量（不经过 Parameter 封装，直接访问数值）。

bias.data：偏置参数的底层张量。

linear_layer.weight.grad可以访问权重参数的梯度张量，而linear_layer.bias.grad可以访问偏置参数的梯度张量。
'''