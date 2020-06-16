# AccountManager
一款基于Raft分布式共识算法和C++编程语言的电子账本管家项目程序

## 快速开始
### 安装说明
运行“安装包3.0.exe”进行安装。（ps.安装目录建议不要出现中文）

### 运行说明
1. 建议显示屏分辨率调到125%及以下
2. 安装后桌面会有两个快捷方式。先点击"start raft"，运行服务器（会自动开启四个）。待有一个服务器出现"I'm a Leader"后再运行客户端程序"Account Manager"（建议客户端不要开太多个，除非对电脑很有信心hh）
3. 由于是单机模拟运行多个服务器，因此运行时占用CPU可能较高，若有略微卡顿，请理解

### 程序说明
1. 程序支持以下功能：注册新用户、用户登录、余额查询、个人相关流水查询、向其他用户转账。
2. 注册用户时密码长度大于6位，支付密码与登录密码相同。
3. 每次新注册用户账本中的默认金额为1000元。

## Raft服务器代码部分
### 更新说明
1. 将raft与审核端整合为多线程运行
2. 完成了raft与审核端的通信，审核传入的数据可以由raft同步机制进行各节点之间的账本的同步

### 运行说明
运行时使用事先编写好的windows bat 脚本进行多个服务器节点的创建
1. 首先运行Debug文件夹下的clear脚本，清空通信组的network文件夹，防止network中的历史文件影响到正常通信,当出现通信错误时切记先保留network文件夹（现在默认为C盘下），再运行clear。
2. 运行start脚本开启多个服务器节点
3. 想要关闭服务器则运行kill脚本，自动关闭所有打开的服务器程序

### 调试说明
1. 运行结果将会保留指定的文件中，Raft日志每次将在当前目录下以服务器id为名新建文本文件，
将raft相关的日志文件写入其中，如果发现同步出现错误时可以查看该文件以判断错误类型
2. 每次运行服务器程序时，对于每一个服务器节点都将会在当前目录下新建User_id文件夹，其中
的users.txt中将会保存注册后的用户名单，users文件夹内将会保存每一个用户的当前账目信息

### 重要变量说明
1. raft_type.h中的 NODE_TOTAL为服务器节点个数，需要修改服务器节点个数时对该变量进行修改并
修改start脚本中开启的程序个数
2. 主程序中只需将check_thread（）内的函数名修改为修改后的check线程程序即可实现多线程
3. 多线程通信涉及的全局变量置于Global.h中，对于全局变量的修改统一使用Global.h中定义的针对每
一个全局类型定义的read_Guard()与write_Guard()函数

## 客户端代码部分
详见QTUI分支

## 待解决的问题
1. 服务器中途关闭，虽然能够重新选出Leader，但是和客户端的通信有概率出错。
2. 后台服务器在接收到改动之后可以向客户端发送消息，避免客户端每隔一段时间向服务器询问。
3. 未考虑服务器数量的增减，维护较困难
4. 需制定合适的外部资金汇入机制
5. 目前仅为单机测试，若有时间可以实现联机测试