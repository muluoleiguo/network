#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main( int argc, char* argv[])
{
    if( argc <= 2 )
    {
        printf( "usage: %s ip_address port _number\n", basename( argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2]);
    struct sockaddr_in server_address;
    bzero( &server_address, sizeof( server—address ));
    server_address.sin_family = AF_INET;
}
