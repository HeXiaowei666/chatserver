# 数据库
user     = debian-sys-maint
password = JF1qW9Pyks2lKn1x

update mysql.user set authentication_string=password('123456') where user='root' and host='localhost'; 低版本Mysql
ALTER USER 'root'@'localhost' IDENTIFIED BY '123456';
update mysql.user set plugin="mysql_native_password";
flush privileges;

mysql -u root -p; 
show variables like "char%";
set character_set_server=utf8;

sudo netstat -tanp 

ERROR 1449 (HY000): The user specified as a definer ('mysql.infoschema'@'localhost') does not exist

创建表 创建库
表的设计范式 表的设计原则 有一门课程
实体与实体 一对一 一对多 多对多 多张表
sql脚本 一键执行创建这几张表 在mysql中如何执行脚本？
依据功能来进行设计的

# 搭建工程
编译命令流程：终端命令还是不知道 用的vscode插件的CMake按钮

# 具体业务
我想起了自己定义网络协议 吕大师的代码 cs结构 就是管理系统删除保存增加那个代码
绑定器具体语法 为什么出现
解耦
ORM  业务层操作的都是对象  DAO 
分层 数据与业务分离 

写类时，最后的分号总是忘记
文件名: usermodel.cpp   类名：class UserModel

error: could not load cache ?
不是什么错误，就是cmake编译时候无法识别，我重新进入CMakeLists.txt 空格保存哈 有改动即可 进入重新编译

undefined reference to `UserModel::updateState(User)'
[build] collect2: error: ld returned 1 exit status ?
原因是cpp文件中，函数名没有加类名。bool updateState(User user) 
bool UserModel::updateState(User user).

bug: 测试时，type must be number, but is string 
gdb ChatServer 调试 忘记语法了都
bin/目录下 gdb ChatServer
break chatservice.cpp:41 
run 
然后客户端登录 telnet 127.0.0.1 6000
p id 
n  然后出现之前的错误提示了
发现问题：客户端测试时传参 {"msgid":1, "id":"15" , "password":"123456"}
把15的双引号去掉
收获：看完整的gdb发现一个问题时的过程，体会debug

之前面试：线上如何调试代码呢 
因为数据库没有创建表，所以无法测试代码 

# 分析聊天业务
客户1 
客户2 ： 它不可能是去服务器拉聊天信息，只能是服务器推送给它，所以要使用长连接 
服务器

# 线程安全
c++ stl容器不具备线程安全 需要自己操作 
数据库的线程安全 由Mysql server保证的

{
    lock_guard<mutex> lock(_connMutex);
    _userConnMap.insert({id, conn});  // c++ stl容器没有考虑线程安全的问题 这样写肯定不行 
}
线程安全你进来就加个锁 出去就解锁 多线程就变成串型的了 没有体现并发编程的优势了
所以锁的力度一定要小，不能扩大。
利用{}构建一个局部作用域，智能锁，即可构建一个小的锁

# 一对一聊天业务
msgid 
id: 1
from: "zhang san"
to: 3
msg: "xxxx"
看你是否在线，不在线就把消息放在离线信息表中

ascii码和utf-8有点搞不清楚

# 离线消息业务
undefined reference to `OfflineMsgModel::insert()
[build] collect2: error: ld returned 1 exit status？

# 服务端异常退出 
# 添加好友功能 
select a.id, a.name, a.state 
from user a 
inner join friend b on b.friendid = a.id 
where b.userid=%d;

# 群组业务 
创建群 加入群 群聊
频繁请求数据库 释放数据库 耗费资源的 所以使用连接池 以及优化sql
能一次请求数据完，最后一次，尽可能少，毕竟数据库磁盘操作速度慢
写sql是非常讲究的 能把复杂sql写好 提升效率很明显的

简单业务 复杂业务 业务是很灵活的 
我们做c++的 要把整个软件设计框架的流程搞清楚 
业务不是不是很重要

面试问你 这个表数据量有多少 几万级即可 如果是几百万则涉及很多其他东西了——表的优化 表拆分
几百万 就会问你数据库的优化了 

mode文件夹 存放的是数据库操作的封装
业务层 数据层 不能混在一起
网络层的代码实现 服务层的代码实现


# 客户端 
客户端传给服务器什么协议 服务端就解析什么
一切从简 用的也是linux tcp

业务是没有尽头的，提什么需求，就开发什么业务
基于终端的客户端

只收到一条离线消息？
设计表的时候把离线消息的 设计成了主键 导致不能重复 修改成not null 
日志很重要，方便看bug 

bug: 重新登录拉了两遍朋友列表？
使用容器之前先要保证是空的，否则就会叠加

# 集群服务器
单台server linux文件描述符调到最大 32位下 最多2万多并发
搞成集群后，多台服务器，但是客户哪知道你哪台服务器啊，每次连接不同服务器麻烦，所以需要一个帮手管理这些服务器
引入：负载均衡器 
    1.把client的请求按照负载算法(轮询 权重 哈希)分发到具体的业务服务器ChatServer上面
    2.能够和ChatServer保持（心跳机制），监测ChatServer故障
    3.能够发现新添加的ChatServer设备，方便扩展服务器数量

选择nginx的tcp负载均衡模块： (轻轻松松5、6W)
    1.如何进行nginx源码编译，包含tcp负载均衡模块
    2.nginx.conf配置文件包含如何配置负载均衡
    3.nginx的平滑加载配招文件启动

更大并发量：负载均衡器也可以集群 前端再挂一个LVS(也常用)
LVS是一个相对偏底层的负载均衡器
负载均衡：有偏业务层的，有传输层的，数据帧来分发的
LVS很容易扩展到十几万 并发量 

如何做到一百万并发呢？好奇 有哪些技术 

聊天服务器属于长连接的业务
有两种：
  1.通过负载均衡器，传输服务器的ip，然后客户端和服务器建立一个Ip隧道 
  2.数据传输都要经过 负载均衡器

client1                         CharServer1 192.168.10.110 client1
client2         负载均衡器       CharServer2 192.168.10.156 client2
client3                         CharServer3 192.168.10.179 client3

两个问题：
1.ChatServer集群后怎么引入负载均衡器
2.如何解决跨服务器通信问题 client1与client2通信？

让各个ChatServer服务器互相之间直接建立TCP连接进行通信，相当于在服务器网络之间进行广播。
这样的设计使得各个服务器之间耦合度太高，不利于系统扩展，并且会占用系统大量的socket资源，各服务器之间的
带宽压力很大，不能够节省资源给更多的客户端提供服务，因此绝不是一个好的设计。

集群部署的服务器之间进行通信，最好的方式就是引入中间件消息队列，解耦各个服务器，使整个系统松耦合，
提高服务器的相应能力，节省服务器的带宽资源。（分层 即增加一层）

charserver1 charserver2 charserver3
    基于发布-订阅的redis消息队列
charserver4 charserver5 charserver6


在集群分布式环境中，经常使用的中间件消息队列有ActiveMQ RabbitMQ Kafka等，都是应用场景广泛并且性能较好的消息队列，
供集群服务器之间，分布式服务之间进行消息通信。限于我们的项目业务类型并不是非常复杂，对并发请求量没有太高的要求，
因此我们的中间件消息队列选型的是 基于发布-订阅模式的redis

redis还可以作为持久化的数据库
Kafka配置相对复杂一些 处理大型应用几十万的并发场景

publish 
subscribe
notify 
设计模式：观察者模式 

# nginx配置tcp负载均衡
官网下载包
解压：tar -axvf nginx-1.12.2.tar.gz 
你在Linux玩源码编译，永远都是：先执行configure生成makefile,再make,再make install 

rm -rf *
sudo apt update 为什么每次安装前都要执行一次呢

换源还是不行啊，无法下载 工作中遇到就拉闸 需要注意！

sbin 可执行程序位于此  conf nginx配置代码位于此
nginx.conf 增加代码
sudo su 
./nginx 
netstat -tanp 查看nginx是否启动 进程中杀nginx杀不死，有容错机制，杀一个重新生成一个
./nginx -s reload 
./nginx -s stop 

测试 
./ChatServer 127.0.0.1 6000 之前nginx.conf配置的两台服务器端口 ip 
./ChatServer 127.0.0.1 6002

我们连接8000端口 nginx 负载均衡 
./ChatClient 127.0.0.1 8000 然后看哪台服务器响应了
./ChatClient 127.0.0.1 8000 另外一台响应了
看什么分配算法 权重是weight 都是1 两台服务器则是50%比例 

# redis 
---把3岁入学计划哈 经济成本 时间成本
网段我不是很熟悉，尤其是不同网段之间的连接，和本网段的连接的关系

mysql 3306 
redis 6379 这样经典的程序都有自己的端口 

redis数据在内存中存储的，快速 mysql是硬盘中 

ps -ef | grep redis 

redis-cli  客户端登录 
set "abc" "hello world"
get "abc"
存的键值对  还可以存数组 链表 集合 映射表   

有些项目放弃使用mysql，使用缓冲数据库redis做数据存储，内存中效率高
也可以做数据持久化存储，不用担心redis重启之后数据找不到了，有rgb anf两种数据持久化方式

redis的一个功能：基于发布-订阅的消息队列 
发布 订阅  
redis-cli 
subscribe 13  阻塞的方式，等待上报信息 

redis-cli 
publish 13 "hello world"
(subscribe的地方收到消息 "13" "hello world")

redis发布-订阅的客户端编程 
支持多种语言，java-jedis c++-hiredis 
git clone https://github.com/redis/hiredis 
cd hiredis
make 
sudo make install   拷贝生成的动态库到/usr/local/lib目录下
sudo ldconfig /usr/local/lib  

quit / exit 退出redis

引出问题，linux下 git clone github仓库的包 
重在理解，逻辑理解清楚。干什么用的一定先搞清楚。
https://blog.csdn.net/qq_61692089/article/details/134042825 我这个是SSH 不是HTTPS 

任何基于长连接的服务器 想在集群环境中 跨服务器通信 都是这样的解决方案

/chat
/chat/build/
rm -rf * 
ls 
cmake ..
make 
cd ..
cd bin/
./ ChatServer 127.0.0.1 6000
手动，不是利用vscode的cmake工具

测试

负载均衡器 ——一致性哈希算法 
轮询 

# 推到github
对github不是很熟悉 
coderwhy git视频 学习 ！！！
把build中的所有文件删除了 谁拉代码谁去编译即可



