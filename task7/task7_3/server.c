#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <poll.h>

#include "server_constants.h"

void close_sock(int *socks, int s) {
	for (int i = 0; i < CLIENTS_COUNT; ++i) {
		if (socks[i] == s) {
			close(socks[i]);
			socks[i] = -1;
			return;
		}
	}
}

void close_all(int *socks) {
	for (int i = 0; i < CLIENTS_COUNT; ++i) {
		close(socks[i]);
	}
}

int main() {
	int srv_sock;
	int clt_socks[CLIENTS_COUNT];
	struct sockaddr_in srv_sockaddr;
	struct sockaddr_in clt_sockaddr;
	int len, err, s = 0;

	char buff[1024];

	signal(SIGPIPE, SIG_IGN);
	memset(clt_socks, -1, sizeof(clt_socks));

	srv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (srv_sock == -1) {
		perror("Socket failed");
		return EXIT_FAILURE;
	}

	memset(&srv_sockaddr, 0, sizeof(struct sockaddr_in));
	srv_sockaddr.sin_family = AF_INET;
	srv_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srv_sockaddr.sin_port = htons(PORT);

	if (bind(srv_sock, (struct sockaddr *)&srv_sockaddr, sizeof(srv_sockaddr)) == -1) {
		perror("Bind failed");
		close(srv_sock);
		return EXIT_FAILURE;
	}

	if (listen(srv_sock, 5) == -1) {
		perror("Listen failed");
		close(srv_sock);
		return EXIT_FAILURE;
	}

	while (1) {
		struct pollfd fds[32];
		int ret;
		int fds_count = 0;
		int timeout = 5000;

		fds[0].fd = srv_sock;
		// means I'm wainting for POLLINs 
		fds[0].events = POLLIN;
		// revents is a field that is filled by poll(2)
		fds[0].revents = 0;
		fds_count++;

		for (int i = 0; i < CLIENTS_COUNT; ++i) {
			if (clt_socks[i] != -1) {
				printf("Add %d index %d\n", clt_socks[i], i);
				fds[fds_count].fd = clt_socks[i];
				fds[fds_count].events = POLLIN;
				fds[fds_count].revents = 0;
				fds_count++;
			}
		}
		
		// if something happened on file descriptors then...
		ret = poll(fds, fds_count, timeout);
		printf("Poll returned %d\n", ret);
		if (ret == 0) {
			printf("no activity on the sockets for >= 5 seconds\n");
			continue;
		} else if (ret > 0) {
			if (fds[0].revents == POLLIN) {
				printf("Ready to accept new connection\n");
				memset(&clt_sockaddr, 0, sizeof(struct sockaddr_in));
				clt_socks[s] = accept(srv_sock, 
						(struct sockaddr *)&clt_sockaddr, &len);
				if (clt_socks[s] == -1){
					perror("Accept failed, continue execution on the next iteration");
					continue;

					// close(srv_sock);
					// return EXIT_FAILURE;
				}
				s++;
			}

			for (int i = 1; i < fds_count; ++i) {
				if (fds[i].revents == POLLIN) {
					printf("POLLIN event on socket %d\n", fds[i].fd);
					ret = read(fds[i].fd, buff, sizeof(buff));
					if (ret == -1) {
						perror("Read failed");
						close_sock(clt_socks, fds[i].fd);
						fds_count--;
						break;
					}
					if (write(fds[i].fd, buff, ret) == 0) {
						perror("Write failed");
						close_sock(clt_socks, fds[i].fd);
						fds_count--;
						break;
					}
				}
			}
		}
	}
	
	close_all(clt_socks);
	
	return EXIT_SUCCESS;
}
