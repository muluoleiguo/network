#include<stdio.h>
#include<fcntl.h>
#define BUF_SIZE 3 //最短数组长度

int main(int argc,char* argv[])
{
    FILE* fp1,*fp2;
    char buf[BUF_SIZE];

    fp1=fopen("news.txt","r");

    fp2=fopen("cpy.txt","w");
    //fd 中保存文件描述符

    while(fgets(buf,BUF_SIZE,fp1)!=NULL)
        fputs(buf,fp2);

    close(fp1);
    close(fp2);
    return 0;
}
