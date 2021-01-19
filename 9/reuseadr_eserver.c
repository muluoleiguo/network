#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
void error_handling(char* message);

int main(int argc,char* argv[])
{
    int serv_sock,clnt_sock;
    char message[20];
    int option,str_len;
    socklen_t optlen,clnt_adr_sz;
    struct sockaddr_in serv_adr,clnt_adr;
    if(argc!=2)
    {
        printf("<port>");
        exit(1);
    }

    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1)
        error_handling("socket() e");
    optlen = sizeof(option);
    option = TRUE;
    setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,(void*)&option,optlen);
    //略过
}
void error_handling(char* message)
{
    fputs(message,stderr);
    exit(1);
}
