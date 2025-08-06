`env:ubuntu24.04`

解决问题：

```
ImportError: libpython3.8.so.1.0: cannot open shared object file: No such file or directory
```

但是使用查找的话会发现

```
$sudo find / -name libpython3.8.so.1.0                                                   
/home/zyt/miniconda3/pkgs/python-3.8.20-he870216_0/lib/libpython3.8.so.1.0
/home/zyt/miniconda3/envs/Fast_RCNN/lib/libpython3.8.so.1.0
/home/zyt/miniconda3/envs/humanoid-gym-op/lib/libpython3.8.so.1.0
/var/lib/docker/overlay2/7fe7802672ececc7fc3856c044a188c3fd2f047ae4a82ca2da6887b81a46a8fa/diff/usr/lib/x86_64-linux-gnu/libpython3.8.so.1.0
/var/lib/docker/overlay2/50b80a7ab07ea982347edac0010966f06eb23cbe808deedafcb0117993eab823/merged/usr/lib/x86_64-linux-gnu/libpython3.8.so.1.0
find: ‘/proc/4649/task/4649/net’: 无效的参数
find: ‘/proc/4649/net’: 无效的参数
find: ‘/run/user/1000/gvfs’: 权限不够
find: ‘/run/user/1000/doc’: 权限不够

```

conda环境里存在这个文件，可能是全局的apt还是什么的找不到它，于是使用了软链接：

```
sudo ln -s /home/zyt/miniconda3/envs/humanoid-gym-op/lib/libpython3.8.so.1.0 /usr/lib/x86_64-linux-gnu/libpython3.8.so.1.0 
```

前面的是实际的文件，后面的链接上的一个实际上不存在的文件，也是刚刚系统在找但是没找到的路径名，相当于创建了一个快捷方式。注意软链接不能将文件链接到已经存在的文件上