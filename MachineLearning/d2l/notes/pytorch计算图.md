## 可视化查看pytorch计算图

```bash
pip install torchviz graphviz
```

下载`Graphviz`软件：https://graphviz.org/download/

```
from torchviz import make_dot

X=torch.randn(1,2)
y=model(X)	#y是模型的输出(y_hat)

dot = make_dot(y, params=dict(model.named_parameters()))
dot.render("model_graph", format="png")
```

程序会生成一个名为model_graph的中间文件，最后渲染得到png