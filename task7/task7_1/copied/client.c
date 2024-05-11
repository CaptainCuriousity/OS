#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include "client.h"

const int8_t ERR_FLAG = -1;

void serverInit(Server *serv){

    printf("Enter hostname: ");
    scanf("%s", serv->hostname);
    printf("\n");
    printf("port: ");
    scanf("%d", &(serv->port));
    printf("\n");
    memset(&serv->address, 0, sizeof(serv->address));
    serv->address.sin_family = AF_INET;
    inet_pton(AF_INET, serv->hostname, &(serv->address.sin_addr));
    serv->address.sin_port = htons(serv->port);
}

int32_t createSocket(){
    int32_t UDPSocket;
    if ((UDPSocket = socket(PF_INET, SOCK_DGRAM, 0)) == ERR_FLAG) {
        if(errno == EACCES) perror("Permission to create a socket of the specified type and/or protocol is denied\n");
        if(errno == EINVAL) perror("Unknown protocol,or protocol family not available\n");
        if((errno == ENOBUFS) || (errno == ENOMEM)) perror("Insufficient memory is available.  The socket cannot be created until sufficient resources are freed\n");
        if(errno == EAFNOSUPPORT) perror("The implementation does not support the specified address family\n");
        return ERR_FLAG;
    }   
    return UDPSocket;
}

ssize_t sendData(int *UDPsocket, Server *UDPServer){
    int8_t msg[100];
    printf("Enter msg: ");
    scanf("%s", msg);
    printf("\n");
    ssize_t len = sendto(*UDPsocket, msg, strlen(msg), 0, (struct sockaddr*)&(UDPServer->address), sizeof(UDPServer->address));
    return len;
}

void recvData(int *UDPsocket, int length, Server *UDPServer){
    char buffer[100];
    buffer[length] = '\0';
    printf("received: '%s'\n", buffer);
}
