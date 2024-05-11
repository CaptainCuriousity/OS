#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include "client.h"

int32_t createSocket(){
    int TCPSocket;
    if ((TCPSocket = socket(AF_INET, SOCK_STREAM, 0)) == ERR_FLAG) {
        if(errno == EACCES) perror("Permission to create a socket of the specified type and/or protocol is denied\n");
        if(errno == EINVAL) perror("Unknown protocol,or protocol family not available\n");
        if((errno == ENOBUFS) || (errno == ENOMEM)) perror("Insufficient memory is available.  The socket cannot be created until sufficient resources are freed\n");
        if(errno == EAFNOSUPPORT) perror("The implementation does not support the specified address family\n");
        return ERR_FLAG;
    }
    return TCPSocket;
}

void assignPort(struct sockaddr_in *addr){
    int32_t port;
    int8_t hostname[16];
    printf("Enter hostname: ");
    scanf("%s", hostname);
    printf("\n");
    printf("port: ");
    scanf("%d", &port);
    printf("\n");
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(hostname);//127.0.0.1
    addr->sin_port = htons(port);
}

void connection(struct sockaddr_in addr, int32_t TCPSocket){
    if (connect(TCPSocket, (struct sockaddr*)&addr, sizeof(addr)) == ERR_FLAG) {
        if(errno == EBADF) perror("The first argument is not a valid file descriptor\n");
        if(errno == ECONNABORTED) perror("A connection has been aborted\n");
        if(errno == EFAULT) perror("The addr argument is not in a writable part of the user address space\n");
        if(errno == EINTR) perror("The system call was interrupted by a signal that was caught before a valid connection arrived\n");
        if(errno == EINVAL) perror("Socket is not listening for connections, or addrlen is invalid\n");
        if(errno == EMFILE) perror("TThe per-process limit on the number of open file descriptors has been reached\n");
        if(errno == ENOTSOCK) perror("The file descriptor does not refer to a socket\n");
        if(errno == EOPNOTSUPP) perror("The referenced socket is not of type SOCK_STREAM\n");
        if(errno == EPROTO) perror("Protocol error\n");
        if(errno == ENODATA) perror("No data available\n");
        if(errno == ECONNREFUSED) perror("Connection refused\n");
        if((errno == ENOBUFS)||(errno == ENOMEM)) perror("Not enough memory\n");
        exit(ERR_FLAG);
    }
}

void echo(int32_t TCPSocket){
    char buf[MAX_SIZE];
    int n;
    bzero(buf, sizeof(buf));
    printf("Enter msg: ");
    scanf("%s", buf);
    printf("\n");
    write(TCPSocket, buf, sizeof(buf));
    bzero(buf, sizeof(buf));
    read(TCPSocket, buf, sizeof(buf));
    printf("received: '%s'\n", buf);
}
