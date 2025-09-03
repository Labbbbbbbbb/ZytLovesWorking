## Docker Daemon 代理

Docker Daemon 是通过 systemd 管理的，因此需要在 systemd 配置文件中添加代理设置。

- 创建 systemd 配置目录：

  ```
  sudo mkdir -p /etc/systemd/system/docker.service.d
  ```

- 创建代理配置文件：

  ```
  sudo touch /etc/systemd/system/docker.service.d/http-proxy.conf
  ```

- 编辑配置文件，添加以下内容：

  ```
  [Service]
  
  Environment="HTTP_PROXY=http://192.168.5.34:7890"
  
  Environment="HTTPS_PROXY=http://192.168.5.34:7890"
  
  Environment="NO_PROXY=localhost,127.0.0.1:7890"
  ```

- 重新加载 systemd 配置并重启 Docker：

```
sudo systemctl daemon-reload
sudo systemctl restart docker
```

- 验证配置是否生效：

  ```
  sudo systemctl show --property=Environment docker
  ```

  



## Q

```
Error response from daemon: Get "https://registry-1.docker.io/v2/": proxyconnect tcp: dial tcp 192.168.5.34:7890: connect: connection refused
或
Error response from daemon: Get "https://registry-1.docker.io/v2/": net/http: request canceled while waiting for connection (Client.Timeout exceeded while awaiting headers)
```

死因：

由于之前不知道摁到防火墙的什么东西了导致后面其实ping本机都ping不通

另外，远程控制服务器走本机的代理（使用 Clash for Windows 为局域网其他设备提供代理服务）只需要打开clash的`Allow LAN`模式即可，远程走本机的流量可以在连接中看到

![image-20250830004138187](F:\ITS_Log\docker\assets\image-20250830004138187.png)