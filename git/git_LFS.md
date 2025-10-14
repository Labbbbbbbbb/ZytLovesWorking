[【git】github如何上传超过100MB大小的单个文件 - mayoyi - 博客园](https://www.cnblogs.com/asyaB404/p/18278850)

[使用 Git LFS 管理大文件并上传至 GitHub使用 Git LFS 管理大文件并上传至 GitHub 的完整实践 - 掘金](https://juejin.cn/post/7524966734665711631)

[关于 GitHub 上的大文件 - GitHub 文档](https://docs.github.com/zh/repositories/working-with-files/managing-large-files/about-large-files-on-github)

比较新的git版本都是默认带了LFS的，因此不需要像第二个链接那样去sudo install



对一个仓库，只需要：

初始化

```
git lfs install		//initialized gitLFS，会生成attributes文件
```

指定大文件(指定要跟踪的类型（正则表达式）)

```
git lfs track "*.pdf" //之类的文件类型，，直接指定大文件名字似乎没有用
```

然后在.gitattributes配置文件就可以看到刚追踪的文件信息，提交这个文件

```
git add .gitattributes
git commit -m "配置 Git LFS 跟踪大文件"
git push
```

配置完 Git LFS 后，后续添加 `.img/.bin/...` 等大文件时，无需额外操作，直接执行普通 Git 命令即可：

```sql
git add 路径/大文件
git commit -m "添加 xxx 大文件"
git push
```

LFS 会自动生效。

注意添加的大文件一定要是LFS追踪过的

![image-20251015010948956](F:\ITS_Log\git\assets\image-20251015010948956.png)
