
'''
相当于n个并列的linear层,输出进入softmax函数进行归一化,输出各类别的概率分布
每一个列向量的值就是每个类代表的矩阵，因为softmax处理后每一行的和为1，所以每一行的每一个值就可以看作是该样本属于某个类别的概率

softmax分类问题几乎总是搭配交叉熵损失
'''

import torch
from IPython import display
from d2l import torch as d2l

def softmax(X):##softmax的实现：先指数化，再归一化
    X_exp = torch.exp(X)
    partition = X_exp.sum(1, keepdim=True)
    return X_exp / partition  # 这里应用了广播机制

def net(X):     #定义softmax回归模型
    return softmax(torch.matmul(X.reshape((-1, W.shape[0])), W) + b)
'''
每张图像的像素数为1×28×28=784，因此W的输入维度W.shape[0]=784；
X.reshape((-1, 784))会将(32, 1, 28, 28)的X展平为(32, 784)的二维矩阵：
第一维32：保持批量大小（32 张图）；
第二维784：每张图的所有像素被 “拉成” 一个长度为 784 的向量（28×28=784）。
X reshape后列数与W的行数相同，才能进行矩阵乘法
X.reshape乘W再加上b,最后输入softmax归一化,W的列数为类别数10
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
    
    