#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 255

#define handle_error(msg) \
        do { fprintf(stderr, msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char **argv)
{
        if (argc < 2)
                handle_error("invalid port");
        uint16_t port = atoi(argv[1]);

        int sock;
        if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
                handle_error("socket corrupted");

        struct sockaddr_in addr; 
        memset(&addr, 0, sizeof(struct sockaddr_in));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(port);

        if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
                handle_error("bind failed");

        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);

        if (recvfrom(sock, buffer, BUFFER_SIZE, 0, NULL, 0) < 0)
                handle_error("recvfrom failed");

        buffer[BUFFER_SIZE - 1] = '\0';
        printf("%s\n", buffer);

        close(sock);
        exit(0);
}
