#include "server.h"
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

const int32_t SERVER_PORT = 2222;
const int32_t MAX_SIZE = 100;

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
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = htonl(INADDR_ANY);
    addr->sin_port = htons(SERVER_PORT);
}

int32_t connection(int32_t TCPSocket, struct sockaddr_in addr, struct sockaddr_in client){
    if ((bind(TCPSocket, (struct sockaddr*)&addr, sizeof(addr))) == ERR_FLAG) {
        if(errno == EACCES) perror("The address is protected, and the user is not root\n");
        if(errno == EBADF ) perror("Invalid file descriptor\n");
        if(errno == EINVAL) perror("Permission to create a socket of the specified type and/or protocol is denied\n");
        if(errno == ENOTSOCK) perror("The file descriptor does not refer to a socket\n");
        if(errno == EFAULT) perror("addr points outside the user's accessible address space\n");
        if(errno == ELOOP) perror("Too many symbolic links were encountered in resolving addr\n");
        if(errno == ENAMETOOLONG) perror("addr is too long\n");
        if(errno == ENOENT) perror("A component in the directory prefix of the socket pathname does not exist\n");
        if(errno == ENOMEM) perror("Insufficient kernel memory was available\n");
        if(errno == EROFS) perror("The socket inode would reside on a read-only filesystem\n");
    }
    if ((listen(TCPSocket, 5)) != 0) {
        if(errno == EADDRINUSE) perror("Another socket is already listening on the same port\n");
        if(errno == EBADF) perror("The first argument is not a valid file descriptor\n");
        if(errno == ENOTSOCK) perror("The file descriptor does not refer to a socket\n");
        if(errno == EOPNOTSUPP) perror("The socket is not of a type that supports the listen() operation\n");
        exit(ERR_FLAG);
    }
    uint32_t length = sizeof(client);
    int32_t conn = accept(TCPSocket, (struct sockaddr*)&client, &length);

    if(connect < 0){
        if(errno == EBADF) perror("The first argument is not a valid file descriptor\n");
        if(errno == ECONNABORTED) perror("A connection has been aborted\n");
        if(errno == EFAULT) perror("The addr argument is not in a writable part of the user address space\n");
        if(errno == EINTR) perror("The system call was interrupted by a signal that was caught before a valid connection arrived\n");
        if(errno == EINVAL) perror("TSocket is not listening for connections, or addrlen is invalid\n");
        if(errno == EMFILE) perror("TThe per-process limit on the number of open file descriptors has been reached\n");
        if(errno == ENOTSOCK) perror("The file descriptor does not refer to a socket\n");
        if(errno == EOPNOTSUPP) perror("The referenced socket is not of type SOCK_STREAM\n");
        if(errno == EPROTO) perror("Protocol error\n");
        if((errno == ENOBUFS)||(errno == ENOMEM)) perror("Not enough memory\n");
        exit(ERR_FLAG);
    }
    return conn;
}

void startServer(int32_t conn, struct sockaddr_in client){
    int8_t buf[MAX_SIZE];
    int32_t n;
    bzero(buf, MAX_SIZE);
    read(conn, buf, sizeof(buf));
    printf("received msg: '%s' from %s\n", buf, inet_ntoa(client.sin_addr));
    write(conn, buf, sizeof(buf));
    printf("sended to %s\n", inet_ntoa(client.sin_addr));
}
