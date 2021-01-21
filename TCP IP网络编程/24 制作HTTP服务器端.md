![image-20210120203200026](C:\Users\55018\AppData\Roaming\Typora\typora-user-images\image-20210120203200026.png)

看完图解HTTP之后应该站在不同的角度才是

请求行含有请求方式（请求目的）信息。典型的请求方式有GET和POST, GET主要用于请求数据，POST主要用于传输数据。

“GET/index.html HTTP/1.1”：请求（GET） index.html文件，希望以1.1版本的HTTP协议进行通信

**请求行只能通过1行（line）发送**



请求行下面的消息头中包含发送请求的（将要接收响应信息的）浏览器信息、用户认证信息等关于HTTP消息的附加信息。最后的消息体中装有客户端向服务器端传输的数据

另外，消息体和消息头之间以空行分开，因此不会发生边界问题



**响应消息（Response Message）的结构**

![image-20210120203459065](C:\Users\55018\AppData\Roaming\Typora\typora-user-images\image-20210120203459065.png)

状态行中含有关于请求的状态信息

第一个字符串状态行中含有关于客户端请求的处理结果。例如，客户端请求index.html文件时，表示index.html文件是否存在、服务器端是否发生问题而无法响应等不同情况的信息将写人状态行。



* 200 OK：成功处理了请求！
* 404 Not Found:请求的文件不存在!
* 400 Bad Request:请求方式错误，请检查!



消息头中含有传输的数据类型和长度等信息。图中的消息头含有如下信息:“服务器端名为SimpleWebServer,传输的数据类型为text/html （ html格式的文本数据）。数据长度不超过2048字节。