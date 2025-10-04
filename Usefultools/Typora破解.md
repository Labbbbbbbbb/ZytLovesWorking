

新版本的Typora进行了一些刁钻的防护并使用了更恶心的弹窗 于是又下载回1.10.8版本（约为2024版本）

在Typora安装目录依次找到这个文件`resources\page-dist\static\js\LicenseIndex.180dd4c7.c77b6991.chunk.js `

Ctrl+F进行查找：

``` 
e.hasActivated="true"==e.hasActivated 替换为 e.hasActivated="true"=="true"
```

即可将弹窗变为“恭喜激活”，好处是从此可以同时打开n个Typora文件

但并不能取消弹窗

类似于如下方法都会导致闪退

```
在Typora安装目录依次找到这个文件
resources\page-dist\license.html
查找
</body></html>
替换为
</body>
<script>setTimeout(function () {window.close()}, 5);</script>
</html>
```



