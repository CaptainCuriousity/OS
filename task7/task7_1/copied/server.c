#include "server.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int32_t SERVER_PORT = 22;
int8_t ERR_FLAG = -1;

int32_t createSocket(){
    int UDPSocket;
    if ((UDPSocket = socket(PF_INET, SOCK_DGRAM, 0)) == ERR_FLAG) {
        if(errno == EACCES) perror("Permission to create a socket of the specified type and/or protocol is denied\n");
        if(errno == EINVAL) perror("Unknown protocol,or protocol family not available\n");
        if((errno == ENOBUFS) || (errno == ENOMEM)) perror("Insufficient memory is available.  The socket cannot be created until sufficient resources are freed\n");
        if(errno == EAFNOSUPPORT) perror("The implementation does not support the specified address family\n");
        return ERR_FLAG;
    }
    return UDPSocket;
}

int32_t listenConnections(int32_t UDPSocket, struct sockaddr_in addr){
    if ((bind(UDPSocket, (struct sockaddr *)&addr, sizeof(addr))) == ERR_FLAG) {
        if(errno == EACCES) perror("The address is protected, and the user is not root\n");
        if(errno == EADDRINUSE) perror("The given address is already in use\n");
        if(errno == EBADF ) perror("Permission to create a socket of the specified type and/or protocol is denied\n");
        if(errno == EINVAL) perror("Permission to create a socket of the specified type and/or protocol is denied\n");
        if(errno == ENOTSOCK) perror("Permission to create a socket of the specified type and/or protocol is denied\n");
        if(errno == EFAULT) perror("Permission to create a socket of the specified type and/or protocol is denied\n");
        if(errno == ELOOP) perror("Permission to create a socket of the specified type and/or protocol is denied\n");
        if(errno == ENAMETOOLONG) perror("Permission to create a socket of the specified type and/or protocol is denied\n");
        if(errno == ENOENT) perror("TA component in the directory prefix of the socket pathname does not exist\n");
        if(errno == ENOMEM) perror("Insufficient kernel memory was available\n");
        if(errno == EROFS) perror("The socket inode would reside on a read-only filesystem\n");
        return ERR_FLAG;
    }
    return 0;
}

int32_t startServer(){
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int32_t UDPSocket = createSocket();
    if(UDPSocket == ERR_FLAG) return ERR_FLAG;
    if(listenConnections(UDPSocket, addr) == ERR_FLAG) return ERR_FLAG;
    struct sockaddr_in client;
    socklen_t clientLength = 0;

    while (1) {
        int8_t buf[4096];

        int32_t len = recvfrom(UDPSocket, buf, sizeof(buf), 0,(struct sockaddr *)&client, &clientLength);
        buf[len] = '\0';
        printf("recieved msg: '%s' from %s\n", buf, inet_ntoa(client.sin_addr));

        sendto(UDPSocket, buf, len, 0, (struct sockaddr *)&client, sizeof(client));
        printf("sended to %s\n", inet_ntoa(client.sin_addr));
    }

    return 0;
}
