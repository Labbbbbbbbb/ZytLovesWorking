
'''
相当于n个并列的linear层,输出进入softmax函数进行归一化,输出各类别的概率分布
每一个列向量的值就是每个类代表的矩阵，因为softmax处理后每一行的和为1，所以每一行的每一个值就可以看作是该样本属于某个类别的概率

softmax分类问题几乎总是搭配交叉熵损失
'''

import torch
from torch import nn
from IPython import display
from d2l import torch as d2l
import matplotlib.pyplot as plt
from torchviz import make_dot
'''
def softmax(X):##softmax的实现：先指数化，再归一化
    X_exp = torch.exp(X)
    partition = X_exp.sum(1, keepdim=True)
    return X_exp / partition  # 这里应用了广播机制

def net(X):     #定义softmax回归模型
    return softmax(torch.matmul(X.reshape((-1, W.shape[0])), W) + b)
'''
'''
每张图像的像素数为1×28×28=784，因此W的输入维度W.shape[0]=784；
X.reshape((-1, 784))会将(32, 1, 28, 28)的X展平为(32, 784)的二维矩阵：
第一维32：保持批量大小（32 张图）；
第二维784：每张图的所有像素被 “拉成” 一个长度为 784 的向量（28×28=784）。
X reshape后列数与W的行数相同，才能进行矩阵乘法
X.reshape乘W再加上b,最后输入softmax归一化,W的列数为类别数10
'''
'''
def cross_entropy(y_hat, y):        #使用样本索引的方式计算交叉熵损失
    return - torch.log(y_hat[range(len(y_hat)), y])



if __name__ == "__main__":
    batch_size = 256
    train_iter, test_iter = d2l.load_data_fashion_mnist(batch_size)

    num_inputs = 28*28  #输入为图片的像素大小
    num_outputs = 10    #输出为类别数目，此处fashion_mnist有10个类别
    W = torch.normal(0, 0.01, size=(num_inputs, num_outputs), requires_grad=True)
    b = torch.zeros(num_outputs, requires_grad=True)
    
    '''
    
import torch
from torch import nn
from d2l import torch as d2l
    
def init_weights(m):
    if type(m) == nn.Linear:
        nn.init.normal_(m.weight, std=0.01)
        
def accuracy(y_hat, y):  #@save
    """计算预测正确的数量"""
    if len(y_hat.shape) > 1 and y_hat.shape[1] > 1:
        y_hat = y_hat.argmax(axis=1)
    cmp = y_hat.type(y.dtype) == y
    return float(cmp.type(y.dtype).sum())

def evaluate_accuracy(net, data_iter):  #@save
    """计算在指定数据集上模型的精度"""
    if isinstance(net, torch.nn.Module):
        net.eval()  # 将模型设置为评估模式
    metric = Accumulator(2)  # 正确预测数、预测总数
    with torch.no_grad():
        for X, y in data_iter:
            metric.add(accuracy(net(X), y), y.numel())
    return metric[0] / metric[1]

class Accumulator:  #@save
    """在n个变量上累加"""
    def __init__(self, n):
        self.data = [0.0] * n

    def add(self, *args):
        self.data = [a + float(b) for a, b in zip(self.data, args)]

    def reset(self):
        self.data = [0.0] * len(self.data)

    def __getitem__(self, idx):
        return self.data[idx]
    
def train_epoch_ch3(net, train_iter, loss, updater):  #@save
    """训练模型一个迭代周期（定义见第3章）"""
    # 将模型设置为训练模式
    if isinstance(net, torch.nn.Module):
        net.train()
    # 训练损失总和、训练准确度总和、样本数
    metric = Accumulator(3)
    for X, y in train_iter:
        # 计算梯度并更新参数
        y_hat = net(X)
        l = loss(y_hat, y)
        if isinstance(updater, torch.optim.Optimizer):
            # 使用PyTorch内置的优化器和损失函数
            updater.zero_grad()
            l.mean().backward()
            updater.step()
        else:
            # 使用定制的优化器和损失函数
            l.sum().backward()
            updater(X.shape[0])
        metric.add(float(l.sum()), accuracy(y_hat, y), y.numel())
        # 可视化计算图并保存为 PNG 文件
        #dot = make_dot(y_hat, params=dict(net.named_parameters()))
        #dot.render("model_graph", format="png")
    # 返回训练损失和训练精度
    return metric[0] / metric[2], metric[1] / metric[2]
class Animator:  #@save
    """在动画中绘制数据"""
    def __init__(self, xlabel=None, ylabel=None, legend=None, xlim=None,
                 ylim=None, xscale='linear', yscale='linear',
                 fmts=('-', 'm--', 'g-.', 'r:'), nrows=1, ncols=1,
                 figsize=(3.5, 2.5)):
        # 增量地绘制多条线
        if legend is None:
            legend = []
        d2l.use_svg_display()
        self.fig, self.axes = d2l.plt.subplots(nrows, ncols, figsize=figsize)
        if nrows * ncols == 1:
            self.axes = [self.axes, ]
        # 使用lambda函数捕获参数
        self.config_axes = lambda: d2l.set_axes(
            self.axes[0], xlabel, ylabel, xlim, ylim, xscale, yscale, legend)
        self.X, self.Y, self.fmts = None, None, fmts

    def add(self, x, y):
        # 向图表中添加多个数据点
        if not hasattr(y, "__len__"):
            y = [y]
        n = len(y)
        if not hasattr(x, "__len__"):
            x = [x] * n
        if not self.X:
            self.X = [[] for _ in range(n)]
        if not self.Y:
            self.Y = [[] for _ in range(n)]
        for i, (a, b) in enumerate(zip(x, y)):
            if a is not None and b is not None:
                self.X[i].append(a)
                self.Y[i].append(b)
        self.axes[0].cla()
        for x, y, fmt in zip(self.X, self.Y, self.fmts):
            self.axes[0].plot(x, y, fmt)
        self.config_axes()
        display.display(self.fig)
        display.clear_output(wait=True)
def train_ch3(net, train_iter, test_iter, loss, num_epochs, updater):  #@save
    """训练模型（定义见第3章）"""
    animator = Animator(xlabel='epoch', xlim=[1, num_epochs], ylim=[0.3, 0.9],
                        legend=['train loss', 'train acc', 'test acc'])
    for epoch in range(num_epochs):
        train_metrics = train_epoch_ch3(net, train_iter, loss, updater)
        test_acc = evaluate_accuracy(net, test_iter)
        animator.add(epoch + 1, train_metrics + (test_acc,))
    train_loss, train_acc = train_metrics
    assert train_loss < 0.5, train_loss
    assert train_acc <= 1 and train_acc > 0.7, train_acc
    assert test_acc <= 1 and test_acc > 0.7, test_acc
        
if __name__ == "__main__":
    batch_size = 256
    train_iter, test_iter = d2l.load_data_fashion_mnist(batch_size)
    net = nn.Sequential(nn.Flatten(), nn.Linear(784, 10))
    
    net.apply(init_weights);
    #如果当前模块是 nn.Linear（线性层），就用均值为 0、标准差为 0.01 的正态分布初始化该层的权重（m.weight）。
    
    loss = nn.CrossEntropyLoss(reduction='none')
    trainer = torch.optim.SGD(net.parameters(), lr=0.1)
    num_epochs = 10
    train_ch3(net, train_iter, test_iter, loss, num_epochs, trainer)
    
    X = torch.rand(size=(1, 1, 28, 28))
    y_hat = net(X)
    # 可视化计算图并保存为 PNG 文件
    dot = make_dot(y_hat, params=dict(net.named_parameters()))
    dot.render("model_graph", format="png")
    
    
    plt.show()