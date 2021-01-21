实现I/O复用的传统方法有select函数和poll函数。

无论如何优化程序性能也无法同时接入上百个客户端（当然，硬件性能不同，差别也很大）。这种select方式并不适合以Web服务器端开发为主流的现代开发环境，所以要学习Linux平台下的epoll。

**select也有优点**

* 服务器端接入者少。
* 程序应具有兼容性。

**基于select的I/O复用技术速度慢的原因**

* 调用select函数后常见的针对所有文件描述符的循环语句。
* 每次调用select函数时都需要向该函数传递监视对象信息 



**仅向操作系统传递1次监视对象，监视范围或内容发生变化时只通知发生变化的事项**

前提是操作系统支持这种处理方式（每种操作系统支持的程度和方式存在差异）。Linux的支持方式是epoll，Windows的支持方式是IOCP。



**实现epoll时必要的函数和结构体**

优点

* 无需编写以监视状态变化为目的的针讨所有文件描述符的循环语句。
* 调用对应select函数的epoll_wait函数时无需每次传递监视对象信息。

需要的3个函数

* epoll_create：创建保存epoll文件描述符的空间。
* epoll_ctl：向空间注册并注销文件描述符，
* epoll_wait：与select函数类似，等待文件描述符发生变化。



select方式中为了保存监视对象文件描述符，直接声明了fd_set变量。但epoll方式下由操作系统负责保存监视对象文件描述符，因此需要向操作系统请求创建保存文件描述符的空间，此时使用的函数就是epoll_create。



此外，为了添加和删除监视对象文件描述符，select方式中需要FD_SET、FD_CLR函数。但在epoll方式中，通过epoll_ctl函数请求操作系统完成



最后，select方式下调用select函数等待文件描述符的变化，而epoll中凋用epoll_wait函数。



select方式中通过fd set变量查看监视对象的状态变化（事件发生与否），而epoll方式中通过如下结构体epoll_event将发生变化的（发生事件的）文件描述符单独集中到一起。

```c

struct epoll_event
{
    __uint32_t events;
    epoll_data_t data;
}

typedef union epoll_data
{
    void* ptr;
    int fd;
    __uint32_t u32;
    __uint64_t u64;   
}epoll_data_t
```

声明足够大的epoll_event结构体数组后，传递给epoll_wait函数时，发生变化的文件描述符信息将被填入该数组。



**epoll_create**

```c
#include<sys/epoll.h>
int epoll_create(int size);
//成功时返回epoll文件描述符
```

size:epoll实例的大小。

调用epoll_create函数时创建的文件描述符保存空间称为“epoll例程“

size并非用来决定epoll例程的大小，而仅供操作系统参考。



epoll_create函数创建的资源与套接字相同，也由操作系统管理。因此，该函数和创建套接字
的情况相同，也会返回文件描述符。也就是说，该函数返回的文件描述符主要用与于区分epoll
例程。需要终止时，与其他文件描述符相同，也要调用close函数。



**epoll_ctl**
生成epoll例程后，应在其内部注册监视对象文件描述符，此时使用epoll_ctl函数

```c
#include<sys/epoll.h>
int epoll_ctl(int epfd,int op,int fd,struct epoll_event* event)
```

epfd:用于注册监视对象的epoll例程的文件描述符。
op:用于指定监视对象的添加、删除或更改等操作。
fd:需要注册的监视对象文件描述符。
event:监视对象的事件类型。



`epoll_ctl(A, EPOLL_CTL_ADD, B,C)；`

“epoll例程A中注册文件描述符B,主要目的是监视参数C中的事件"

`epoll_ctl(A, EPOLL_CTL_DEL, B, NULL)`

"从epoll例程A中删除文件描述符B"



EPOLL_CTL_ADD：将文件描述符注册到epoll例程。
EPOLL_CTL_DEL:	从epoll例程中删除文件描述符。
EPOLL_CTL_MOD：更改注册的文件描述符的关注事件发生情况



epoll_even结构体用于保存发生事件的文件描述符集合

```c
struct epoll_event event;

//..
event.events = EPOLLIN	;
event.data.fd = sockfd;
epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&event);
//....
```

上述代码将sockfd注册到epoll例程epfd中



events中可以保存的常量及所指的事件类型:

EPOLLIN:需要读取数据的情况。

* EPOLLOUT：输出缓冲为空，可以立即发送数据的情况。
* EPOLLPRI:收到OOB数据的情况。
* EPOLLRDHUP:断开连接或半关闭的情况，这在边缘触发方式下非常有用。
* EPOLLERR：发生错误的情况。
* EPOLLET：以边缘触发的方式得到事件通知。
* EPOLLONESHOT：发生一次事件后，相应文件描述符不再收到事件通知。因此需要向
  epoll_ctl函数的第二个参数传递EPOLL_CTL_MOD,再次设置事件。



**epoll_wait**

epoll相关函数中默认最后调用该函数。

```c
#include<sys/epoll.h>
int epoll_wait(int epfd,struct epoll_event* event,int maxevents,int timeout);
```

epfd:表示事件发生监视范围的epoll例程的文件描述符。
events:保存发生事件的文件描述符集合的结构体地址值。**第二个参数所指缓冲需要动态分配**

maxevents:第二个参数中可以保存的最大事件数。
timeout:以1/1000秒为单位的等待时间，传递-1时，一直等待直到发生事件。



## 17.2条件触发和边缘触发

条件触发(Level Trigger )和边缘触发(Edge Trigger )，

**条件触发方式中，只要输入缓冲有数据就会一直通知该事件**

**边缘触发中输入缓冲收到数据时仅注册1次该事件。即使输入缓冲中还留有数据，也不会再进行注册。**