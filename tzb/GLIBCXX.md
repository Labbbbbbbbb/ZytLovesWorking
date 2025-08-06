报错

> ImportError:  /home/zyt/miniconda3/envs/isaacgym/bin/../lib/libstdc++.so.6: version  `GLIBCXX_3.4.32' not found (required by  /home/zyt/.cache/torch_extensions/py38_cu113/gymtorch/gymtorch.so) 

用`strings /usr/lib/x86_64-linux-gnu/libstdc++.so.6 | grep GLIBCXX`查看发现已经存在3.4.32,并不是版本不够新，是它没检测到这个版本

#### **确保 Conda 环境没有覆盖系统库**

如果你使用 Conda，运行：

bash

```
conda list | grep libstdcxx
```

如果存在 `libstdcxx-ng`，可能是它覆盖了系统库，可以尝试：

bash

```
conda remove libstdcxx-ng --force
```