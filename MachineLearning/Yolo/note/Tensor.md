# Torch学习

## 张量

```python
>>> tensor_a=torch.arange(0,12)
>>> tensor_a
tensor([ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11])
>>> tensor_a.ndim
1
```

n维张量可以理解为n维数组，张量维数成为轴数，tensor_a.ndim是访问数组维度的方法，如图返回1，即为一维数组（张量）

#### 基本操作

##### 生成张量：

```
#数组形式
array_=np.array([[1,2,3],[4,5,6],[7,8,9]])
#张量形式
tensor_=torch.tensor([[1,2,3],[4,5,6],[7,8,9]])
```

区别：打印出来时，

```
tensor([[1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]])  #tensor形式
array([[1, 2, 3],
       [4, 5, 6],
       [7, 8, 9]])  #数组形式
```

以及用shape时

```
>>> array_.shape
(3, 3)
>>> tensor_.shape
torch.Size([3, 3])
```

等等 可见数组和张量形式和原理上基本一样，但在函数中的表示(以及在gpu运算等)上有所不同

```
>>> torch_c.shape[0]
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'torch_c' is not defined. Did you mean: 'torch'?
>>> tensor_c.shape[0]
3
>>> tensor_d=torch.tensor([[1,2,3,4],[4,5,6],[7,8,9]])
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ValueError: expected sequence of length 4 at dim 1 (got 3)
>>> tensor_d=torch.tensor([[1,2,3,4],[4,5,6,7],[7,8,9]])
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ValueError: expected sequence of length 4 at dim 1 (got 3)
>>> tensor_d=torch.tensor([[1,2,3,4],[4,5,6,7],[7,8,9,10]])
>>> tensor_d.shape[0]
3
>>> tensor_e=torch.tensor([[1],[4],[7]])
>>> tensor_e.ndim
2
>>> tensor_e.shape[1]
1
>>>
```

##### 改变形状

```
torch_a.view(3,4) #---->变为形状为（3，4）的张量
```

##### 生成指定形状的张量

```
>>> torch.zeros((3,4,5))
tensor([[[0., 0., 0., 0., 0.],
         [0., 0., 0., 0., 0.],
         [0., 0., 0., 0., 0.],
         [0., 0., 0., 0., 0.]],

        [[0., 0., 0., 0., 0.],
         [0., 0., 0., 0., 0.],
         [0., 0., 0., 0., 0.],
         [0., 0., 0., 0., 0.]],

        [[0., 0., 0., 0., 0.],
         [0., 0., 0., 0., 0.],
         [0., 0., 0., 0., 0.],
         [0., 0., 0., 0., 0.]]])

>>> torch.ones((3,4,5))
tensor([[[1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.]],

        [[1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.]],

        [[1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.],
         [1., 1., 1., 1., 1.]]])
```



##### 张量的运算

```
#加减
x = torch.tensor([1, 3, 5])
y = torch.tensor([3, 7, 4])

x + y
tensor([ 4, 10,  9])

#点乘
x = torch.tensor([1, 3, 5])
y = torch.tensor([3, 7, 4])

torch.dot(x, y) # 1*3 + 3*7 + 5*4 = 3 + 21 + 20 = 44
tensor(44)

#乘法
x = torch.tensor([1, 3, 5])
y = torch.tensor([3, 7, 4])

x * y
tensor([ 3, 21, 20])#直接把对应位置的数相乘

#数乘
k = 5
x = torch.tensor([1, 3, 5])

k * x
tensor([ 5, 15, 25])
```



```
>>> num_inputs=2
>>> num_examples=1000
>>> true_w=[2,-3.4]
>>> true_b=4.2
>>> features=torch.randn(num_examples,num_inputs,dtype=torch.float32)
>>> lables=true_w[0]*features[:,0]+true_w[1]*features[:,1]+true_b
>>> lables+=torch.tensor(np.random.normal(0,0.01,size=lables.size()),dtype=torch.float32)
```

