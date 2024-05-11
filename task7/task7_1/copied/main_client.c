#include <unistd.h>
#include "client.h"

int main() {
    Server UDPServer;
    serverInit(&UDPServer);

    int32_t UDPSocket = createSocket();
    ssize_t tmp = sendData(&UDPSocket, &UDPServer);
    recvData(&UDPSocket, tmp, &UDPServer);
    close(UDPSocket);
    return 0;
}
