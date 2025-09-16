# 环境配置
## 1.依赖包
```
pip install itchat
pip install requests
```
## 2.申请openai api key，并修改python文件中api_key的路径
这一步网上可以搜到

## 3.更改itchat文件的一个小地方：
初次运行时，可能会收到以下报错：
```shell
~~~~~~~~~~~~~~~~~~~^^^^^^^^^^^^^^
File "C:\Users\kitten\AppData\Roaming\Python\Python313\site-packages\itchat\utils.py", line 69, in msg_formatter
d[k] = htmlParser.unescape(d[k])
^^^^^^^^^^^^^^^^^^^
AttributeError: 'HTMLParser' object has no attribute 'unescape'
```
解决方法是打开`C:\Users\你自己的用户名\AppData\Roaming\Python\Python313\site-packages\itchat\utils.py`文件（即报错信息中的路径）：
- 在代码开头引用部分添加`import html`
- 找到第69行（即报错内容的行数），把`d[k] = htmlParser.unescape(d[k])`改成`d[k] = html.unescape(d[k])`后保存文件即可。

# 运行代码
## 1.运行itchat_item.py文件
```
python itchat_item.py
```
此处我尝试使用vscode右上角的三角形运行，但是失败了，不知道为什么命令行运行就可以……？

## 2.扫描二维码登录
注意一定要快速！像抢票一样！这个二维码的刷新超级无敌快。。。
不过登录成功一次后，下次运行就不用登录啦~
所以多试几遍就好嗯嗯，，，

## 3.开始对话
命令行弹出以下对话的时候就代表已经在工作了：
```bash
Login successfully as 示
Start auto replying.
```
想要退出的话，按下Ctrl+C即可。