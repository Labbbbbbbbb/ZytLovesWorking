```
$ git init
Initialized empty Git repository in /mnt/g/workspace/temp/gitSubTest/.git/

$ git submodule add https://gitee.com/scitechlabs/stl_utils.git ./modules/stl_utils    
前面是远程仓库地址，后面是文件夹路径

$ git submodule update
更新子仓库的状态
```

要更新子模块在远程的状态，要在父仓库中也进行commit和push，子仓库并非以一个仓库的形式附在父仓库上，而只是一个提交的形式，所以需要在总仓库重新提交推送
