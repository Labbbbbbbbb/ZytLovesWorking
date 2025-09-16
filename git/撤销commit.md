如果已经提交了更改但尚未将其推送到远程仓库，（尤其是有时一手痒commit了发现提交了一些大文件push不上去）使用以下步骤取消取消：

**HEAD^ 表示上个版本,HEAD~2表示上上个版本miced commitId**

或者直接写上你想回到的提交节点即可

**1. 查看日志** 

　　**git log**

**2.** **此时如果想撤销commit，同时保留git add**

　　**git reset --soft HEAD^**

**3. 删除工作空间改动代码，撤销commit，撤销git add**

　　**git reset --hard HEAD^
**

**4. 不删除工作空间改动代码，撤销commit，并且撤销git add(常用)**

　　**git reset --mixed HEAD^ 或者git reset HEAD^
**

**5. 如果只想修改下git commit 的注释内容**

　　**git commit --amend**

**6. 还没git commit ，只撤销git add，此时会保留本地修改（绿字变红字）**

　　**git reset HEAD filename**

　　**全部：git reset HEAD**

**7. 不想保留本地修改**

　　**git checkout filename**