import torch
from d2l import torch as d2l
import matplotlib.pyplot as plt
from torchviz import make_dot

x = torch.arange(-8.0, 8.0, 0.1, requires_grad=True)
y = torch.relu(x)
d2l.plot(x.detach(), y.detach(), 'x', 'relu(x)', figsize=(5, 2.5))


y=torch.prelu(x,torch.tensor([0.5]))
d2l.plot(x.detach(), y.detach(), 'x', 'prelu(x)', figsize=(5, 2.5))
plt.show()

y.backward(torch.ones_like(x), retain_graph=True)
d2l.plot(x.detach(), x.grad.detach(), 'x', 'grad of prelu(x)', figsize=(5, 2.5))
plt.show()

dot = make_dot(y)
dot.render("model_graph", format="png")