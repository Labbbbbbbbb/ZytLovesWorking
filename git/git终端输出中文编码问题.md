在命令行用git status查看的时候中文会输出一堆不知为何物的乱码

![image-20250917130424463](assets\image-20250917130424463.png)

是**UTF-8 编码的中文在被以 “八进制转义序列”（Octal Escape Sequence）形式显示时的结果**，本质上是中文的 UTF-8 字节被转义成了八进制数字表示。

若想在 Git 输出中直接看到中文（避免八进制转义），可通过配置 Git 的字符编码相关参数解决

```
git config --global core.quotepath off		# 确保Git输出路径时不转义非ASCII字符（关键配置）
```

我用了这一句就解决了

![image-20250917130644202](assets\image-20250917130644202.png)

如果解决不了可以参考以下：

```
# 配置Git的字符编码为UTF-8（适配中文）
git config --global i18n.commit.encoding utf-8
git config --global i18n.logoutputencoding utf-8
```

#### 确保终端支持 UTF-8

- **Windows（CMD/PowerShell）**：
  打开终端后，执行 `chcp 65001`（将终端编码设为 UTF-8），或在终端设置中默认使用 “UTF-8” 编码。
- **macOS/Linux（终端）**：
  通常默认已使用 UTF-8，可通过 `echo $LANG` 查看（输出含 `UTF-8` 即可），若不是，可在 `~/.bashrc` 或 `~/.zshrc` 中添加 `export LANG=en_US.UTF-8` 并生效。

