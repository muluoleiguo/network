#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc,char* argv[])
{
    int serv_sock,clnt_sock;
    char buf[BUF_SIZE];
    int read_cnt;
    FILE* fp;

    struct sockaddr_in serv_adr,clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc!=2)
    {
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    fp = fopen("file_server.c","rb");

    serv_sock =socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock,5)==-1)
        error_handling("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
    while(1)
    {
        read_cnt = fread((void*)buf,1,BUF_SIZE,fp);
        if(read_cnt<BUF_SIZE)
        {
            write(clnt_sock,buf,read_cnt);
            break;
        }
        write(clnt_sock,buf,BUF_SIZE);
    }
    shutdown(clnt_sock,SHUT_WR);
    read(clnt_sock,buf,BUF_SIZE);
    printf("Message form client: %s \n",buf);
    fclose(fp);


    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

