#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server_constants.h"

int main() {
	int srv_sock;
	int clt_sock;
	
	struct sockaddr_in srv_sockaddr;
	struct sockaddr_in clt_sockaddr;

	int len = 0;
	int val = 0;

	memset(&srv_sockaddr, 0, sizeof(struct sockaddr_in));

	
	srv_sockaddr.sin_family = AF_INET;
	srv_sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srv_sockaddr.sin_port = htons(PORT);


	signal(SIGPIPE, SIG_IGN);
	
	clt_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (clt_sock == -1) {
		perror("Socket failed");
		return EXIT_FAILURE;
	}

	if (connect(clt_sock, (struct sockaddr *)&srv_sockaddr, sizeof(srv_sockaddr)) == -1) {

		return EXIT_FAILURE;
	}
	
	while (1) {
		int ret;
		char buff[BUFF_SIZE];

		scanf("%s", &buff);

		ret = write(clt_sock, buff, sizeof(buff));
		if (ret == -1 || ret == 0) {
			printf("Read failed");
			break;
		}

		ret = read(clt_sock, buff, sizeof(buff));
		if (ret == -1 || ret == 0) {
			printf("Write failed");
			break;
		}

		printf("%s\n", buff);
	}

	close(clt_sock);

	return EXIT_SUCCESS;
}
