# 简单版本，只会重复，未调用chatgpt的api，用来检测是否能正常登录和回复
from itchat.content import TEXT
import itchat

# 登录微信
itchat.auto_login(hotReload=True)

# 扫描二维码登录
@itchat.msg_register(TEXT)
def text_reply(msg):
    
    if msg['Text'] == '你好':
            return '你好，有什么可以帮助你的吗？'
    elif msg['Text'] == '你是谁':
            return '我是一个微信机器人'
    else:
            return '我猜你要说:%s' % msg['Text']

# 开始监听和自动回复
itchat.run()