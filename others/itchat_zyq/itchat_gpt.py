# 调用了itchat和openai的接口，实现了一个简单的微信聊天机器人
# 现在有点bug：会回复自己发出去的消息，也就是说会无限循环回复下去。。不过可以拿来玩玩
from itchat.content import TEXT
import itchat
import requests


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
        return ai_chat(msg)

def ai_chat(msg):
    from openai import OpenAI
    import os
    # 读取API KEY，把下面的地址换成你自己在openai申请的api的key存放地址
    with open("D:\Desktop\justplay\itchat\sk_apikey.txt", "r", encoding="utf-8") as f:
        API_KEY = f.read().strip()
    client = OpenAI(api_key=API_KEY)
    try:
        response = client.chat.completions.create(
            model="gpt-4.1-mini",
            messages=[{"role": "user", "content": msg['Text']}]
        )
        return response.choices[0].message.content
    except Exception as e:
        return f"[AI接口异常]: {e}"

# 开始监听和自动回复
itchat.run()