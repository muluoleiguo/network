TCP ：transmission control protocol 对数据传输过程的控制

**TCP服务器端默认的函数调用顺序**

![image-20210118124218011](C:\Users\55018\AppData\Roaming\Typora\typora-user-images\image-20210118124218011.png)

**3进入等待连接请求状态**

已经由bind（）给套接字分配律地址，调用listen函数后客户端才进入可发出连接请求的状态，即这时客户端才可以调用connect函数

```c
#include<sys/socket.h>
int listen(int sock,int backlog);
//成功0，失败-1
```

sock：希望进入等待连接请求状态的套接字文件描述符，传递的描述符套接字参数成为服务器端套接字（监听套接字）

backlog：连接请求等待队列（queue）的长度，最多让几个连接请求进入队列



客户端请求连接时，受理连接前一直等待咯

注意客户端连接请求本身也是从网络中接收到的一种数据，而想要接收就需要套接字。由服务器端套接字完成，这就像是门卫。



调用listen生成服务器端套接字，backlog决定了等候室的大小



**4.受理客户端连接请求**

调用listen函数后，如果有新的连接请求，则应该按序受理。受理请求意味着进入可接受数据的状态。进入这种状态所需当然还是套接字，但不是用服务器端套接字，需要另外一个由accept函数自动创建的套接字。

```c
#include<sys/socket.h>
int accept(int sock,struct sockaddr* addr,socklen_t* addrlen);
//成功返回创建的套接字的文件描述符，失败返回-1
```

sock：服务器套接字文件描述符

addr：保存发起连接请求的客户端地址信息的变量的地址值，调用函数后向传递来的地址变量参数填充客户端地址信息

addrlen 第二个参数addr结构体长度，但存入长度变量地址

accept调用成功时，内部将产生用于数据I/O的套接字，并自动与发起连接请求的客户端建立连接

![image-20210118134306938](C:\Users\55018\AppData\Roaming\Typora\typora-user-images\image-20210118134306938.png)

调用accept函数时若等待队列为空，则accept函数不会返回，直到队列中出现新的客户端连接



**TCP客户端的默认函数调用顺序**

![image-20210118135121182](C:\Users\55018\AppData\Roaming\Typora\typora-user-images\image-20210118135121182.png)

发起连接请求

```c
#include<sys/socket.h>
int connect(int sock,struct sockaddr* servaddr,socklen_t addrlen);
//成0败-1
```

sock：客户端套接字文件描述符

servaddr：保存目标服务器端地址信息的变量地址值

addrlen：以字节为单位传递 已传递给第二个结构体参数servaddr的地址变量长度



调用connect后，发生下列情况才算是完成函数调用

1、服务器端**接受连接**请求

2、发生断网等导致中断连接请求



**接受连接**并不是服务器端调用accept函数，而是服务区端把连接请求信息记录到等待队列。因此connect函数返回后并不立即进行数据交换



客户端在调用connect函数时分配地址在操作系统内核里，IP用计算机的IP，端口随机，无需调用标记的bind函数进行分配。



**基于TCP的服务器端和客户端函数调用关系**

![image-20210118140912344](C:\Users\55018\AppData\Roaming\Typora\typora-user-images\image-20210118140912344.png)

在客户端调用connect前，服务器端如果先调用accept，则进入阻塞状态，直至客户端调用connect为止



**实现迭代**

![image-20210118141414405](C:\Users\55018\AppData\Roaming\Typora\typora-user-images\image-20210118141414405.png)

服务器端函数调用顺序：插入循环反复调用accept



close并非针对服务器端套接字，而是针对accept函数调用时创建的套接字

调用close函数意味结束了针对某一客户端的服务，如果想要再次服务，则需要重新调用accept函数。

注意现在**同一时刻，只能服务于一个客户端**，之后多线程再说其他的。



**回声echo程序**

服务器端在同时刻只与一个客户端连接并提供回声服务

服务器端依次向5个客户端提供服务并退出

客户端接受用户输入的字符串并发送到服务器端

服务器端将接受的字符串数据传回客户端，即echo

服务器端与客户端之间的字符串回声一直执行到客户端输入Q为止

