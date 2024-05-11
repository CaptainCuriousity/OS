#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

int main() {
	int srv_sock;
	int clt_sock;
	
	struct sockaddr_un srv_sockaddr;
	struct sockaddr_un clt_sockaddr;

	int len = 0;
	int val = 0;

	char* dsock_file = "./dsock";
	
	memset(&srv_sockaddr, 0, sizeof(struct sockaddr_un));
	srv_sockaddr.sun_family = AF_UNIX;
	strcpy(srv_sockaddr.sun_path, dsock_file);

	signal(SIGPIPE, SIG_IGN);
	
	// what 0 means here?
	clt_sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (clt_sock == -1) {
		perror("Socket failed");
		return EXIT_FAILURE;
	}

	if (connect(clt_sock, (struct sockaddr *)&srv_sockaddr, sizeof(srv_sockaddr)) == -1) {
		perror("Connect failed");
		return EXIT_FAILURE;
	}
	
	while (1) {
		int ret;
		char buff[128];

		scanf("%s", &buff);

		ret = write(clt_sock, buff, sizeof(buff));
		if (ret == -1 || ret == 0) {
			printf("Read failed");
			close(clt_sock);
			return EXIT_FAILURE;
		}

		ret = read(clt_sock, buff, sizeof(buff));
		if (ret == -1 || ret == 0) {
			printf("Write failed");
			close(clt_sock);
			return EXIT_FAILURE;
		}

		printf("%s\n", buff);
	}

	return EXIT_SUCCESS;
}
