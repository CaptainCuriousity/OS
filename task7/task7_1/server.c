#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>

#include "server_constants.h"

int main() {
	int srv_sock, clt_sock, len = 0, err;
	struct sockaddr_in srv_sockaddr;
	struct sockaddr_in clt_sockaddr;

	signal(SIGPIPE, SIG_IGN);

	srv_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (srv_sock == -1) {
		perror("Socket failed");
		return EXIT_FAILURE;
	}

	memset(&srv_sockaddr, 0, sizeof(struct sockaddr_in));
	srv_sockaddr.sin_family = AF_INET;
	// INADDR_ANY means that i listen to all interfaces
	// htonl means host to network long:
	srv_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// htons means host to network short (port is two bytes long, up to 65535)
	srv_sockaddr.sin_port = htons(PORT);

	if (bind(srv_sock, (struct sockaddr *)&srv_sockaddr, sizeof(srv_sockaddr)) == -1) {
		perror("Bind failed");
		close(srv_sock);
		return EXIT_FAILURE;
	}
	
	while (1) {
		char buff[128];
		for (int i = 0; i < 128; ++i) {
			buff[i] = '\0';
		}
		
		len = sizeof(clt_sockaddr);
		err = recvfrom(srv_sock, buff, sizeof(buff), 0,
			       	(struct sockaddr*)&clt_sockaddr, &len);
		if (err == -1 || err == 0) {
			perror("Read failed");
			break;
		}
		
		printf("Message reveived: %s\n", buff);

		err = sendto(srv_sock, buff, sizeof(buff), 0,
			       	(struct sockaddr*)&clt_sockaddr, sizeof(clt_sockaddr));
		if (err == -1 || err == 0) {
			perror("Write failed");
			break;
		}
	}

	close(srv_sock);

	return EXIT_SUCCESS;
}
