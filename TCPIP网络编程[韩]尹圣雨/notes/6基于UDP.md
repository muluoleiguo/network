UDP不可靠的数据传输服务，但UDP简洁性能高

TCP的生命在**控制流**

UDP最重要的作用就是根据端口号将传到主机的数据包交付给最终的UDP套接字



TCP比UDP慢的原因主要：

收发数据前后进行的连接设置以及清除过程

收发数据过程中为保证可靠性增加的流控制





UDP中服务器端和客户端没有连接，只有创建套接字的过程和数据交换过程



**客户端和服务器端都只需要1个套接字**

![image-20210118174237643](C:\Users\55018\AppData\Roaming\Typora\typora-user-images\image-20210118174237643.png)

基于UDP的数据i/o函数

UDP套接字不会保持连接状态，因此每次传输数据都要添加目标地址信息。

```c
#include<sys/socket.h>
ssize_t sendto(int sock,void* buff,size_t nbytes,int flags,struct sockaddr* to,socklen_t addrlen);
//成功返回传输的字节数，失败时返回-1
```



sock:传输数据的UDP套接字文件描述符

buff：保存待传数据的缓冲地址值

nbytes：待传输数据长度

flags：可选项参数

to:存有目标地址信息的sockaddr结构体变量的地址值

addrlen ：to 结构体变量长度



UDP数据的发送端并不固定，因此接收UDP数据的函数 定义为可接收发送端信息的形式

```c
#include<sys/socket.h>
ssize_t recvfrom(int sock,void*buff,size_t nbytes ,int flags,struct sockaddr* from,socklen_t* addrlen)
   //成 接受字节数 败-1
```

参数差不多

最核心的就是这俩函数



**回声**

UDP不存在请求连接和受理过程，因此无法明确区分服务器端和客户端



UDP客户端套接字的地址分配

**调用sendto函数时自动分配IP和端口号**



**UDP的数据传输特性和调用connect函数**

**UDP套接字存在数据边界**

调用几次输出函数发送的数据必须通过几次输入函数才能接受完



UDP数据报（datagram）

UDP1个数据包即可成为1个完整数据

**已连接UDP套接字和未连接UDP套接字**

sendto

1、向UDP套接字注册目标IP和端口号

2、传输数据

3、删除UDP套接字中注册的目标地址信息

每次调用sendto重复123，每次变更目标地址，因此可以重复利用同一UDP套接字向不同目标传输数据。这种未注册目标地址信息的套接字称为未连接套接字，反之则是连接套接字。显然UDP默认是未连接套接字

与同一主机长时间通信需要变成连接套接字



**创建已连接UDP套接字**

只需要针对UDP套接字调用connect函数。

这样看似就像TCP套接字创建过程一样，但是针对UDP套接字调用connect函数并不意味着要与对方UDP套接字连接，这只是向UDP套接字注册目标IP和端口号

之后更TCP套接字一样，每次调用sendto时只需传输数据，不仅仅可以用sendto和recvfrom。还可以使用write和read函数进行通信。

