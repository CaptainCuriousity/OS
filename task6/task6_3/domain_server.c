#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

#define CLIENTS_COUNT 5


int main() {
	int srv_sock;
	int clt_sock;
	
	struct sockaddr_un srv_sockaddr;
	struct sockaddr_un clt_socksaddr;

	int len = 0;
	int val = 0;

	char* dsock_file = "./dsock";

	signal(SIGPIPE, SIG_IGN);
	
	// what 0 means here?
	srv_sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (srv_sock == -1) {
		perror("Socket failed");
		return EXIT_FAILURE;
	}

	memset(&srv_sockaddr, 0, sizeof(struct sockaddr_un));
	srv_sockaddr.sun_family = AF_UNIX;
	strcpy(srv_sockaddr.sun_path, dsock_file);

	unlink(dsock_file);

	if (bind(srv_sock, (struct sockaddr *)&srv_sockaddr, sizeof(srv_sockaddr))) {
		perror("Bind failed");
		close(srv_sock);
		return EXIT_FAILURE;
	}

	if (listen(srv_sock, CLIENTS_COUNT) == -1) {
		printf("Listen failed");
		close(srv_sock);
		return EXIT_FAILURE;
	}

	while (1) {
		pid_t pid;
		printf("Listening connections on domain socket %s\n", dsock_file);

		memset(&clt_socksaddr, 0, sizeof(struct sockaddr_un));
		clt_sock = accept(srv_sock, (struct sockaddr *)&clt_socksaddr, &len);
		if (clt_sock == -1) {
			perror("Accept failed");
			close(srv_sock);
			return EXIT_FAILURE;
		}
		len = sizeof(clt_socksaddr);

		if (getpeername(clt_sock, (struct sockaddr*)&clt_socksaddr, &len) == -1) {
			perror("getpeername() failed");
			close(srv_sock);
			close(clt_sock);
			return EXIT_FAILURE;
		}

		printf("Client socket filepath: %s\n", clt_socksaddr.sun_path);

		pid = fork();
		if (pid == -1) {
			perror("Pid failed");
			close(srv_sock);
			close(clt_sock);
			return EXIT_FAILURE;
		}

		if (pid == 0)
			break;

		close(clt_sock);
	}


	while (1) {
		int ret;
		char buff[128];

		for (int i = 0; i < 128; ++i)
			buff[i] = '\0';

		ret = read(clt_sock, buff, sizeof(buff));
		if (ret == -1 || ret == 0) {
			perror("Read failed");
			close(clt_sock);
			return EXIT_FAILURE;
		} 

		printf("Recieved message: %s\n", buff);

		ret = write(clt_sock, buff, sizeof(buff));
		if (ret == -1 || ret == 0) {
			printf("Write failed");
			close(clt_sock);
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
